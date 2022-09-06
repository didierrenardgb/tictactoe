#include "App.h"

#include "Competition.h"
#include "CompetitionRunner.h"
#include "MatchWindow.h"
#include "PlayerExample.h"
#include "Schedule.h"
#include "ScheduleWindow.h"
#include "Standing.h"
#include "StandingsWindow.h"

namespace ttt
{

App::App(std::chrono::milliseconds&& updateInterval, ImVec4&& clearColor):
mCompetition(std::make_unique<Competition>())
, mClearColor(std::forward<ImVec4>(clearColor))
, mWindow("TicTacToe")
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

    mCompetition->addPlayer(std::make_unique<ttt::PlayerExample>("Player1"));
    mCompetition->addPlayer(std::make_unique<ttt::PlayerExample>("Player2"));
    mCompetition->addPlayer(std::make_unique<ttt::PlayerExample>("Player3"));
    mCompetition->addPlayer(std::make_unique<ttt::PlayerExample>("Player4"));
    mCompetition->start();

    mWindow.mainLoop(mClearColor);
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
