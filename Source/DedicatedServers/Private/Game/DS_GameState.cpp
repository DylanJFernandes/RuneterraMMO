// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_GameState.h"

#include "Net/UnrealNetwork.h"
 #include "Player/PlayerInfoState.h"
 
 ADS_GameState::ADS_GameState()
 {
 	bReplicates = true;
 }
 
void ADS_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
 {
 	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
 
 	DOREPLIFETIME(ADS_GameState, PlayerInfoState);
 }
 
void ADS_GameState::BeginPlay()
 {
 	Super::BeginPlay();
 
 	if (HasAuthority())
 	{
 		CreatePlayerInfoState();
 		OnPlayerInfoStateInitialized.Broadcast(PlayerInfoState);
 	}
 }
 
void ADS_GameState::CreatePlayerInfoState()
 {
 	if (UWorld* World = GetWorld(); IsValid(World))
 	{
 		FActorSpawnParameters SpawnParams;
 		PlayerInfoState = World->SpawnActor<APlayerInfoState>(
			 APlayerInfoState::StaticClass(),
			 FVector::ZeroVector,
			 FRotator::ZeroRotator,
			 SpawnParams
		 );
 		if (IsValid(PlayerInfoState))
 		{
 			PlayerInfoState->SetOwner(this);
 		}
 	}
 }
 
void ADS_GameState::OnRep_PlayerInfoState()
 {
 	OnPlayerInfoStateInitialized.Broadcast(PlayerInfoState);
 }