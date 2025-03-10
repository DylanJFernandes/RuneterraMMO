// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_HostGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Player/DSPlayerController.h"

void ADS_HostGameMode::BeginPlay()
{
	Super::BeginPlay();
}


void ADS_HostGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
                                  FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	const FString PlayerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
	const FString Username = UGameplayStatics::ParseOption(Options, TEXT("Username"));

	TryAcceptPlayerSessions(PlayerSessionId,Username,ErrorMessage);
		
}

FString ADS_HostGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
	const FString& Options, const FString& Portal)
{
	FString InitializedString =  Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	const FString PlayerSessionId = UGameplayStatics::ParseOption(Options,TEXT("PlaySessionId"));
	const FString Username = UGameplayStatics::ParseOption(Options,TEXT("Username"));

	if (ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(NewPlayerController); IsValid(DSPlayerController))
	{
		DSPlayerController->PlayerSessionId = PlayerSessionId;
		DSPlayerController->Username = Username;
	}
	
	return InitializedString;
}

void ADS_HostGameMode::TryAcceptPlayerSessions(const FString& PlayerSessionId, const FString& Username, FString& OutErrorMessage)
{
	if (PlayerSessionId.IsEmpty() || Username.IsEmpty())
	{
		OutErrorMessage = TEXT("Player Session Id and/or Username invalid.");
		return;
	}

#if WITH_GAMELIFT
	Aws::GameLift::Server::Model::DescribePlayerSessionsRequest DescribePlayerSessionsRequest;
	DescribePlayerSessionsRequest.SetPlayerSessionId(TCHAR_TO_ANSI(*PlayerSessionId));

	const auto& DescribePlayerSessionsOutcome = Aws::GameLift::Server::DescribePlayerSessions(DescribePlayerSessionsRequest);
	if (!DescribePlayerSessionsOutcome.IsSuccess())
	{
		OutErrorMessage = TEXT("DescribePlayerSessions Failed.");
		return;
	}
		int32 Count = 0;
		const auto& DescribePlayerSessionsResult = DescribePlayerSessionsOutcome.GetResult();

		const Aws::GameLift::Server::Model::PlayerSession* PlayerSessions = DescribePlayerSessionsResult.GetPlayerSessions(Count);
		if (PlayerSessions == nullptr || Count == 0)
		{
			OutErrorMessage = TEXT("GetPlayerSessions Failed.");
			return;
		}

		for (int32 i = 0; i < Count; i++)
		{
			const Aws::GameLift::Server::Model::PlayerSession& PlayerSession = PlayerSessions[i];
			if (!Username.Equals(PlayerSession.GetPlayerId())) continue;
			if (PlayerSession.GetStatus() != Aws::GameLift::Server::Model::PlayerSessionStatus::RESERVED)
			{
				OutErrorMessage = FString::Printf(TEXT("Session for %s note RESERVED; Fail PreLogin"), *Username);
				return;
			}

			const auto& AcceptPlayerSessionsOutcome = Aws::GameLift::Server::AcceptPlayerSession(TCHAR_TO_ANSI(*PlayerSessionId));
			OutErrorMessage =  AcceptPlayerSessionsOutcome.IsSuccess() ? "" : FString::Printf(TEXT("Failed to accept player session for %s"),*Username);
		}
	
	
		
#endif
	
}