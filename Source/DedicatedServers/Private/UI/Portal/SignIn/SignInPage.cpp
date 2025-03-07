// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/SignInPage.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

void USignInPage::UpdateStatusMessage(const FString& Message, bool bShouldResetWidgets)
{
	TextBlock_StatusMessage->SetText(FText::FromString(Message));
	if (bShouldResetWidgets)
	{
		Button_SignIn->SetIsEnabled(true);
	}
}

void USignInPage::NativeConstruct()
{
	Super::NativeConstruct();
	
	check(TextBox_Password);
	check(CheckBox_ShowPassword);
	
	TextBox_Password->SetIsPassword(true);
	CheckBox_ShowPassword->SetIsChecked(false);
	
	CheckBox_ShowPassword->OnCheckStateChanged.AddDynamic(this, &USignInPage::ShowPassword);
}

void USignInPage::ShowPassword(bool bIsChecked)
{
	TextBox_Password->SetIsPassword(!bIsChecked);
}
