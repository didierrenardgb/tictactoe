/***************************************************************
*                          $$$$$$$                             *
*                      $$$$$$$$$$$$$$                          *
*                   $$$$$$$$$$$$$$$$$$                         *
*                  $$$$$$$$$$$$$$$$$$$$                        *
*                 $$$$$$$$$$$$$$$O$$$$$    $$$$$$              *
*                 $$$$$$$$O$$$$$$$$$$$$   $$$  $$$             *
*                 $$$$$$$$$$$$$$$$$$$$  $$$$    $$             *
*                  $$$$$$$$$$$$$$$$$$$$$$$$                    *
*                   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$             *
*           $$$$$$     $$$$$$$$$$$$$$$$$$        $$            *
*          $$   $$$$$$$$$$$$$$$$$$$$$$$         $$             *
*           $$$    $$$$$$$$$$$$$$$$$$$$$$$$$$$$                *
*               $$$$$$$$  $$$ $$$$$$          $$               *
*             $$$$       $$$  $$$ $$$      $$$$                *
*            $$$       $$$$   $$$  $$$                         *
*             $$$$$   $$$     $$$   $$$$    $$                 *
*              $$$    $$$$$$  $$$    $$$$$$$$                  *
*                      $$$     $$$$$   $$$$                    *
*                                                              *
*                      Erick Andersson                         *
*                erick.andersson98@gmail.com                   *
*                                                              *
****************************************************************/

#include "MiniMax.h"

#include "Board.h"
#include "Coordinates.h"
#include "Tile.h"

//Debugging
#include <iostream>

