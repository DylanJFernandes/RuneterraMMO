// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameLiftServerSDK.h"
#include "GameFramework/GameMode.h"
#include "DS_GameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDS_MMOGameMode, Log, All);

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_GameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ADS_GameMode();
protected:
	virtual void BeginPlay() override;
	
	void SetServerParameters(FServerParameters& ServerParameters);

private:
	//process Parameters need to remain in scope for the lifetime of the app
	FProcessParameters M_Parameters;

	void ParseCommandLinePort(int32& OutPort);
	void InitGameLift();
};
