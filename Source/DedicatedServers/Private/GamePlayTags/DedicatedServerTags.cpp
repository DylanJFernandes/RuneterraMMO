#include "GamePlayTags/DedicatedServerTags.h"


namespace DedicatedServerTags
{
	namespace GameSessionsAPI
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ListFleets,"DedicatedServerTags.GameSessionsAPI.ListFleets", "List Fleets resource on the game API")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(FindOrCreateGameSession,"DedicatedServerTags.GameSessionsAPI.FindOrCreateGameSession", "Retrieves an ACTIVE game session, creating one if one doesn't exist on the GameSession API")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(CreatePlayerSession,"DedicatedServerTags.GameSessionsAPI.CreatePlayerSession", "Create a New Player Session on the GameSession API")
	}
}

