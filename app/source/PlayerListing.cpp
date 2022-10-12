#include "PlayerListing.h"
#include "Competition.h"
#include "PlayerExample.h"

namespace ttt
{

void setupPlayers(std::unique_ptr<Competition>& forCompetition)
{
	forCompetition->addPlayer(std::make_unique<ttt::PlayerExample>("[BOT] Ludwig Van Example"));

	// Add new players below:

	forCompetition->addPlayer(std::make_unique<ttt::PlayerErick>); // Check this

	forCompetition->addPlayer(std::make_unique<ttt::PlayerExample>("[BOT] Johnny Silverhand"));

	forCompetition->addPlayer(std::make_unique<ttt::PlayerExample>("[BOT] Eri the Octopus"));
}

} // namespace
