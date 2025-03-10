#pragma once

#include "CharacterStats.generated.h"

UENUM(meta = (Bitflags))
enum class EClassType : uint16
{
	Tank = 0,
	Mage			= 1 << 0,		// 00000000 00000001
	Assassin		= 1 << 1,		// 00000000 00000010
};

ENUM_CLASS_FLAGS(EClassType)

UENUM(meta = (Bitflags))
enum class ERegion : uint16
{
	Piltover = 0,
	Zaun        = 1 << 0,    // 00000000 00000001
};

ENUM_CLASS_FLAGS(ERegion)

UENUM(meta = (Bitflags))
enum class ESex : uint8
{
	Male = 0,
	Female        = 1 << 0,    // 00000000 00000001
};

ENUM_CLASS_FLAGS(ESex)

UENUM(meta = (Bitflags))
enum class ERace : uint8
{
	Human = 0,
	Yordle			= 1 << 0,	// 00000001
	Vastaya			= 1 << 1,	// 00000010
	Troll			= 1 << 2,	// 00000100
	Minotaur        = 1 << 3    // 00001000
};

ENUM_CLASS_FLAGS(ERace)

USTRUCT()
struct FCharacterStats
{
	GENERATED_BODY()

	// Example constructor that takes the enum values but stores them as ints.
	FCharacterStats(const FString& InCharacterName,
					ERace         InRace,
					ERegion       InRegion,
					EClassType    InClassType,
					ESex          InGender,
					int32         InLevel,
					int32         InHealth,
					int32         InMana)
		: characterName(InCharacterName)
		, race( static_cast<int32>(InRace) )
		, region( static_cast<int32>(InRegion) )
		, classType( static_cast<int32>(InClassType) )
		, gender( static_cast<int32>(InGender) )
		, level(InLevel)
		, health(InHealth)
		, mana(InMana)
	{
	}

	// Default constructor
	FCharacterStats() {}
    
	// Start Permanent Stats
	
	UPROPERTY()
	FString characterName{};

	// Instead of storing enums, store it as an int
	// (i.e. the bitmask integer representing one or more bits in the enum).
	UPROPERTY()
	int32 race{};

	UPROPERTY()
	int32 region{};

	UPROPERTY()
	int32 classType{};

	UPROPERTY()
	int32 gender{};

	// End Permanent Stats
	
	// Start Active Stats
	
	UPROPERTY()
	int32 level{};

	UPROPERTY()
	int32 health{};

	UPROPERTY()
	int32 mana{};

	// End Active Stats
};

USTRUCT()
struct FPlayerStats
{
	GENERATED_BODY()

	UPROPERTY()
	int32 currentSelectedCharacterIndex{};
	
	UPROPERTY()
	FCharacterStats slotOne_CharacterStats{};

	UPROPERTY()
	FCharacterStats slotTwo_CharacterStats{};

	UPROPERTY()
	FCharacterStats slotThree_CharacterStats{};
	
	UPROPERTY()
	FCharacterStats slotFour_CharacterStats{};
	
};