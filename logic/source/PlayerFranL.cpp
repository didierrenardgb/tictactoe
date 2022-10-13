#include "PlayerFranL.h"

#include "Board.h"
#include "Coordinates.h"
#include "Tile.h"

#include "PlayerExample.h"

#include <random>

namespace ttt
{




	PlayerFranL::PlayerFranL(std::string const& name, int depth) :
		mName(name), mDepth(depth)
	{
	}

	std::string const& PlayerFranL::name() const
	{
		return mName;
	}


	Coordinates PlayerFranL::finish_off(Board const& board) const{
		for (int i = 0; i < board.width(); i++)
		{
			for (int j = 0; j < board.height(); j++)
			{
				Board testboard = board;
				return { 0,0 };
			}
		}
	}

	Coordinates PlayerFranL::defend(Board const& board) const{
		return { 0,0 };
	}

	Coordinates PlayerFranL::fork(Board const& board) const{
		for (int i = 0; i < board.width(); i++)
		{
			for (int j = 0; j < board.height(); j++)
			{
				return { 0,0 };
			}
		}
	}

	Coordinates PlayerFranL::de_fork(Board const& board) const{
		for (int i = 0; i < board.width(); i++)
		{
			for (int j = 0; j < board.height(); j++)
			{
				return { 0,0 };
			}
		}
	}

	PlayerFranL::next_move PlayerFranL::strategize(int depth, Board const& board) const {
		return { -1,{0,0} };
	}

	Coordinates PlayerFranL::play(Board const& board) const
	{
		if (board.valid())
		{
			PlayerExample player("test");
			return player.play(board);

			Coordinates coords = PlayerFranL::strategize(mDepth,board).coords;
			return coords;
		}

		return { 0, 0 };
	}

} // namespace ttt
