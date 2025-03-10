#include "GamePlayTags/DedicatedServerTags.h"


namespace DedicatedServerTags
{
	namespace GameSessionsAPI
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ListFleets,"DedicatedServerTags.GameSessionsAPI.ListFleets", "List Fleets resource on the game API")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(FindOrCreateGameSession,"DedicatedServerTags.GameSessionsAPI.FindOrCreateGameSession", "Retrieves an ACTIVE game session, creating one if one doesn't exist on the GameSession API")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(CreatePlayerSession,"DedicatedServerTags.GameSessionsAPI.CreatePlayerSession", "Create a New Player Session on the GameSession API")
	}

	namespace PortalAPI
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(SignUp,"DedicatedServerTags.PortalAPI.SignUp", "Creates a new User in the Portal API")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ConfirmSignUp,"DedicatedServerTags.PortalAPI.ConfirmSignUp", "Confirms a User in the portal AP")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(SignIn,"DedicatedServerTags.PortalAPI.SignIn", "Retrieves access token,Id Token, and refresh Token in Portal API")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(SignOut,"DedicatedServerTags.PortalAPI.SignOut", "Global Sign out and invalidates the tokens of Portal API")
	}
	
	namespace PlayerStatsAPI
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(RecordPlayerStats,"DedicatedServerTags.PlayerStatsAPI.RecordPlayerStats", "Records player stats in the PlayStatsAPI for the Dynamo Database")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(RetrievePlayerStats,"DedicatedServerTags.PlayerStatsAPI.RetrievePlayerStats", "Retrieves the players stats in the PlayStatsAPI from the Dynamo Database")
	}
}

