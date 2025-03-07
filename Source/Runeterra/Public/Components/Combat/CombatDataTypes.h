#pragma once

#include "CombatDataTypes.generated.h"

USTRUCT(BlueprintType)
struct FPlayerHitResult
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FVector_NetQuantize Start = FVector_NetQuantize::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	FVector_NetQuantize End = FVector_NetQuantize::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	bool bHitPlayer = false;
	
	UPROPERTY(BlueprintReadWrite)
	bool bHitPlayerLastFrame = false;

	UPROPERTY(BlueprintReadWrite)
	bool bHeadShot = false;
};

USTRUCT(BlueprintType)
struct FReticleParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ShapeCutFactor_RoundFired = 0.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ScaleFactor_RoundFired = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RoundFiredInterpSpeed = 20.f;
};