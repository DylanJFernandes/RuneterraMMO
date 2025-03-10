// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DS_GameState.generated.h"

class APlayerInfoState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerInfoStateInitialized, APlayerInfoState*, PlayerInfoState);

UCLASS()
class DEDICATEDSERVERS_API ADS_GameState : public AGameState
{
	GENERATED_BODY()
public:
	ADS_GameState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
 
	UPROPERTY(ReplicatedUsing=OnRep_PlayerInfoState)
	TObjectPtr<APlayerInfoState> PlayerInfoState;
 
	UPROPERTY(BlueprintAssignable)
	FOnPlayerInfoStateInitialized OnPlayerInfoStateInitialized;
 
protected:
	virtual void BeginPlay() override;
	void CreatePlayerInfoState();
private:
 
	UFUNCTION()
	void OnRep_PlayerInfoState();
};
