#include "MatchPlayer.h"

#include "Coordinates.h"
#include "Board.h"
#include "Match.h"
#include "Tile.h"

#include <limits>
#include <map>

namespace ttt
{

static IPlayer const& findTurn(Match const& match)
{
	IPlayer const* nextTurn = &match.players().front().get();
	std::map<IPlayer const*, int> playCounts;
	for (auto const& player : match.players())
	{
		playCounts[&player.get()] = 0;
	}

	for (int x = 0; x < match.board().width(); x++)
	{
		for (int y = 0; y < match.board().height(); y++)
		{
			auto const& tile = match.board().tile({x, y});
			if (tile->owner().has_value())
			{
				playCounts[&tile->owner().value().get()]++;
			}
		}
	}

	int minCount = std::numeric_limits<int>::max();
	for (auto const& [player, playCount] : playCounts)
	{
		if (playCount < minCount)
		{
			nextTurn = player;
			minCount = playCount;
		}
	}

	return *nextTurn;
}

bool MatchPlayer::step(Match& match)
{
	if (!match.played())
	{
		IPlayer const& player = findTurn(match);
		auto play = player.play(match.board());
		return match.board().play(player, play);
	}

	return false;
}

} // namespace ttt