namespace ttt
{

//Tune this for changes on the algorithm
const int POINTSFORWIN = 10;
const int POINTSFORLOSE = -10;
const int MAXDEPTH = 4;

enum Points : int
{
	win = 50,
	lose = -50,
	preventVictory = 7,
	center = 11,
	nextTo = 1
};

enum PlayerID : int
{
	opponent = -1,
	empty,
	me
};

MiniMax::MiniMax(Board const& board, std::string const& player)
{
	mPlayer = player;
    mMaxC = board.width();
    mMaxR = board.height();
    mIsEmpty = board.empty();
	mWinCondition = board.winCondition();
    copyBoard(board);
}

void MiniMax::copyBoard(Board const& board)
{
	for(int r = 0; r < mMaxR; r++)
	{
        std::vector<int> row;
		for(int c = 0; c < mMaxC; c++)
		{
			//Copy everything into a vector of vector of int
			Coordinates coords;
			coords.x = c;
			coords.y = r;
			if(!board.tile(coords)->owner().has_value())
			{
				row.push_back(PlayerID::empty); //It's empty.
			}
			else if(board.tile(coords)->owner()->get().name() == mPlayer)
			{
				row.push_back(PlayerID::me); //It's mine.
			}
			else
			{
				row.push_back(PlayerID::opponent); //It's not mine.
			}
		}
        mBoard.push_back(row);
	}
}

void MiniMax::printBoard()
{
	std::cout << "\nMINIMAX CLASS \nMETHOD PRINT BOARD"<<std::endl;
	for(int i=0; i < mMaxR; i++)
	{
		for(int j=0; j < mMaxC; j++)
		{
			std::cout << mBoard[i][j] << " | ";
		}
		std::cout << std::endl;
	}
	std::cout << "----------" << std::endl;
}

int MiniMax::checkWincon(int row, int column, int rowIncrement, int colIncrement)
{
	//Set the variables
	int toReturn = 0;
	int currentPlayer = PlayerID::empty;
	int lastPlayer = PlayerID::empty;
	int i = row-mWinCondition+1;
	int j = column-mWinCondition+1;
	if(rowIncrement == 0)
	{
		i = row;
	}
	if(colIncrement == 0)
	{
		j = column;
	}

	int connected = 1;
	
	//Start roaming	
	for(int k = 0; k < 2*mWinCondition - 1; k++)
	{
		//Check if the coordinate is on limits.
		if((i>=0) && (j>=0))
		{
			if((i<mMaxR) && (j<mMaxC))
			{
				currentPlayer = mBoard[i][j];
				if((currentPlayer != PlayerID::empty) && (currentPlayer == lastPlayer))
				{
					connected++;
					//Check for mWinCondition amount of connections.
					if(connected == mWinCondition)
					{
						//Someone won! But who?
						toReturn = (currentPlayer == PlayerID::me) ? Points::win : Points::lose;
						break;
					}
				}
				else
				{
					connected = 1; //Reset
				}
				lastPlayer = mBoard[i][j];
			}
			else
			{
				break; //Out of bounds, prevent useless checks.
			}
		}
		i += rowIncrement; //Advancing in row
		j += colIncrement; //Advancing in column
	}

	return toReturn;
}

int MiniMax::evaluate(int row, int column)
{
	//First turn, always right at the middle
	if((mIsEmpty) && (row == mMaxR/2) && (column == mMaxC/2))
	{
		return Points::center; //Early return, preventing useless checks
	}

	int toReturn = 0;

	//Row
	toReturn = checkWincon(row, column, 0, 1);
	
	//Column
	if(toReturn == 0)
	{
		toReturn = checkWincon(row, column, 1, 0);
	}

	//Desc diagonal
	if(toReturn == 0)
	{
		toReturn = checkWincon(row, column, 1, 1);;
	}

	//Asc diagonal
	if(toReturn == 0)
	{
		toReturn = checkWincon(row, column, -1, 1);
	}

	//Additional points for placing the token next to another of the same type.
	if((toReturn != POINTSFORWIN) && (toReturn != POINTSFORLOSE))
	{
		for(int i=row-1; i <= row+1; i++)
		{
			for(int j=column-1; j <= column+1; j++)
			{
				if((i==row) && (j==column))
				{
					continue;
				}
				if((i>=0)&&(i<mMaxR)&&(j>=0)&&(j<mMaxC))
				{
					//Check limits
					if((mBoard[i][j]==1) && (mBoard[row][column] == 1))
					{
						toReturn+=Points::nextTo;
						break;
					}
					else if((mBoard[i][j]==-1) && (mBoard[row][column] == -1))
					{
						toReturn-=Points::nextTo;
						break;
					}
				}
			}
		}
	}

	return toReturn;
}

int MiniMax::searchPlay(int row, int column, int depth, bool isMaximizer)
{
    int worth;
	if(isMaximizer)
	{
		worth = evaluate(row, column) - depth; //This is the heuristic
	}
	else
	{
		worth = evaluate(row, column) + depth; //This is the heuristic
	}
	//Check for TERMINAL STATES.

	//If it plays on this position, it wins.
	//Or the maximum depth has been reached, so it's the end of the search on that branch.
	if((worth >= POINTSFORWIN) || (worth <= POINTSFORLOSE) || (depth >= MAXDEPTH))
	{ 
		return worth;
	}

	//Check for a full board and not wincon, that means a draw.
	bool full = true;
	for(int i = 0; i < mMaxR; i++)
	{
		for(int j = 0; j < mMaxR; j++)
		{
			if(mBoard[i][j] == PlayerID::empty)
			{
				full=false;
				break;
			}
		}
		if(!full)
		{
			break;
		}
	}
	if(full)
	{
		return 0;
	}

	//Keep looking because the current state of the game is not a leaf
	int toReturn = 0;
	if(isMaximizer)
	{
		//Start of oponent path.
		int worstUtility = std::numeric_limits<int>::max();
		for(int r = 0; r < mMaxR; r++)
		{
			for(int c = 0; c < mMaxC; c++)
			{
				//Check if the coordinate is occupied or not.
				if(mBoard[r][c] == PlayerID::empty)
				{
					//Play on that coordinate
					mBoard[r][c] = PlayerID::opponent;
					//Get the worth value
					worth = searchPlay(r, c, depth+1, false);
					if(worth < worstUtility)
					{
						worstUtility = worth;
					}
					mBoard[r][c] = PlayerID::empty; //Remove token
				}
			}
		}
		toReturn = worstUtility;
	}
	else
	{
		//Start of player path
		int bestUtility = std::numeric_limits<int>::min();
		for(int r = 0; r < mMaxR; r++)
		{
			for(int c = 0; c < mMaxC; c++)
			{
				//Check if the coordinate is occupied or not.
				if(mBoard[r][c] == PlayerID::empty)
				{
					//Play on that coordinate
					mBoard[r][c] = PlayerID::me;
					//Get the worth value
					worth = searchPlay(r, c, depth+1, true);
					if(worth > bestUtility)
					{
						bestUtility = worth; 
					}
					mBoard[r][c] = PlayerID::empty; //Remove token
				}
			}
		}
		toReturn = bestUtility;
	}
	
	return toReturn; 
}

Coordinates MiniMax::play()
{
    //Set utility variables.
	Coordinates bestCoordinate;
	int worth = 0;
	int bestUtility = std::numeric_limits<int>::min();
	int depth = 0;
	bestCoordinate.x = mMaxC/2;
	bestCoordinate.y = mMaxR/2;

	//The board here is a matrix where you can put a token anywhere that's not occupied.
	//Considering that, we need to roam for all the posible places to check their worth value, and select the best one.
	for(int r = 0; r < mMaxR; r++)
	{
		for(int c = 0; c < mMaxC; c++)
		{
			//Check if the coordinate is occupied or not.
			if(mBoard[r][c] == PlayerID::empty)
			{
				//Play on that coordinate
				mBoard[r][c] = PlayerID::me;
				//Get the worth value
				worth = searchPlay(r, c, depth, true);
				if(worth > bestUtility)
				{
					bestUtility = worth;
					bestCoordinate.x = c;
					bestCoordinate.y = r; 
				}
				mBoard[r][c] = PlayerID::empty; //Remove token
			}
		}
	}

	return bestCoordinate;
}

} // namespace ttt
