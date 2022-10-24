#include "PlayerListing.h"
#include "Competition.h"
#include "PlayerExample.h"
#include "PlayerExe.h"

namespace ttt
{

void setupPlayers(std::unique_ptr<Competition>& forCompetition)
{
	forCompetition->addPlayer(std::make_unique<ttt::PlayerExe>("[BOT] EXE_A"));
	forCompetition->addPlayer(std::make_unique<ttt::PlayerExample>("[BOT] RANDOM_A"));
	forCompetition->addPlayer(std::make_unique<ttt::PlayerExe>("[BOT] EXE_B"));
	forCompetition->addPlayer(std::make_unique<ttt::PlayerExample>("[BOT] RANDOM_B"));
	// Add new players below:
}

} // namespace
