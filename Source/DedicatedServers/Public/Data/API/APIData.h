// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GamePlayTags/DedicatedServerTags.h"
#include "APIData.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UAPIData : public UDataAsset
{
	GENERATED_BODY()

public:
	FString GetAPIEndpoint(const FGameplayTag& APIEndpoint);
protected:

//	not used in any code only used for label apidata
	UPROPERTY(EditDefaultsOnly)
	FString Name;

	UPROPERTY(EditDefaultsOnly)
	FString InvokeURL;

	UPROPERTY(EditDefaultsOnly)
	FString Stage;

	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag ,FString> Resources;
	
};
