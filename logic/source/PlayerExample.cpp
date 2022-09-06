#include "PlayerExample.h"

#include "Board.h"
#include "Coordinates.h"
#include "Tile.h"

#include <random>

namespace ttt
{

static Coordinates randomCoordinate(Board const& board)
{
	return { std::rand() % board.width(), std::rand() % board.height() };
}

static Coordinates play(Board const& board)
{
	if (board.valid())
	{
		Coordinates coords = randomCoordinate(board);
		while ((board.tile(coords) == nullptr) || board.tile(coords)->owner().has_value())
		{
			coords = randomCoordinate(board);
		}
		return coords;
	}

	return { 0, 0 };
}

PlayerExample::PlayerExample(std::string const& name):
	mName(name)
{
}

std::string const& PlayerExample::name() const
{
	return mName;
}

Coordinates PlayerExample::play(Board const& board) const
{
	return ttt::play(board);
}

} // namespace ttt
