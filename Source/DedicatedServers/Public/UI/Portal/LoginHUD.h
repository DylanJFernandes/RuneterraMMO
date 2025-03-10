// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Portal/Interfaces/SignInManagement.h"
#include "LoginHUD.generated.h"

class UDashboardOverlay;
class USignInOverlay;

UCLASS()
class DEDICATEDSERVERS_API ALoginHUD : public AHUD, public ISignInManagement
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USignInOverlay> SignInOverlayClass;

	virtual void OnSignIn() override;
	
protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> LobbyMap;

private:

	UPROPERTY()
	TObjectPtr<USignInOverlay> SignInOverlay;
};
