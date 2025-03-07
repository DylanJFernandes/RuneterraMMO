// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RuneterraPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Interfaces/PlayerInterface.h"

ARuneterraPlayerController::ARuneterraPlayerController()
{
	bReplicates = true;
	bPawnAlive = true;
	bGameMenuOpen = false;
}

void ARuneterraPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	bPawnAlive = true;
}

void ARuneterraPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	OnPlayerStateReplicated.Broadcast();
}

void ARuneterraPlayerController::EnableInput(APlayerController* PlayerController)
{
	Super::EnableInput(PlayerController);
	if (IsValid(GetPawn()) && GetPawn()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_EnableGameActions(GetPawn(), true);
	}
}

void ARuneterraPlayerController::DisableInput(APlayerController* PlayerController)
{
	Super::DisableInput(PlayerController);
	if (IsValid(GetPawn()) && GetPawn()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_EnableGameActions(GetPawn(), false);
	}
}

void ARuneterraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(PlayerIMC, 0);
	}
}

void ARuneterraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* PlayerInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	PlayerInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARuneterraPlayerController::Input_Move);
	PlayerInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARuneterraPlayerController::Input_Look);
	PlayerInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ARuneterraPlayerController::Input_Crouch);
	PlayerInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ARuneterraPlayerController::Input_Jump);
	
	PlayerInputComponent->BindAction(GameMenuAction, ETriggerEvent::Triggered, this, &ARuneterraPlayerController::Input_GameMenu);
}

void ARuneterraPlayerController::Input_Move(const FInputActionValue& InputActionValue)
{
	if (!bPawnAlive) return;
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void ARuneterraPlayerController::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	AddYawInput(InputAxisVector.X);
	AddPitchInput(InputAxisVector.Y);
}

void ARuneterraPlayerController::Input_Crouch()
{
	if (!bPawnAlive) return;
	if (GetPawn() == nullptr || !GetPawn()->Implements<UPlayerInterface>()) return;
	IPlayerInterface::Execute_Initiate_Crouch(GetPawn());
}

void ARuneterraPlayerController::Input_Jump()
{
	if (!bPawnAlive) return;
	if (GetPawn() == nullptr || !GetPawn()->Implements<UPlayerInterface>()) return;
	IPlayerInterface::Execute_Initiate_Jump(GetPawn());
}

void ARuneterraPlayerController::Input_GameMenu()
{
	bGameMenuOpen = !bGameMenuOpen;
	if (bGameMenuOpen)
	{
		FInputModeGameAndUI InputMode;
		SetInputMode(InputMode);
		SetShowMouseCursor(true);
		OnGameMenuOpen.Broadcast(true);
	}
	else
	{
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
		SetShowMouseCursor(false);
		OnGameMenuOpen.Broadcast(false);
	}
}