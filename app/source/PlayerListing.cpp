#include "PlayerListing.h"
#include "Competition.h"
#include "PlayerExample.h"
#include "PlayerFranL.h"

namespace ttt
{

void setupPlayers(std::unique_ptr<Competition>& forCompetition)
{
	forCompetition->addPlayer(std::make_unique<ttt::PlayerExample>("[BOT] Ludwig Van Example"));

	// Add new players below:

	forCompetition->addPlayer(std::make_unique<ttt::PlayerFranL>("[BOT] Frederik FranLobwiz"));
}

} // namespace
