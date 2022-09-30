#include "ScheduleWindow.h"

#include <sstream>

#include "imgui.h"

#include "Configuration.h"
#include "Fixture.h"
#include "Match.h"
#include "MatchWidget.h"
#include "Schedule.h"

namespace ttt
{

ScheduleWindow::ScheduleWindow(Schedule const& schedule):
	mSchedule(schedule)
{
}

void ScheduleWindow::draw() const
{
    int currentFixture = 1;
    WindowGeometry mainGeometry = mainWindowGeometry();

    ImGui::SetNextWindowPos({ 0, static_cast<float>(mainGeometry.height / 2) }, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize({ static_cast<float>(mainGeometry.width / 2), static_cast<float>(mainGeometry.height / 2) }, ImGuiCond_FirstUseEver);
    ImGui::Begin("Schedule");
    std::string ongoingMatchTitle;

    for (auto& fixture : mSchedule.fixtures())
    {
        int currentMatch = 1;
        bool showFixtureMatches = ImGui::CollapsingHeader((std::string("Fixture ") + std::to_string(currentFixture)).c_str());
        for (auto& match : fixture.matches())
        {
            std::stringstream ss;

            if (!match.started() || match.board().empty())
            {
                ss << "Pending match (" << currentFixture << ", " << currentMatch << ")";
            }
            else if (match.started() && !match.played())
            {
                ss << "Current match (" << currentFixture << ", " << currentMatch << ")";
                ongoingMatchTitle = ss.str();
            }
            else
            {
                ss << "Completed match (" << currentFixture << ", " << currentMatch << ")";
            }

            if (showFixtureMatches)
            {
                ImGui::BeginGroup();
                ImGui::Text(ss.str().c_str());
                MatchWidget matchWidget(match);
                matchWidget.draw();
                ImGui::EndGroup();
            }

            currentMatch++;
        }

        currentFixture++;
    }
    ImGui::End();
}

} // namespace ttt
