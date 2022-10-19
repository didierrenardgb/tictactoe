#include "PlayerFede.h"

#include "Board.h"
#include "Coordinates.h"
#include "Tile.h"

namespace ttt
{

static void addAt(Spot** myMap, players owner, Coordinates const& coord)
{
	int value = 0;
	if (owner == PLAYER)
		value = 1;
	else if (owner == OTHER)
		value = -1;

	//Assing owner to spot
	myMap[coord.x][coord.y].setOwner(owner);
	
	//Add score to spot
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++) {
			if (coord.x + i < 3 && coord.x + i >= 0 && coord.y + j < 3 && coord.y + j >= 0)
				myMap[coord.x + i][coord.y].addScore(value); //For each neighbour Player +1 --- For each neighbour OTHER -1
		}
	}
}
static Spot** fillBoard(Board const& board, PlayerFede const& player)
{
	//Reserve Memory
	Spot** myMap = new Spot* [board.width()];
	for (int i = 0; i < board.width(); i++)
		myMap[i] = new Spot[board.height()];

	//Add owners to dummy
	for (int i = 0; i < board.width(); i++)
	{
		myMap[i] = new Spot[board.height()];
		for (int j{ 0 }; j < board.height(); j++)
		{
			if (board.tile({ i,j })->owner().has_value())
			{
				if (&board.tile({ i,j })->owner().value().get() == &player)
				{
					addAt(myMap, PLAYER, { i,j });
				}
				else
				{
					addAt(myMap, OTHER, { i,j });
				}
			}
			else
				addAt(myMap, NONE, { i,j });
		}
	}

	return myMap;
}
static void release(Spot** myBoard, int width)
{
	for (int i{ 0 }; i < width; i++)
		delete[]myBoard[i];
	delete myBoard;
}

static bool check(Spot** board, int x, int y, char who, int width = 3, int height = 3)
{
	if (x < width && x >= 0 && y < height && y >= 0)
		return board[x][y].owner() == who;
	else
		return false;
}
Coordinates winningCoords(Spot** myBoard, players who, int width = 3, int height = 3, int winCon = 3)
{
	//For each spot in dummy board, get if winnable
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (myBoard[x][y].owner() == NONE)
			{
				int vertical = 1;
				int horizontal = 1;
				int diagonal = 1;
				int diagonal2 = 1;
				for (int w = 1; w < winCon; w++)
				{
					if (check(myBoard, x + w, y, who, width, height))
					{
						horizontal++;
					}

					if (check(myBoard, x - w, y, who, width, height))
					{
						horizontal++;
					}

					if (check(myBoard, x, y + w, who, width, height))
					{
						vertical++;
					}

					if (check(myBoard, x, y - w, who, width, height))
					{
						vertical++;
					}

					if (check(myBoard, x + w, y + w, who, width, height))
					{
						diagonal++;
					}

					if (check(myBoard, x - w, y - w, who, width, height))
					{
						diagonal++;
					}

					if (check(myBoard, x - w, y + w, who, width, height))
					{
						diagonal2++;
					}

					if (check(myBoard, x + w, y - w, who, width, height))
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

	return { NOTACOORD,NOTACOORD };
}

static Coordinates play(Board const& board, PlayerFede const& player)
{
	Coordinates coord = { NOTACOORD,NOTACOORD };

	if (board.valid())
	{
		//Initialize dummy board
		Spot** myBoard = fillBoard(board, player);
		int bestScore = MAXIMUM;

		//Check if PLAYER can win in the next move
		coord = winningCoords(myBoard, PLAYER, board.width(), board.height(), board.winCondition());
		if (coord.x != NOTACOORD || coord.y != NOTACOORD)
		{
			//free memory before return
			release(myBoard, board.width());
			return coord;
		}

		//Check if OTHER can win in the next move
		coord = winningCoords(myBoard, OTHER, board.width(), board.height(), board.winCondition());
		if (coord.x != NOTACOORD || coord.y != NOTACOORD)
		{
			//free memory before return
			release(myBoard, board.width());
			return coord;
		}

		//Get Highest score from the dummy board
		for (int i = 0; i < board.width(); i++)
		{
			for (int j = 0; j < board.width(); j++)
			{
				if (myBoard[i][j].owner() == NONE && myBoard[i][j].score() < bestScore)
				{
					coord = { i,j };
					bestScore = myBoard[i][j].score();
				}
			}
		}

		//free memory before return
		release(myBoard, board.width());
	}

	//if nothing else return origin
	if (coord.x == NOTACOORD || coord.y == NOTACOORD)
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
Coordinates PlayerFede::play(Board const& board) const
{
	return ttt::play(board, *this);
}

char Spot::owner() const
{
	return mOwner;
}
int Spot::score() const
{
	return mScore;
}
int Spot::addScore(int amount)
{
	mScore += amount;
	return mScore;
}
void Spot::setOwner(char owner)
{
	mOwner = owner;
}
} // namespace ttt
