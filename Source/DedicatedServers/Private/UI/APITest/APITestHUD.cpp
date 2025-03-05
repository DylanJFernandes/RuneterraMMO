// Fill out your copyright notice in the Description page of Project Settings.


#include "DedicatedServers/Public/UI/APITest/APITestHUD.h"

#include "Kismet/GameplayStatics.h"
#include "UI/APITest/APITestOverlay.h"

void AAPITestHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetOwningPlayerController();
	if (IsValid(PlayerController) && APITestOverlayClass)
	{
		APITestOverlay = CreateWidget<UAPITestOverlay>(PlayerController, APITestOverlayClass);
		APITestOverlay->AddToViewport();
	}
}
