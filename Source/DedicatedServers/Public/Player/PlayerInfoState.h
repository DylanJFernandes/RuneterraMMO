#pragma once
 
#include "CoreMinimal.h"
#include "PlayerInfo.h"
#include "GameFramework/Info.h"
#include "PlayerInfoState.generated.h"
 
USTRUCT()
struct FPlayerInfoStateDelta
{
	GENERATED_BODY()
 
	UPROPERTY()
	TArray<FPlayerInfo> AddedPlayers{};
 
	UPROPERTY()
	TArray<FPlayerInfo> RemovedPlayers{};
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerInfoChanged, const FPlayerInfo&, PlayerInfo);
 
UCLASS()
class DEDICATEDSERVERS_API APlayerInfoState : public AInfo
{
	GENERATED_BODY()
 
public:
	APlayerInfoState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
 
	UPROPERTY(BlueprintAssignable)
	FOnPlayerInfoChanged OnPlayerInfoAdded;
 
	UPROPERTY(BlueprintAssignable)
	FOnPlayerInfoChanged OnPlayerInfoRemoved;
 
	void AddPlayerInfo(const FPlayerInfo& PlayerInfo);
	void RemovePlayerInfo(const FString& Username);

	TArray<FPlayerInfo> GetPlayerInfos() const;
protected:
 
	UFUNCTION()
	void OnRep_PlayerInfoPlayerInfo();
 	
private:
 
	UPROPERTY(ReplicatedUsing=OnRep_PlayerInfoPlayerInfo)
	FPlayerInfoArray PlayerInfoArray;
	
	UPROPERTY()
	FPlayerInfoArray LastPlayerInfoArray;
 
	FPlayerInfoStateDelta ComputePlayerInfoDelta(const TArray<FPlayerInfo>& OldArray, const TArray<FPlayerInfo>& NewArray);
};