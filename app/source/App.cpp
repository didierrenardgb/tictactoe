#include "App.h"

#include <stdio.h>

#include "Configuration.h"
#include "Competition.h"
#include "CompetitionRunner.h"
#include "MatchWindow.h"
#include "Schedule.h"
#include "ScheduleWindow.h"
#include "Standing.h"
#include "StandingsWindow.h"
#include "PlayerListing.h"

namespace ttt
{

App::App(std::chrono::milliseconds&& updateInterval, ImVec4&& clearColor):
mCompetition(std::make_unique<Competition>())
, mClearColor(std::forward<ImVec4>(clearColor))
, mWindow("TicTacToe", mainWindowGeometry())
{
	mCompetitionRunner = std::make_unique<CompetitionRunner>(*mCompetition, std::forward<std::chrono::milliseconds>(updateInterval));
}

App::~App() = default;

int App::run()
{
    if (!mWindow.start([this](ImVec4&) { draw(); }, [this]() { update(); }))
    {
        return 1;
    }

    setupPlayers(mCompetition);
    if (mCompetition->start())
    {
        mWindow.mainLoop(mClearColor);
    }
    else
    {
        printf("ERROR: Not enough players on the competition\n");
    }
    mWindow.end();

    return 0;
}

void App::update()
{
	mCompetitionRunner->update();
}

void App::draw()
{
    std::vector<std::unique_ptr<IDrawable>> drawables;
    drawables.push_back(std::make_unique<ScheduleWindow>(*mCompetition->schedule()));
    drawables.push_back(std::make_unique<MatchWindow>("Ongoing match", mCompetition->schedule()->ongoingMatch()));
    drawables.push_back(std::make_unique<StandingsWindow>(mCompetition->standings()));

    for (auto const& drawable : drawables)
    {
        drawable->draw();
    }
}

} // namespacet ttt
