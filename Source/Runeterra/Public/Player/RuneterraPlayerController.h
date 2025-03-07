﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/DSPlayerController.h"
#include "RuneterraPlayerController.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerStateReplicated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameMenuOpen, bool, bOpen);

UCLASS()
class RUNETERRA_API ARuneterraPlayerController : public ADSPlayerController
{
	GENERATED_BODY()
	
public:

	ARuneterraPlayerController();
	
	bool bPawnAlive;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_PlayerState() override;
	virtual void EnableInput(class APlayerController* PlayerController) override;
	virtual void DisableInput(class APlayerController* PlayerController) override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateReplicated OnPlayerStateReplicated;

	UPROPERTY(BlueprintAssignable)
	FOnGameMenuOpen OnGameMenuOpen;
	
protected:
	
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> PlayerIMC;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> JumpAction;	

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> GameMenuAction;	

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Crouch();
	void Input_Jump();
	
	
	void Input_GameMenu();
	bool bGameMenuOpen;
};