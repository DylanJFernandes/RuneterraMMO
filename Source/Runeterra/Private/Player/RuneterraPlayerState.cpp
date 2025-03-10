// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RuneterraPlayerState.h"

void ARuneterraPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void ARuneterraPlayerState::OnLogout(const FString& Username)
{
	SavePlayerStats(Username);
}

void ARuneterraPlayerState::OnRetrievePlayerStats(const FDSPlayerStatsResponse& RetrievePlayerStatsResponse)
{
	Super::OnRetrievePlayerStats(RetrievePlayerStatsResponse);

	LocalPlayerStats.currentSelectedCharacterIndex = RetrievePlayerStatsResponse.currentSelectedCharacterIndex;
	LocalPlayerStats.slotOne_CharacterStats = RetrievePlayerStatsResponse.slotOne_CharacterStats;
	LocalPlayerStats.slotTwo_CharacterStats = RetrievePlayerStatsResponse.slotTwo_CharacterStats;
	LocalPlayerStats.slotThree_CharacterStats = RetrievePlayerStatsResponse.slotThree_CharacterStats;
	LocalPlayerStats.slotFour_CharacterStats = RetrievePlayerStatsResponse.slotFour_CharacterStats;
}

// TODO: Make a function that will periodically call this
void ARuneterraPlayerState::SavePlayerStats(const FString& Username)
{
	// Setup PlayerStatsInput for sending request to DB
	FDSPlayerStatsInput PlayerStatsInput;
	PlayerStatsInput.username = Username;
	PlayerStatsInput.playerStats = LocalPlayerStats;
	PlayerStatsInput.preferredSlotToSave = "all";

	// Save PlayerStats to DB
	RecordPlayerStats(PlayerStatsInput);
}
