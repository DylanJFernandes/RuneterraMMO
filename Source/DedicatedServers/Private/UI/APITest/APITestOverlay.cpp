// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/APITest/APITestOverlay.h"

#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "UI/API/ListFleets/FleetId.h"
#include "UI/API/ListFleets/ListFleetBox.h"
#include "UI/APITest/APIHTTPTestManager.h"
#include "UI/HTTP/HTTPRequestTypes.h"

void UAPITestOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	check(APITestManagerClass);
	HTTPTestManager = NewObject<UAPIHTTPTestManager>(this,APITestManagerClass);

	check(ListFleetBox);
	check(ListFleetBox->Button_ListFleets);
	ListFleetBox->Button_ListFleets->OnClicked.AddDynamic(this, &UAPITestOverlay::ListFleetsButtonClicked);	
}

void UAPITestOverlay::ListFleetsButtonClicked()
{
	check(HTTPTestManager);
	HTTPTestManager->OnListFleetsResponseReceived.AddDynamic(this, &UAPITestOverlay::OnListFleetsResponseReceived);
	HTTPTestManager->ListFleets();
	
	ListFleetBox->Button_ListFleets->SetIsEnabled(false);
	
}

void UAPITestOverlay::OnListFleetsResponseReceived(const FDSListFleetsResponse& ListFleetsResponse, bool BWasSuccessful)
{
	if (HTTPTestManager->OnListFleetsResponseReceived.IsAlreadyBound(this,&UAPITestOverlay::OnListFleetsResponseReceived))
	{
		HTTPTestManager->OnListFleetsResponseReceived.RemoveDynamic(this,&UAPITestOverlay::OnListFleetsResponseReceived);
	}
	ListFleetBox->ScrollBox_ListFleets->ClearChildren();
	if (BWasSuccessful)
	{
		for (const FString& FleetId : ListFleetsResponse.FleetIds)
		{
			UFleetId* FleetIdWidget = CreateWidget<UFleetId>(this, FleetIdWidgetClass);
			FleetIdWidget->TextBlock_ListFleets->SetText(FText::FromString(FleetId));
			ListFleetBox->ScrollBox_ListFleets->AddChild(FleetIdWidget);
		}
		
	}
	else
	{
		UFleetId* FleetIdWidget = CreateWidget<UFleetId>(this, FleetIdWidgetClass);
		FleetIdWidget->TextBlock_ListFleets->SetText(FText::FromString("Something Went WRONG!"));
		ListFleetBox->ScrollBox_ListFleets->AddChild(FleetIdWidget);
	}
	ListFleetBox->Button_ListFleets->SetIsEnabled(true);
}
