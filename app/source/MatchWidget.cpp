#include "MatchWidget.h"

#include <sstream>

#include "imgui.h"

#include "Coordinates.h"
#include "IPlayer.h"
#include "Match.h"
#include "Tile.h"

namespace ttt
{

static char symbolForTile(ttt::Match const& match, ttt::Tile const& tile)
{
    if (tile.owner().has_value())
    {
        return match.playerSymbol(tile.owner().value().get());
    }

    return ' ';
}

static std::string buildVersusString(ttt::Match const& match)
{
    std::stringstream ss;
    for (auto const& player : match.players())
    {
        ss << player.get().name() << " (" << match.playerSymbol(player) << ")";
        if (&player.get() != &match.players().back().get())
        {
            ss << " vs ";
        }
    }
    return ss.str();
}

MatchWidget::MatchWidget(Match const& match):
	mMatch(match)
{
}

void MatchWidget::draw() const
{
    ImGui::Text(buildVersusString(mMatch).c_str());
    static char const* kRowSeparator = "-------";
    ImGui::Text(kRowSeparator);
    for (int y = 0; y < mMatch.board().height(); y++)
    {
        std::stringstream ss;
        ss << '|';
        for (int x = 0; x < mMatch.board().width(); x++)
        {
            auto const& tile = mMatch.board().tile({ x, y });
            if (tile->owner().has_value())
            {
                ss << symbolForTile(mMatch, *tile) << '|';
            }
            else
            {
                ss << " |";
            }
        }
        ImGui::Text(ss.str().c_str());
        ImGui::Text(kRowSeparator);
    }
}

} // namespace
