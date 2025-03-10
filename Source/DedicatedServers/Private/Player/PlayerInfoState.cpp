// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerInfoState.h"
#include "Net/UnrealNetwork.h"
 
 APlayerInfoState::APlayerInfoState()
 {
 	PrimaryActorTick.bCanEverTick = false;
 	bReplicates = true;
 	bAlwaysRelevant = true;
 }
 
void APlayerInfoState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
 {
 	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
 	DOREPLIFETIME(APlayerInfoState, PlayerInfoArray);
 }
 
void APlayerInfoState::AddPlayerInfo(const FPlayerInfo& PlayerInfo)
 {
 	PlayerInfoArray.AddPlayer(PlayerInfo);
 }
 
void APlayerInfoState::RemovePlayerInfo(const FString& Username)
 {
 	PlayerInfoArray.RemovePlayer(Username);
 }

TArray<FPlayerInfo> APlayerInfoState::GetPlayerInfos() const
{
 	return PlayerInfoArray.Players;
}

void APlayerInfoState::OnRep_PlayerInfoPlayerInfo()
 {
 	FPlayerInfoStateDelta Delta = ComputePlayerInfoDelta(LastPlayerInfoArray.Players, PlayerInfoArray.Players);
 	for (const auto& PlayerInfo : Delta.AddedPlayers)
 	{
 		OnPlayerInfoAdded.Broadcast(PlayerInfo);
 	}
 	for (const auto& PlayerInfo : Delta.RemovedPlayers)
 	{
 		OnPlayerInfoRemoved.Broadcast(PlayerInfo);
 	}
 
 	LastPlayerInfoArray = PlayerInfoArray;
 }
 
FPlayerInfoStateDelta APlayerInfoState::ComputePlayerInfoDelta(const TArray<FPlayerInfo>& OldArray, const TArray<FPlayerInfo>& NewArray)
 {
 	FPlayerInfoStateDelta Delta;
 
 	TMap<FString, const FPlayerInfo*> OldMap;
 	TMap<FString, const FPlayerInfo*> NewMap;
 
 	for (const auto& PlayerInfo : OldArray)
 	{
 		OldMap.Add(PlayerInfo.Username, &PlayerInfo);
 	}
 	for (const auto& PlayerInfo : NewArray)
 	{
 		NewMap.Add(PlayerInfo.Username, &PlayerInfo);
 	}
 	for (const auto& OldPlayerInfo : OldArray)
 	{
 		if (!NewMap.Contains(OldPlayerInfo.Username))
 		{
 			Delta.RemovedPlayers.Add(OldPlayerInfo);
 		}
 	}
 	for (const auto& NewPlayerInfo : NewArray)
 	{
 		if (!OldMap.Contains(NewPlayerInfo.Username))
 		{
 			Delta.AddedPlayers.Add(NewPlayerInfo);
 		}
 	}
 
 	return Delta;
 }

