// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DS_PlayerState.h"

#include "UI/HTTP/HTTPRequestTypes.h"

void ADS_PlayerState::BeginPlay()
{
	Super::BeginPlay();

	PlayerStatsManager = NewObject<UPlayerStatsManager>(this, PlayerStatsManagerClass);
	LoadPlayerStats();

	PlayerStatsManager->OnRetrievePlayerStatsResponseReceived.AddDynamic(this, &ADS_PlayerState::OnRetrievePlayerStats);
}

void ADS_PlayerState::RecordPlayerStats(const FDSPlayerStatsInput& PlayerStatsInput) const
{
	check(IsValid(PlayerStatsManager));
	PlayerStatsManager->RecordPlayerStats(PlayerStatsInput);
}


void ADS_PlayerState::LoadPlayerStats() const
{
	check(IsValid(PlayerStatsManager));
	PlayerStatsManager->RetrievePlayerStats();
}

void ADS_PlayerState::OnRetrievePlayerStats(const FDSPlayerStatsResponse& RetrievePlayerStatsResponse)
{
	
}