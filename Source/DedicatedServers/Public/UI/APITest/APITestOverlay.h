// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APITestOverlay.generated.h"

class UFleetId;
class UAPIHTTPTestManager;
class UListFleetBox;
struct FDSListFleetsResponse;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UAPITestOverlay : public UUserWidget
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAPIHTTPTestManager> APITestManagerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFleetId> FleetIdWidgetClass;
	
	protected:
	virtual void NativeConstruct() override;
private:
	UPROPERTY(meta = (BindWidget));
	TObjectPtr<UListFleetBox> ListFleetBox;

	UPROPERTY();
	TObjectPtr<UAPIHTTPTestManager> HTTPTestManager;

	UFUNCTION()
	void ListFleetsButtonClicked();

	UFUNCTION()
	void OnListFleetsResponseReceived(const FDSListFleetsResponse& ListFleetsResponse, bool BWasSuccessful);

	
};
