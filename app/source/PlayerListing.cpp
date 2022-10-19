#include "PlayerListing.h"
#include "Competition.h"
#include "PlayerExample.h"
#include "PlayerErick.h"
#include "PlayerManual.h"

namespace ttt
{

void setupPlayers(std::unique_ptr<Competition>& forCompetition)
{
	forCompetition->addPlayer(std::make_unique<ttt::PlayerErick>());
	
	// Add new players below:
	forCompetition->addPlayer(std::make_unique<ttt::PlayerExample>("[BOT] Gyro Zeppeli"));

	forCompetition->addPlayer(std::make_unique<ttt::PlayerExample>("[BOT] Eri the Octopus"));
	
	forCompetition->addPlayer(std::make_unique<ttt::PlayerExample>("[BOT] Johnny Silverhand"));

	forCompetition->addPlayer(std::make_unique<ttt::PlayerExample>("[BOT] Rei Hino"));
	
	forCompetition->addPlayer(std::make_unique<ttt::PlayerExample>("[BOT] Maximo Cozetti"));

	/*
	forCompetition->addPlayer(std::make_unique<ttt::PlayerManual>("PlayerManual"));
	*/
}

} // namespace
