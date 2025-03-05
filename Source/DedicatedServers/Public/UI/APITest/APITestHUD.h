// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DebugCameraHUD.h"
#include "APITestHUD.generated.h"

class UAPITestOverlay;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API AAPITestHUD : public ADebugCameraHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAPITestOverlay> APITestOverlayClass;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UAPITestOverlay> APITestOverlay;
};
