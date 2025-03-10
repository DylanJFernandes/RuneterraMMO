#include "Player/PlayerInfo.h"
 
void FPlayerInfoArray::AddPlayer(const FPlayerInfo& NewPlayerInfo)
{
	int32 Index = Players.Add(NewPlayerInfo);
	MarkItemDirty(Players[Index]);
	Players[Index].PostReplicatedAdd(*this);
}
 
void FPlayerInfoArray::RemovePlayer(const FString& Username)
{
	for (int32 PlayerIndex = 0; PlayerIndex < Players.Num(); ++PlayerIndex)
	{
		FPlayerInfo& PlayerInfo = Players[PlayerIndex];
		if (PlayerInfo.Username == Username)
		{
			PlayerInfo.PreReplicatedRemove(*this);
			Players.RemoveAtSwap(PlayerIndex);
			MarkArrayDirty();
			break;
		}
	}
}