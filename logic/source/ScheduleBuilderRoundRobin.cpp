#include "ScheduleBuilderRoundRobin.h"

#include "Fixture.h"
#include "Match.h"
#include "Schedule.h"

namespace ttt
{

static void rotate(std::vector<std::reference_wrapper<std::unique_ptr<IPlayer> const>>& playersA, std::vector<std::reference_wrapper<std::unique_ptr<IPlayer> const>>& playersB)
{
    int offset = ((playersA.size() + playersB.size()) % 2 == 0)? 1 : 0;
    auto& playersADown = playersA.back().get();
    auto& playersBUp = playersB.front().get();

    for (int i = 1; i < playersB.size(); i++)
    {
        playersB[i - 1] = playersB[i];
    }
    playersB[playersB.size() - 1] = playersADown;

    for (int j = static_cast<int>(playersA.size()) - 1; j >= offset; j--)
    {
        playersA[j] = playersA[j - 1];
    }

    if (offset < playersA.size())
    {
        playersA[offset] = playersBUp;
    }
}

ScheduleBuilderRoundRobin::ScheduleBuilderRoundRobin(Board&& baseBoard):
    mBaseBoard(baseBoard)
{
}

std::unique_ptr<Schedule> ScheduleBuilderRoundRobin::build(std::vector<std::unique_ptr<IPlayer>> const& players) const
{
    std::list<Fixture> fixtures;
    std::vector<std::reference_wrapper<std::unique_ptr<IPlayer> const>> playerReferences;
    playerReferences.reserve(players.size());

    for (auto i = 0; i < players.size(); i++)
    {
        if (i < (players.size() - (((players.size() % 2) == 0) ? 1 : 0)))
        {
            fixtures.push_back(Fixture());
        }

        playerReferences.push_back(std::reference_wrapper<std::unique_ptr<IPlayer> const>(players[i]));
    }

    auto half = std::next(playerReferences.begin(), playerReferences.size() / 2 + playerReferences.size() % 2);
    auto rHalf = std::next(playerReferences.rbegin(), playerReferences.size() / 2);
    std::vector<std::reference_wrapper<std::unique_ptr<IPlayer> const>> playersA(playerReferences.begin(), half);
    std::vector<std::reference_wrapper<std::unique_ptr<IPlayer> const>> playersB(playerReferences.rbegin(), rHalf);

    int currentFixture = 0;
    for (Fixture& fixture : fixtures)
    {
        bool flipHomeAway = (currentFixture % 2) == 1;
        currentFixture++;
        for (int i = 0; i < std::min(playersA.size(), playersB.size()); i++)
        {
            auto& home = flipHomeAway? playersB : playersA;
            auto& away = flipHomeAway? playersA : playersB;
            fixture.addMatch(Match(Board(mBaseBoard), std::list<std::reference_wrapper<IPlayer const>>{ { *home[i].get() }, { *away[i].get() } }));
        }
        rotate(playersA, playersB);
    }

    auto schedule = std::make_unique<Schedule>();
    for (Fixture& fixture : fixtures)
    {
        schedule->addFixture(std::move(fixture));
    }
    return schedule;
}

} // namespace ttt
