#include "PlayerFede.h"

#include "Board.h"
#include "Coordinates.h"
#include "Tile.h"
#include <iostream>

namespace ttt
{
namespace constants
{
const int NOTACOORD = -1;
enum class PLAYERS : char
{
	FIRST = 'X',
	SECOND = 'O',
	NONE = ' '
};
}

struct Spot
{
	int mScore{ 0 };
	constants::PLAYERS mOwner = constants::PLAYERS::NONE;
};

using SpotMat = std::vector<std::vector<Spot>>;

/*
 * Given a dummyboard, coords and dimentions,
 * returns if spot at coords is valid
 */
static bool checkValid(SpotMat& board, int x, int y)
{
	return (x < board.size() && x >= 0 && y < board[0].size() && y >= 0);
}

/*
 * Given a dummyboard, coords, dimentions and an owner,
 * returns if owner owns the Spot at coords in the dummyboard
 * if spot invalid also returns false
 */
static bool check(SpotMat& board, int x, int y, constants::PLAYERS who)
{
	return (checkValid(board, x, y) && board[x][y].mOwner == who);
}

/*
 * Given a board an owner and a coord, assings owner to Spot at coord in board
 * then updates the score of neighbouring Spots
 */
static void addAt(SpotMat& myBoard, constants::PLAYERS owner, Coordinates const& coord)
{
	int value = owner == constants::PLAYERS::FIRST ? 1 : -1;

	//Assing owner to spot
	myBoard[coord.x][coord.y].mOwner = owner;

	if (owner != constants::PLAYERS::NONE)
	{
		//Add score to neighbours
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				if (checkValid(myBoard, coord.x + i, coord.y + j))
				{
					//if neighbour and current share owner, set additional scores
					if (owner == myBoard[coord.x + i][coord.y + j].mOwner)
					{
						if (checkValid(myBoard, coord.x - i, coord.y - j))
							myBoard[coord.x - i][coord.y - j].mScore += value; //3 in line assing simple points again

						if (checkValid(myBoard, coord.x + i * 2, coord.y + j * 2))
							myBoard[coord.x + 2 * i][coord.y + 2 * j].mScore += 2 * value; //3 in line assing double points
					}
					myBoard[coord.x + i][coord.y + j].mScore += value; //For each neighbour FIRST +1 --- For each neighbour SECOND -1
				}
			}
		}
	}
}

/*
 * Given a board a player and dimentions,
 * returns if they exists the winning movement coords.
 * if they don't exist it returns invalid coords
 */
Coordinates winningCoords(SpotMat& myBoard, constants::PLAYERS who, int winCon = 3)
{
	//For each spot in dummy board, get if winnable
	for (int x = 0; x < myBoard.size(); x++)
	{
		for (int y = 0; y < myBoard[x].size(); y++)
		{
			if (myBoard[x][y].mOwner == constants::PLAYERS::NONE)
			{
				int vertical = 1;
				int horizontal = 1;
				int diagonal = 1;
				int diagonal2 = 1;
				for (int w = 1; w < winCon; w++)
				{
					if (check(myBoard, x + w, y, who))
					{
						horizontal++;
					}

					if (check(myBoard, x - w, y, who))
					{
						horizontal++;
					}

					if (check(myBoard, x, y + w, who))
					{
						vertical++;
					}

					if (check(myBoard, x, y - w, who))
					{
						vertical++;
					}

					if (check(myBoard, x + w, y + w, who))
					{
						diagonal++;
					}

					if (check(myBoard, x - w, y - w, who))
					{
						diagonal++;
					}

					if (check(myBoard, x - w, y + w, who))
					{
						diagonal2++;
					}

					if (check(myBoard, x + w, y - w, who))
					{
						diagonal2++;
					}
				}

				int wc = winCon;
				if ((vertical == wc) || (horizontal == wc) || (diagonal == wc) || (diagonal2 == wc))
					return { x , y };
				
			}
		}

	}

	return { constants::NOTACOORD, constants::NOTACOORD };
}

/*
 * Given a board and a player, creates a dummyboard
 * assigning FIRST value to FIRST owned spots, SECOND value
 * to non FIRST owned spots and NONE value to ownerless Spots
 */
static SpotMat fillBoard(Board const& board, PlayerFede const& player, SpotMat& myBoard)
{
	//Add owners to dummy
	for (int i = 0; i < board.width(); i++)
	{
		for (int j{ 0 }; j < board.height(); j++)
		{
			if (board.tile({ i,j })->owner().has_value())
			{
				if (&board.tile({ i,j })->owner().value().get() == &player)
				{
					addAt(myBoard, constants::PLAYERS::FIRST, { i,j });
				}
				else
				{
					addAt(myBoard, constants::PLAYERS::SECOND, { i,j });
				}
			}
			else
				addAt(myBoard, constants::PLAYERS::NONE, { i,j });
		}
	}

	return myBoard;
}

Coordinates PlayerFede::play(Board const& board) const
{
	Coordinates coord = { constants::NOTACOORD,constants::NOTACOORD };

	if (board.valid())
	{
		//Initialize dummy board
		SpotMat myBoard(board.width(), std::vector<Spot>(board.height()));
		fillBoard(board, *this, myBoard);
		int bestScore = std::numeric_limits<int>::min();

		//Check if FIRST can win in the next move
		coord = winningCoords(myBoard, constants::PLAYERS::FIRST, board.winCondition());
		if (coord.x == constants::NOTACOORD && coord.y == constants::NOTACOORD)
		{
			//Check if SECOND can win in the next move
			coord = winningCoords(myBoard, constants::PLAYERS::SECOND, board.winCondition());
		}

		if (coord.x == constants::NOTACOORD && coord.y == constants::NOTACOORD)
		{
			//Get Highest score from the dummy board
			for (int i = 0; i < board.width(); i++)
			{
				for (int j = 0; j < board.width(); j++)
				{
					if (myBoard[i][j].mOwner == constants::PLAYERS::NONE && abs(myBoard[i][j].mScore) > bestScore)
					{
						coord = { i,j };
						bestScore = myBoard[i][j].mScore;
					}
				}
			}
			
		}
	}

	//if nothing else return origin
	if (coord.x == constants::NOTACOORD || coord.y == constants::NOTACOORD)
		coord = { 0,0 };
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
