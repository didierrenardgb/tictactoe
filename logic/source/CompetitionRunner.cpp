#include "CompetitionRunner.h"

#include "Competition.h"
#include "Fixture.h"
#include "Match.h"
#include "MatchPlayer.h"
#include "Schedule.h"

namespace ttt
{

static constexpr char kAvailableSymbols[]{ 'X', 'O', 'Y', 'Z' };

static void assignPlayerSymbols(Match& forMatch)
{
    int nextSymbolIndex = 0;
    for (auto const& player : forMatch.players())
    {
        forMatch.setPlayerSymbol(player, kAvailableSymbols[nextSymbolIndex++]);
    }
}

CompetitionRunner::CompetitionRunner(Competition& competition, std::chrono::milliseconds&& updateInterval):
	mCompetition(competition)
    , mUpdateInterval(updateInterval)
{
    mCompetition.start();
}

void CompetitionRunner::update()
{
    for (auto& fixture : mCompetition.schedule()->fixtures())
    {
        for (auto& match : fixture.matches())
        {
            if (!match.started())
            {
                assignPlayerSymbols(match);
            }
            else if (!match.played() && shouldStep())
            {
                ttt::MatchPlayer().step(match);
                mLastStep = std::chrono::system_clock::now();
                return;
            }
        }
    }
}

bool CompetitionRunner::shouldStep() const
{
    return (std::chrono::system_clock::now() - mLastStep) > mUpdateInterval;
}

} // namespace ttt
