// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"

#include "EnhancedInputComponent.h"
#include "Components/Health/HealthComponent.h"
#include "Data/WeaponData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Combat/CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/RuneterraGameMode.h"
#include "Player/RuneterraPlayerController.h"
#include "Weapon/Weapon.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->bOnlyOwnerSee = false;
	GetMesh()->bOwnerNoSee = true;
	GetMesh()->bReceivesDecals = false;

	Combat = CreateDefaultSubobject<UCombatComponent>("Combat");
	Combat->SetIsReplicated(true);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health");
	HealthComponent->SetIsReplicated(true);

	DefaultFieldOfView = 90.f;
	bWeaponFirstReplicated = false;

	bEnableGameActions = true;
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
	ARuneterraPlayerController* VictimController = Cast<ARuneterraPlayerController>(GetController());
	if (IsLocallyControlled() && IsValid(VictimController))
	{
		VictimController->bPawnAlive = true;
	}
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(BasicAttackAction, ETriggerEvent::Started, this, &ABaseCharacter::Input_BasicAttack_Pressed);
	EnhancedInputComponent->BindAction(BasicAttackAction, ETriggerEvent::Completed, this, &ABaseCharacter::Input_BasicAttack_Released);
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	APlayerController* RuneterraController = Cast<APlayerController>(NewController);
	if (IsValid(RuneterraController))
	{
		EnableInput(RuneterraController);
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(Combat) && IsValid(Combat->CurrentWeapon))
	{
		HANDLE_SocketTransform = Combat->CurrentWeapon->GetMesh()->GetSocketTransform(FName("HANDLE_Socket"), RTS_World);
		FVector OutLocation;
		FRotator OutRotation;
		GetMesh()->TransformToBoneSpace(FName("hand_r"), HANDLE_SocketTransform.GetLocation(), HANDLE_SocketTransform.GetRotation().Rotator(), OutLocation, OutRotation);
		HANDLE_SocketTransform.SetLocation(OutLocation);
		HANDLE_SocketTransform.SetRotation(OutRotation.Quaternion());
	}
}

void ABaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

FName ABaseCharacter::GetWeaponAttachPoint_Implementation(const FGameplayTag& WeaponType) const
{
	checkf(Combat->WeaponData, TEXT("No Weapon Data Asset - Please fill out BP_RuneterraCharacter"));
	return Combat->WeaponData->GripPoints.FindChecked(WeaponType);
}

bool ABaseCharacter::DoDamage_Implementation(float DamageAmount, AActor* DamageInstigator)
{
	if (!IsValid(HealthComponent)) return false;

	const bool bLethal = HealthComponent->ChangeHealthByAmount(-DamageAmount, DamageInstigator);
	const int32 MontageSelection = FMath::RandRange(0, HitReacts.Num() - 1);
	Multicast_HitReact(MontageSelection);
	return bLethal;
}


AWeapon* ABaseCharacter::GetCurrentWeapon_Implementation()
{
	if (!IsValid(Combat)) return nullptr;
	return Combat->CurrentWeapon;
}

void ABaseCharacter::Multicast_HitReact_Implementation(int32 MontageIndex)
{
	if (GetNetMode() != NM_DedicatedServer && !IsLocallyControlled())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(HitReacts[MontageIndex]);
	}
}

void ABaseCharacter::Input_BasicAttack_Pressed()
{
	if (!bEnableGameActions) return;
	Combat->Initiate_BasicAttack_Pressed();
}

void ABaseCharacter::Input_BasicAttack_Released()
{
	if (!bEnableGameActions) return;
	Combat->Initiate_BasicAttack_Released();
}

void ABaseCharacter::Initiate_Crouch_Implementation()
{
	GetCharacterMovement()->bWantsToCrouch = !GetCharacterMovement()->bWantsToCrouch;
}

void ABaseCharacter::Initiate_Jump_Implementation()
{
	if (GetCharacterMovement()->bWantsToCrouch)
	{
		GetCharacterMovement()->bWantsToCrouch = false;
	}
	else
	{
		Jump();
	}
}

bool ABaseCharacter::IsDeadOrDying_Implementation()
{
	if (IsValid(HealthComponent))
	{
		return HealthComponent->IsDeadOrDying();
	}
	return true;
}

void ABaseCharacter::WeaponReplicated_Implementation()
{
	if (!bWeaponFirstReplicated)
	{
		bWeaponFirstReplicated = true;
		OnWeaponFirstReplicated.Broadcast(Combat->CurrentWeapon);
	}
}

void ABaseCharacter::EnableGameActions_Implementation(bool bEnable)
{
	bEnableGameActions = bEnable;
	if (!bEnable && IsValid(Combat))
	{
		Combat->Initiate_BasicAttack_Released();
	}
}
