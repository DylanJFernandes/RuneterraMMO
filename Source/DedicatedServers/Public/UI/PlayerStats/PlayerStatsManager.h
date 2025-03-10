// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HttpFwd.h"
#include "UI/HTTP/HTTPRequestManager.h"
#include "PlayerStatsManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRetrievePlayerStatsResponseReceived, const FDSPlayerStatsResponse&,
                                            RetrievePlayerStatsResponse);
 
struct FDSPlayerStatsInput;

UCLASS()
class DEDICATEDSERVERS_API UPlayerStatsManager : public UHTTPRequestManager
{
	GENERATED_BODY()

public:
	void RecordPlayerStats(const FDSPlayerStatsInput& DSPlayerStatsInput);
	void RetrievePlayerStats();
	
	void RecordPlayerStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void RetrievePlayerStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UPROPERTY()
	FOnRetrievePlayerStatsResponseReceived OnRetrievePlayerStatsResponseReceived;
	UPROPERTY()
	FAPIStatusMessage RetrievePlayerStatsStatusMesssage;

};
