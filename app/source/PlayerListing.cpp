#include "PlayerListing.h"
#include "Competition.h"
#include "PlayerExample.h"
#include "PlayerErick.h"

#include "PlayerFranL.h"
#include "PlayerFede.h"
#include "PlayerUnbeatable.h"
#include "PlayerTor.h"

namespace ttt
{

void setupPlayers(std::unique_ptr<Competition> &forCompetition)
{

	// Add new players below:

	forCompetition->addPlayer(std::make_unique<ttt::PlayerFranL>("[FRANLBOT] Frederik Franlebowitz"));
	forCompetition->addPlayer(std::make_unique<ttt::PlayerFede>("[BOT] Fede"));
    forCompetition->addPlayer(std::make_unique<ttt::PlayerErick>());
}

} // namespace
