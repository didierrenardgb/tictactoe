#include "PlayerFede.h"

#include "DummyBoard.h"

namespace ttt
{
Coordinates PlayerFede::play(Board const& board) const
{
	Coordinates coord = { constants::NOTACOORD,constants::NOTACOORD };

	if (board.valid())
	{
		//Initialize dummy board
		DummyBoard dummy (board, *this);
		int bestScore = std::numeric_limits<int>::min();

		//Check if FIRST can win in the next move
		coord = dummy.winningCoords(constants::PLAYERS::FIRST, board.winCondition());
		if (coord.x == constants::NOTACOORD && coord.y == constants::NOTACOORD)
		{
			//Check if SECOND can win in the next move
			coord = dummy.winningCoords(constants::PLAYERS::SECOND, board.winCondition());
		}

		if (coord.x == constants::NOTACOORD && coord.y == constants::NOTACOORD)
		{
			//Get Highest score from the dummy board
			for (int i = 0; i < board.width(); i++)
			{
				for (int j = 0; j < board.height(); j++)
				{
					if (dummy.at({ i,j }).mOwner == constants::PLAYERS::NONE && std::abs(dummy.at({ i,j }).mScore) > bestScore)
					{
						coord = { i,j };
						bestScore = dummy.at({ i,j }).mScore;
					}
				}
			}
			
		}
	}

	//if nothing else return origin
	if (coord.x == constants::NOTACOORD || coord.y == constants::NOTACOORD)
	{
		coord = { 0,0 };
	}
	return coord;
}
PlayerFede::PlayerFede(std::string const& name) :
	mName(name)
{
}
std::string const& PlayerFede::name() const
{
	return mName;
}


} // namespace ttt
 