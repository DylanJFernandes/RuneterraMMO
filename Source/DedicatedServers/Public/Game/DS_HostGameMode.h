// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DS_GameMode.h"
#include "DS_HostGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_HostGameMode : public ADS_GameMode
{
	GENERATED_BODY()
protected:
	
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = L"") override;
	
	virtual void BeginPlay() override;
	
private:

	void TryAcceptPlayerSessions(const FString& PlayerSessionId, const FString& Username, FString&  OutErrorMessage);
};
