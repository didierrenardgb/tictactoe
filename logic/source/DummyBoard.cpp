#include "DummyBoard.h"

#include "Tile.h"
#include "Coordinates.h"
#include "IPlayer.h"
#include "Board.h"

#include <vector>

namespace ttt
{

/*
 * Given coords and dimentions,
 * returns if spot at coords is valid
 */
bool DummyBoard::checkValid(int x, int y)
{
	return (x < m_width && x >= 0 && y < m_height && y >= 0);
}

/*
 * Given coords, dimentions and an owner,
 * returns if owner owns the Spot at coords in the dummyboard
 * if spot invalid also returns false
 */
bool DummyBoard::check(int x, int y, constants::PLAYERS who)
{
	return (this->checkValid(x, y) && m_board[x][y].mOwner == who);
}

/*
 * Given an owner and a coord, assings owner to Spot at coord in board
 * then updates the score of neighbouring Spots
 */
void DummyBoard::addAt(constants::PLAYERS owner, Coordinates const& coord)
{
	int value = (owner == constants::PLAYERS::FIRST) ? 1 : -1;

	//Assing owner to spot
	m_board[coord.x][coord.y].mOwner = owner;

	if (owner != constants::PLAYERS::NONE)
	{
		//Add score to neighbours
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				if (this->checkValid(coord.x + i, coord.y + j))
				{
					//if neighbour and current share owner, set additional scores
					if (owner == m_board[coord.x + i][coord.y + j].mOwner)
					{
						if (this->checkValid(coord.x - i, coord.y - j))
						{
							m_board[coord.x - i][coord.y - j].mScore += value; //3 in line assing simple points again
						}

						if (this->checkValid(coord.x + i * 2, coord.y + j * 2))
						{
							m_board[coord.x + 2 * i][coord.y + 2 * j].mScore += 2 * value; //3 in line assing double points
						}
					}
					m_board[coord.x + i][coord.y + j].mScore += value; //For each neighbour FIRST +1 --- For each neighbour SECOND -1
				}
			}
		}
	}
}

/*
 * Given a player and dimentions,
 * if winning coords exists it returns them else
 * if they don't exist it returns invalid coords
 */
Coordinates DummyBoard::winningCoords(constants::PLAYERS who, int winCon = 3)
{
	//For each spot in dummy board, get if winnable
	for (int x = 0; x < m_width; x++)
	{
		for (int y = 0; y < m_height; y++)
		{
			if (m_board[x][y].mOwner == constants::PLAYERS::NONE)
			{
				int vertical = 1;
				int horizontal = 1;
				int diagonal = 1;
				int diagonal2 = 1;
				for (int w = 1; w < winCon; w++)
				{
					if (this->check(x + w, y, who))
					{
						horizontal++;
					}

					if (this->check(x - w, y, who))
					{
						horizontal++;
					}

					if (this->check(x, y + w, who))
					{
						vertical++;
					}

					if (this->check (x, y - w, who))
					{
						vertical++;
					}

					if (this->check(x + w, y + w, who))
					{
						diagonal++;
					}

					if (this->check(x - w, y - w, who))
					{
						diagonal++;
					}

					if (this->check(x - w, y + w, who))
					{
						diagonal2++;
					}

					if (this->check(x + w, y - w, who))
					{
						diagonal2++;
					}
				}

				int wc = winCon;
				if ((vertical == wc) || (horizontal == wc) || (diagonal == wc) || (diagonal2 == wc))
				{
					return { x , y };
				}
				
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
DummyBoard::DummyBoard(Board const& board, IPlayer const& player)
	: m_width(board.width()), m_height(board.height())
{
	m_board = SpotMat (m_width, std::vector<Spot>(m_height));

	//Add owners to dummy
	for (int i = 0; i < board.width(); i++)
	{
		for (int j{ 0 }; j < board.height(); j++)
		{
			if (board.tile({ i,j })->owner().has_value())
			{
				if (&board.tile({ i,j })->owner().value().get() == &player)
				{
					this->addAt(constants::PLAYERS::FIRST, { i,j });
				}
				else
				{
					this->addAt(constants::PLAYERS::SECOND, { i,j });
				}
			}
			else
				this->addAt(constants::PLAYERS::NONE, { i,j });
		}
	}
}

Spot& DummyBoard::at(Coordinates const& coord)
{
	return m_board[coord.x][coord.y];
}

} // namespace ttt
 