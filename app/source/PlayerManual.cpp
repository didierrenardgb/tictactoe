#include "PlayerManual.h"

#include "Board.h"
#include "Coordinates.h"
#include "Tile.h"

#include <random>
#include <iostream>

namespace ttt
{

static Coordinates play(Board const& board)
{
	if (board.valid())
	{
        Coordinates coords;
		std::cout << "Enter coordinates (x,y), player manual:" << std::endl;
        std::cin >> coords.x;
        std::cin >> coords.y;
        return coords; 
	}

	return { 0, 0 };
}

PlayerManual::PlayerManual(std::string const& name):
	mName(name)
{
}

std::string const& PlayerManual::name() const
{
	return mName;
}

Coordinates PlayerManual::play(Board const& board) const
{
	return ttt::play(board);
}

} // namespace ttt
