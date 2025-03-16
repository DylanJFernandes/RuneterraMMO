// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "Components/Health/HealthComponent.h"
#include "Data/WeaponData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Combat/CombatComponent.h"
#include "Game/RuneterraGameMode.h"
#include "Player/RuneterraPlayerController.h"
#include "AbilitySystem/RuneterraAbilitySystemComponent.h"
#include "Player/RuneterraPlayerState.h"
#include "Weapon/Weapon.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	

	Combat = CreateDefaultSubobject<UCombatComponent>("Combat");
	Combat->SetIsReplicated(true);

	DefaultFieldOfView = 90.f;
	bWeaponFirstReplicated = false;

	bEnableGameActions = true;
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

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(BasicAttackAction, ETriggerEvent::Started, this, &APlayerCharacter::Input_BasicAttack_Pressed);
	EnhancedInputComponent->BindAction(BasicAttackAction, ETriggerEvent::Completed, this, &APlayerCharacter::Input_BasicAttack_Released);
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

void APlayerCharacter::Tick(float DeltaTime)
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

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilitySystemComponent();
}

FName APlayerCharacter::GetWeaponAttachPoint_Implementation(const FGameplayTag& WeaponType) const
{
	checkf(Combat->WeaponData, TEXT("No Weapon Data Asset - Please fill out BP_RuneterraCharacter"));
	return Combat->WeaponData->GripPoints.FindChecked(WeaponType);
}

bool APlayerCharacter::DoDamage_Implementation(float DamageAmount, AActor* DamageInstigator)
{
	if (!IsValid(HealthComponent)) return false;

	const bool bLethal = HealthComponent->ChangeHealthByAmount(-DamageAmount, DamageInstigator);
	const int32 MontageSelection = FMath::RandRange(0, HitReacts.Num() - 1);
	Multicast_HitReact(MontageSelection);
	return bLethal;
}


AWeapon* APlayerCharacter::GetCurrentWeapon_Implementation()
{
	if (!IsValid(Combat)) return nullptr;
	return Combat->CurrentWeapon;
}

void APlayerCharacter::Multicast_HitReact_Implementation(int32 MontageIndex)
{
	if (GetNetMode() != NM_DedicatedServer && !IsLocallyControlled())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(HitReacts[MontageIndex]);
	}
}

void APlayerCharacter::Input_BasicAttack_Pressed()
{
	if (!bEnableGameActions) return;
	Combat->Initiate_BasicAttack_Pressed();
}

void APlayerCharacter::Input_BasicAttack_Released()
{
	if (!bEnableGameActions) return;
	Combat->Initiate_BasicAttack_Released();
}

void APlayerCharacter::Initiate_Crouch_Implementation()
{
	GetCharacterMovement()->bWantsToCrouch = !GetCharacterMovement()->bWantsToCrouch;
}

void APlayerCharacter::Initiate_Jump_Implementation()
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

bool APlayerCharacter::IsDeadOrDying_Implementation()
{
	if (IsValid(HealthComponent))
	{
		return HealthComponent->IsDeadOrDying();
	}
	return true;
}

void APlayerCharacter::WeaponReplicated_Implementation()
{
	if (!bWeaponFirstReplicated)
	{
		bWeaponFirstReplicated = true;
		OnWeaponFirstReplicated.Broadcast(Combat->CurrentWeapon);
	}
}



void APlayerCharacter::EnableGameActions_Implementation(bool bEnable)
{
	bEnableGameActions = bEnable;
	if (!bEnable && IsValid(Combat))
	{
		Combat->Initiate_BasicAttack_Released();
	}
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

