#include "StandingsWindow.h"

#include "imgui.h"

#include "Configuration.h"
#include "Standing.h"
#include "IPlayer.h"

namespace ttt
{

static char const* const kStandingsHeaderLabels[]{ "Pos", "Name", "P", "Pld", "W", "D", "L" };
static char const* const kStandingNumberFormat = "%d";

StandingsWindow::StandingsWindow(std::vector<ttt::Standing> const& standings):
    mStandings(standings)
{
}

void StandingsWindow::draw() const
{
    WindowGeometry mainGeometry = mainWindowGeometry();

    ImGui::SetNextWindowPos({ static_cast<float>(mainGeometry.width / 2), static_cast<float>(mainGeometry.height / 2) }, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize({ static_cast<float>(mainGeometry.width / 2), static_cast<float>(mainGeometry.height / 2) }, ImGuiCond_FirstUseEver);
    ImGui::Begin("Standings");
    if (ImGui::BeginTable("StandingsTable", 7))
    {
        auto const columnCount = sizeof(kStandingsHeaderLabels) / sizeof(char const*);

        ImGui::TableHeadersRow();
        for (int i = 0; i < columnCount; i++)
        {
            ImGui::TableSetColumnIndex(i);
            ImGui::Text(kStandingsHeaderLabels[i]);
        }

        int p = 1;
        for (auto const& standing : mStandings)
        {
            ImGui::TableNextRow();

            std::string cellValues[] {
                std::to_string(p),
                standing.player()->name(),
                std::to_string(standing.points()),
                std::to_string(standing.played()),
                std::to_string(standing.won()),
                std::to_string(standing.drawn()),
                std::to_string(standing.lost())
            };

            for (int i = 0; i < columnCount; i++)
            {
                ImGui::TableSetColumnIndex(i);
                ImGui::Text(cellValues[i].c_str());
            }

            p++;
        }
        ImGui::EndTable();
    }
    ImGui::End();
}

} // namespace ttt
