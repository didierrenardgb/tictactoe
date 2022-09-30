#pragma once

#include <memory>

namespace ttt
{
class Competition;

void setupPlayers(std::unique_ptr<Competition>& forCompetition);

} // namespace ttt
