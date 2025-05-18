// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
#include "Player/RuneterraPlayerController.h"
#include "AbilitySystem/RuneterraAbilitySystemComponent.h"
#include "Player/RuneterraPlayerState.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<URuneterraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	ARuneterraPlayerController* VictimController = Cast<ARuneterraPlayerController>(GetController());
	if (IsLocallyControlled() && IsValid(VictimController))
	{
		VictimController->bPawnAlive = true;
	}
	
	
}
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	APlayerController* RuneterraController = Cast<APlayerController>(NewController);
	if (IsValid(RuneterraController))
	{
		EnableInput(RuneterraController);
	}
	InitAbilitySystemComponent();
	GiveDefaultAbilites();
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilitySystemComponent();
}

void APlayerCharacter::InitAbilitySystemComponent()
{
	ARuneterraPlayerState* RuneterraPlayerState = GetPlayerState<ARuneterraPlayerState>();
	if (IsValid (RuneterraPlayerState))
	{
		AbilitySystemComponent = Cast<URuneterraAbilitySystemComponent>(RuneterraPlayerState);
		AbilitySystemComponent->InitAbilityActorInfo(RuneterraPlayerState,this);
	}
}

