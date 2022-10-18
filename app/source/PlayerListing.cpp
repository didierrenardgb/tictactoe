#include "PlayerListing.h"
#include "Competition.h"
#include "PlayerExample.h"
#include "PlayerErick.h"
#include "PlayerManual.h"
//#include ".\logic\include\PlayerErick.h"






namespace ttt
{

void setupPlayers(std::unique_ptr<Competition>& forCompetition)
{
	forCompetition->addPlayer(std::make_unique<ttt::PlayerErick>());
	
	// Add new players below:

	//forCompetition->addPlayer(std::make_unique<ttt::PlayerManual>("PlayerManual"));

	//forCompetition->addPlayer(std::make_unique<ttt::PlayerExample>("[BOT] Johnny Silverhand"));

	forCompetition->addPlayer(std::make_unique<ttt::PlayerExample>("[BOT] Eri the Octopus"));

	forCompetition->addPlayer(std::make_unique<ttt::PlayerExample>("[BOT] Rei Hino"));

	forCompetition->addPlayer(std::make_unique<ttt::PlayerExample>("[BOT] Maximo Cozetti"));

	
}

} // namespace
