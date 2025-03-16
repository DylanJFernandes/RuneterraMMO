// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"

#include "Components/Health/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/RuneterraGameMode.h"
#include "Player/RuneterraPlayerController.h"
#include "AbilitySystem/RuneterraAbilitySystemComponent.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->bOnlyOwnerSee = false;
	GetMesh()->bOwnerNoSee = true;
	GetMesh()->bReceivesDecals = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health");
	HealthComponent->SetIsReplicated(true);
	
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseCharacter::GiveDefaultAbilites()
{
	check(AbilitySystemComponent);
	if(!HasAuthority()) return;

	for (TSubclassOf<UGameplayAbility> AbilityClass: DefaultAbilites)
	{
		const FGameplayAbilitySpec AbilitySpec(AbilityClass, 1 );
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

void ABaseCharacter::OnDeathStarted(AActor* DyingActor, AActor* DeathInstigator)
{
	int32 Index = FMath::RandRange(0, DeathMontages.Num() - 1);
	UAnimMontage* DeathMontage = DeathMontages[Index];
	ARuneterraPlayerController* VictimController = Cast<ARuneterraPlayerController>(GetController());
	if (GetNetMode() != NM_DedicatedServer)
	{
		DeathEffects(DeathInstigator, DeathMontage);
		if (IsValid(VictimController))
		{
			DisableInput(VictimController);
			if (IsLocallyControlled())
			{
				VictimController->bPawnAlive = false;
			}
		}
	}
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	// TODO: Change "ECC_GameTraceChannel1" to "ECC_Weapon"
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(HealthComponent))
	{
		HealthComponent->OnDeathStarted.AddDynamic(this, &ABaseCharacter::OnDeathStarted);
	}
	
}

