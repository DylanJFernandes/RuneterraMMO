// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyHUD.h"

#include "Kismet/GameplayStatics.h"
#include "UI/Portal/Dashboard/DashboardOverlay.h"

void ALobbyHUD::BeginPlay()
{
	Super::BeginPlay();
	
	
	APlayerController* OwningPlayerController = GetOwningPlayerController();
	DashboardOverlay = CreateWidget<UDashboardOverlay>(OwningPlayerController, DashboardOverlayClass);
	if (IsValid(DashboardOverlay))
	{
		DashboardOverlay->AddToViewport();
	}
	
	FInputModeGameAndUI InputModeData;
	OwningPlayerController->SetInputMode(InputModeData);
	OwningPlayerController->SetShowMouseCursor(true);
}

void ALobbyHUD::OnSignOut()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, SignInMap);
}
