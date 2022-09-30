#include "Competition.h"

#include "Board.h"
#include "Fixture.h"
#include "IPlayer.h"
#include "Match.h"
#include "Schedule.h"
#include "ScheduleBuilderRoundRobin.h"
#include "Standing.h"

#include <algorithm>
#include <map>

namespace ttt
{
Competition::Competition() = default;
Competition::~Competition() = default;

void Competition::addPlayer(std::unique_ptr<IPlayer>&& player)
{
	mPlayers.push_back(std::forward<std::unique_ptr<IPlayer>>(player));
}

std::unique_ptr<Schedule> const& Competition::start()
{
    if (mPlayers.size() >= 2)
    {
        mSchedule = ScheduleBuilderRoundRobin(Board()).build(mPlayers);
    }
	return mSchedule;
}

std::unique_ptr<Schedule> const& Competition::schedule() const
{
    return mSchedule;
}

bool Competition::complete() const
{
	return mSchedule->complete();
}

std::vector<Standing> Competition::standings() const
{
    std::map<std::string, Standing> standings;

    if (mSchedule)
    {
        for (auto const& player : mPlayers)
        {
            standings.insert({ player->name(), Standing(player) });
        }

        for (auto const& fixture : mSchedule->fixtures())
        {
            for (auto const& match : fixture.matches())
            {
                if (match.played())
                {
                    for (auto const& player : match.players())
                    {
                        standings[player.get().name()].processMatch(match);
                    }
                }
            }
        }
    }

    std::vector<Standing> sorted;
    for (auto& standing : standings)
    {
        sorted.push_back(std::move(standing.second));
    }
    std::sort(sorted.begin(), sorted.end());
    return sorted;
}

} // namespace ttt
