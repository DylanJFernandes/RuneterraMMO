// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Portal/Interfaces/SignOutManagement.h"
#include "LobbyHUD.generated.h"

class UDashboardOverlay;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ALobbyHUD : public AHUD, public ISignOutManagement
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDashboardOverlay> DashboardOverlayClass;
	
	virtual void OnSignOut() override;

protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> SignInMap;

private:
	
	UPROPERTY()
	TObjectPtr<UDashboardOverlay> DashboardOverlay;
};
