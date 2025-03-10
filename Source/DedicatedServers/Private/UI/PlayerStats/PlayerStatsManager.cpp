// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerStats/PlayerStatsManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/API/APIData.h"
#include "DedicatedServers/DedicatedServers.h"
#include "Interfaces/IHttpRequest.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "GamePlayTags/DedicatedServerTags.h"
#include "Interfaces/IHttpResponse.h"
#include "Player/DSLocalPlayerSubsystem.h"

void UPlayerStatsManager::RecordPlayerStats(const FDSPlayerStatsInput& DSPlayerStatsInput)
{
	FString JsonString;
	FJsonObjectConverter::UStructToJsonObjectString(FDSPlayerStatsInput::StaticStruct(), &DSPlayerStatsInput, JsonString);
	GEngine->AddOnScreenDebugMessage(-1,600,FColor::Red,JsonString);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString ApiUrl = APIData->GetAPIEndpoint(DedicatedServerTags::PlayerStatsAPI::RecordPlayerStats);
	Request->OnProcessRequestComplete().BindUObject(this, &UPlayerStatsManager::RecordPlayerStats_Response);
	Request->SetURL(ApiUrl);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(JsonString);
	Request->ProcessRequest();
}

void UPlayerStatsManager::RecordPlayerStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogDedicatedServers, Error, TEXT("RecordPlayerStats failed."))
		return;
	}
	
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		ContainsErrors(JsonObject);
	}
}

void UPlayerStatsManager::RetrievePlayerStats()
{
	RetrievePlayerStatsStatusMesssage.Broadcast(TEXT("Retrieving match stats..."), false);
	UDSLocalPlayerSubsystem* LocalPlayerSubsystem = GetDSLocalPlayerSubsystem();
	if (!IsValid(LocalPlayerSubsystem)) return;
	check(APIData);
 	
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString ApiUrl = APIData->GetAPIEndpoint(DedicatedServerTags::PlayerStatsAPI::RetrievePlayerStats);
	Request->OnProcessRequestComplete().BindUObject(this, &UPlayerStatsManager::RetrievePlayerStats_Response);
	Request->SetURL(ApiUrl);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
 
	TMap<FString, FString> Params = {
		{ TEXT("accessToken"), LocalPlayerSubsystem->GetAuthResult().AccessToken }
	};
	const FString Content = SerializeJsonContent(Params);
 	
	Request->SetContentAsString(Content);
	Request->ProcessRequest();
}
 
void UPlayerStatsManager::RetrievePlayerStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		OnRetrievePlayerStatsResponseReceived.Broadcast(FDSPlayerStatsResponse());
		return;
	}
 
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			OnRetrievePlayerStatsResponseReceived.Broadcast(FDSPlayerStatsResponse());
			return;
		}
 
		FDSPlayerStatsResponse RetrievePlayerStatsResponse;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &RetrievePlayerStatsResponse);
		//RetrievePlayerStatsResponse.Dump();
 
		OnRetrievePlayerStatsResponseReceived.Broadcast(RetrievePlayerStatsResponse);
	}	
}
