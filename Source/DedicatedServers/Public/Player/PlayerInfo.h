#pragma once
 
#include "Net/Serialization/FastArraySerializer.h"
 
#include "PlayerInfo.generated.h"
 
USTRUCT(BlueprintType)
struct FPlayerInfo : public FFastArraySerializerItem
{
	GENERATED_BODY()
 
	FPlayerInfo() {}
	FPlayerInfo(const FString& Name) : Username(Name) {}
 
	UPROPERTY(BlueprintReadWrite)
	FString Username{};
};
 
USTRUCT()
struct FPlayerInfoArray : public FFastArraySerializer
{
	GENERATED_BODY()
 
	UPROPERTY()
	TArray<FPlayerInfo> Players;
 
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FPlayerInfo, FPlayerInfoArray>(Players, DeltaParams, *this);
	}
 
	void AddPlayer(const FPlayerInfo& NewPlayerInfo);
	void RemovePlayer(const FString& Username);
};
 
template<>
struct TStructOpsTypeTraits<FPlayerInfoArray> : public TStructOpsTypeTraitsBase2<FPlayerInfoArray>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};