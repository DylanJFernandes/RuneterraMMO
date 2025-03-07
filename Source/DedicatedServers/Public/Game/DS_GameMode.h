#pragma once

#include "CoreMinimal.h"
#include "Game/DS_GameModeBase.h"
#include "GameLiftServerSDK.h"
#include "DS_GameMode.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogDS_GameMode, Log, All);
class UDS_GameInstanceSubsystem;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_GameMode : public ADS_GameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;


private:

	FProcessParameters ProcessParameters;
	UPROPERTY()
	TObjectPtr<UDS_GameInstanceSubsystem> DSGameInstanceSubsystem;
	
	void InitGameLift();
	void SetServerParameters(FServerParameters& OutServerParameters);
};