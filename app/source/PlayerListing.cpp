#include "PlayerListing.h"
#include "Competition.h"
#include "PlayerExample.h"
#include "PlayerErick.h"

namespace ttt
{

void setupPlayers(std::unique_ptr<Competition> &forCompetition)
{

	// Add new players below:

	forCompetition->addPlayer(std::make_unique<ttt::PlayerErick>());
}

} // namespace
