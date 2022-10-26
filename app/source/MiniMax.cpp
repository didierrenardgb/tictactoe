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
const int pointsForWin = 5;
const int maxDepth = 4;

enum Points : int
{
	win = 10,
	lose = -10,
	preventVictory = 8,
	center = 7,
	nextTo = 1
};

MiniMax::MiniMax(Board const& board, std::string player)
{
	mPlayer = player;
    mMaxC = board.width();
    mMaxR = board.height();
    mIsEmpty = board.empty();
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
				row.push_back(0); //It's empty.
			}
			else if(board.tile(coords)->owner()->get().name() == mPlayer)
			{
				row.push_back(1); //It's mine.
			}
			else
			{
				row.push_back(-1); //It's not mine.
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

const int MiniMax::evaluate(int const& row, int const& column)
{
	/*
	//Center
	return 7;
	//Next to one of yours
	return 1;
	//Wincon
	return 10;
	//Draw
	return 0;
	*/

	//First turn, always right at the middle
	if((mIsEmpty) && (row == mMaxR/2) && (column == mMaxC/2))
	{
		return Points::center; //Early return, preventing useless checks
	}

	int toReturn = 0;
	//Check if you're preventing the oponent's victory (in between the checks for wincon).

	//Check wincon for row.
	if((column == 0) && (mBoard[row][column+1] != 0))
	{ //Leftmost col
		//Only check two to the right
		if((mBoard[row][column+1] == mBoard[row][column+2]))
		{
			//Check who won
			if((mBoard[row][column] == 1) && (mBoard[row][column] == mBoard[row][column+1]))
			{
				toReturn = Points::win; //It was me
			}
			else if((mBoard[row][column] == -1) && (mBoard[row][column] == mBoard[row][column+1]))
			{
				toReturn = Points::lose; //It was the oponent
			}
			else
			{
				//Prevented the oponent's victory
				toReturn = (mBoard[row][column] == 1) ? Points::preventVictory : -Points::preventVictory;
			}
		}
	}
	else if((column == mMaxC-1) && (mBoard[row][column-1] != 0))
	{ //Rightmost col
		//Only check two to the left
		if((mBoard[row][column-1] == mBoard[row][column-2]))
		{
			//Check who won
			if((mBoard[row][column] == 1)&&(mBoard[row][column] == mBoard[row][column-1]))
			{
				toReturn = Points::win; //It was me
			}
			else if((mBoard[row][column] == -1)&&(mBoard[row][column] == mBoard[row][column-1]))
			{
				toReturn = Points::lose; //It was the oponent
			}
			else
			{
				//Prevented the oponent's victory
				toReturn = (mBoard[row][column] == 1) ? Points::preventVictory : -Points::preventVictory;
			}
		}
	}
	else if((column != 0) && (column != mMaxC-1))
	{ //Both sides now
		//One left-One right, Two left, Two right
		if((mBoard[row][column-1] != 0) && (mBoard[row][column-1] == mBoard[row][column+1]))
		{
			//Check who won
			if((mBoard[row][column] == 1)&&(mBoard[row][column] == mBoard[row][column+1]))
			{
				toReturn = Points::win; //It was me
			}
			else if((mBoard[row][column] == -1)&&(mBoard[row][column] == mBoard[row][column+1]))
			{
				toReturn = Points::lose; //It was the oponent
			}
			else
			{
				//Prevented the oponent's victory
				toReturn = (mBoard[row][column] == 1) ? Points::preventVictory : -Points::preventVictory;
			}
		}

		if((column-2 >= 0) && (mBoard[row][column-1] != 0) && (mBoard[row][column-1] == mBoard[row][column-2]))
		{
			//Check who won
			if((mBoard[row][column] == mBoard[row][column-1]) && (mBoard[row][column] == 1))
			{
				toReturn = Points::win; //It was me
			}
			else if((mBoard[row][column] == mBoard[row][column-1]) && (mBoard[row][column] == -1))
			{
				toReturn = Points::lose; //It was the oponent
			}
			else
			{
				//Prevented the oponent's victory
				toReturn = (mBoard[row][column] == 1) ? Points::preventVictory : -Points::preventVictory;
			}
		}

		if((column+2 < mMaxC)&&(mBoard[row][column+1] != 0)&&(mBoard[row][column+1] == mBoard[row][column+2]))
		{
			//Check who won
			if((mBoard[row][column] == mBoard[row][column+1]) && (mBoard[row][column] == 1))
			{
				toReturn = Points::win; //It was me
			}
			else if((mBoard[row][column] == mBoard[row][column+1]) && (mBoard[row][column] == -1))
			{
				toReturn = Points::lose; //It was the oponent
			}
			else
			{
				//Prevented the oponent's victory
				toReturn = (mBoard[row][column] == 1) ? Points::preventVictory : -Points::preventVictory;
			}
		}
	}

	//Check wincon for column.
	if((row == 0) && (mBoard[row+1][column] != 0))
	{ //Upmost row
		//Only check two to the bottom
		if(mBoard[row+1][column] == mBoard[row+2][column])
		{
			//Check who won
			if((mBoard[row][column] == 1)&&(mBoard[row][column] == mBoard[row+1][column]))
			{
				toReturn = Points::win; //It was me
			}
			else if((mBoard[row][column] == -1)&&(mBoard[row][column] == mBoard[row+1][column]))
			{
				toReturn = Points::lose; //It was the oponent
			}
			else
			{
				//Prevented the oponent's victory
				toReturn = (mBoard[row][column] == 1) ? Points::preventVictory : -Points::preventVictory;
			}
		}
	}
	else if((row == mMaxR-1) && (mBoard[row-1][column] != 0))
	{ //Downmost row
		//Only check two to the top
		if(mBoard[row-1][column] == mBoard[row-2][column])
		{
			//Check who won
			if((mBoard[row][column] == 1)&&(mBoard[row][column] == mBoard[row-1][column]))
			{
				toReturn = Points::win; //It was me
			}
			else if((mBoard[row][column] == -1)&&(mBoard[row][column] == mBoard[row-1][column]))
			{
				toReturn = Points::lose; //It was the oponent
			}
			else
			{
				//Prevented the oponent's victory
				toReturn = (mBoard[row][column] == 1) ? Points::preventVictory : -Points::preventVictory;
			}
		}
	}
	else if((row != 0) && (row != mMaxR-1))
	{ //Both sides now
		//One up-One down, Two up, Two down
		if((mBoard[row+1][column] != 0) && (mBoard[row+1][column] == mBoard[row-1][column]))
		{
			//Check who won
			if((mBoard[row][column] == 1)&&(mBoard[row][column] == mBoard[row+1][column]))
			{
				toReturn = Points::win; //It was me
			}
			else if((mBoard[row][column] == -1)&&(mBoard[row][column] == mBoard[row+1][column]))
			{
				toReturn = Points::lose; //It was the oponent
			}
			else
			{
				//Prevented the oponent's victory
				toReturn = (mBoard[row][column] == 1) ? Points::preventVictory : -Points::preventVictory;				
			}
		}

		if((row-2 >= 0) && (mBoard[row-1][column] != 0) && (mBoard[row-1][column] == mBoard[row-2][column]))
		{
			//Check who won
			if((mBoard[row][column] == 1)&&(mBoard[row][column] == mBoard[row-1][column]))
			{
				toReturn = Points::win; //It was me
			}
			else if((mBoard[row][column] == -1)&&(mBoard[row][column] == mBoard[row-1][column]))
			{
				toReturn = Points::lose; //It was the oponent
			}
			else
			{
				//Prevented the oponent's victory
				toReturn = (mBoard[row][column] == 1) ? Points::preventVictory : -Points::preventVictory;
			}
		}

		if((row+2 < mMaxR) && (mBoard[row+1][column] != 0) && (mBoard[row+1][column] == mBoard[row+2][column]))
		{
			//Check who won
			if((mBoard[row][column] == 1)&&(mBoard[row][column] == mBoard[row+1][column]))
			{
				toReturn = Points::win; //It was me
			}
			else if((mBoard[row][column] == -1)&&(mBoard[row][column] == mBoard[row+1][column]))
			{
				toReturn = Points::lose; //It was the oponent
			}
			else
			{
				//Prevented the oponent's victory
				toReturn = (mBoard[row][column] == 1) ? Points::preventVictory : -Points::preventVictory;
			}
		}
	}

	//Diagonals.
	//Two down right
	if((row+2 < mMaxR) && (column+2 < mMaxC) && (mBoard[row+1][column+1] != 0)
		&& (mBoard[row+1][column+1] == mBoard[row+2][column+2]))
		{
		//Check who won
		if((mBoard[row][column] == 1) && (mBoard[row][column] == mBoard[row+1][column+1]))
		{
			toReturn = Points::win; //It was me
		}
		else if((mBoard[row][column] == -1) && (mBoard[row][column] == mBoard[row+1][column+1]))
		{
			toReturn = Points::lose; //It was the oponent
		}
		else
		{
			//Prevented the oponent's victory
			toReturn = (mBoard[row][column] == 1) ? Points::preventVictory : -Points::preventVictory;
		}
	}
	//Two down left
	if((row+2 < mMaxR) && (column-2 >= 0) && (mBoard[row+1][column-1] != 0)
		&& (mBoard[row+1][column-1] == mBoard[row+2][column-2]))
		{
		//Check who won
		if((mBoard[row][column] == 1) && (mBoard[row][column] == mBoard[row+1][column-1]))
		{
			toReturn = Points::win; //It was me
		}
		else if((mBoard[row][column] == -1) && (mBoard[row][column] == mBoard[row+1][column-1]))
		{
			toReturn = Points::lose; //It was the oponent
		}
		else
		{
			//Prevented the oponent's victory
			toReturn = (mBoard[row][column] == 1) ? Points::preventVictory : -Points::preventVictory;
		}
	}
	//Two up right
	if((row-2 >= 0) && (column+2 < mMaxC) && (mBoard[row-1][column+1] != 0)
		&& (mBoard[row-1][column+1] == mBoard[row-2][column+2]))
		{
		//Check who won
		if((mBoard[row][column] == 1) && (mBoard[row][column] == mBoard[row-1][column+1]))
		{
			toReturn = Points::win; //It was me
		}
		else if((mBoard[row][column] == -1) && (mBoard[row][column] == mBoard[row-1][column+1]))
		{
			toReturn = Points::lose; //It was the oponent
		}
		else
		{
			//Prevented the oponent's victory
			toReturn = (mBoard[row][column] == 1) ? Points::preventVictory : -Points::preventVictory;
		}
	}
	//Two up left
	if((row-2 >= 0) && (column-2 >= 0) && (mBoard[row-1][column-1] != 0)
		&& (mBoard[row-1][column-1] == mBoard[row-2][column-2]))
		{
		//Check who won
		if((mBoard[row][column] == 1) && (mBoard[row][column] == mBoard[row-1][column-1]))
		{
			toReturn = Points::win; //It was me
		}
		else if((mBoard[row][column] == -1) && (mBoard[row][column] == mBoard[row-1][column-1]))
		{
			toReturn = Points::lose; //It was the oponent
		}
		else
		{
			//Prevented the oponent's victory
			toReturn = (mBoard[row][column] == 1) ? Points::preventVictory : -Points::preventVictory;
		}
	}
	//Others
	if((row-1 >= 0) && (row+1 < mMaxR) && (column-1 >= 0) && (column+1 < mMaxC))
	{
		//One up left, one down right
		if((mBoard[row-1][column-1] != 0) && (mBoard[row-1][column-1] == mBoard[row+1][column+1]))
		{
			//Check who won
			if((mBoard[row][column] == 1) && (mBoard[row][column] == mBoard[row-1][column-1]))
			{
				toReturn = Points::win; //It was me
			}
			else if((mBoard[row][column] == -1) && (mBoard[row][column] == mBoard[row-1][column-1]))
			{
				toReturn = Points::lose; //It was the oponent
			}
			else
			{
				//Prevented the oponent's victory
				toReturn = (mBoard[row][column] == 1) ? Points::preventVictory : -Points::preventVictory;
			}
		}
		//One down left, one up right
		if((mBoard[row+1][column-1] != 0) && (mBoard[row+1][column-1] == mBoard[row-1][column+1]))
		{
			//Check who won
			if((mBoard[row][column] == 1) && (mBoard[row][column] == mBoard[row+1][column-1]))
			{
				toReturn = Points::win; //It was me
			}
			else if((mBoard[row][column] == -1) && (mBoard[row][column] == mBoard[row+1][column-1]))
			{
				toReturn = Points::lose; //It was the oponent
			}
			else
			{
				//Prevented the oponent's victory
				toReturn = (mBoard[row][column] == 1) ? Points::preventVictory : -Points::preventVictory;
			}
		}
	}

	//Additional points for placing the token next to another of the same type.
	if((toReturn != 10) && (toReturn != -10))
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
				{ //Check limits
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

int MiniMax::searchPlay(int const& row, int const& column, int depth, bool isMaximizer)
{
    int worth = evaluate(row, column) - depth; //This is the heuristic
	
	//Check for TERMINAL STATES.

	//If it plays on this position, it wins.
	//Or the maximum depth has been reached, so it's the end of the search on that branch.
	if((worth >= pointsForWin) || (depth >= maxDepth))
	{ 
		return worth;
	}

	//Check for a full board and not wincon, that means a draw.
	bool full = true;
	for(int i = 0; i < mMaxR; i++)
	{
		for(int j = 0; j < mMaxR; j++)
		{
			if(mBoard[i][j] == 0)
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
	int toReturn;
	if(isMaximizer)
	{
		//Start of oponent path.
		int worstUtility = std::numeric_limits<int>::max();
		for(int r = 0; r < mMaxR; r++)
		{
			for(int c = 0; c < mMaxC; c++)
			{
				//Check if the coordinate is occupied or not.
				if(mBoard[r][c] == 0)
				{
					//Play on that coordinate
					mBoard[r][c] = -1;
					//Get the worth value
					worth = searchPlay(r, c, depth+1, false);
					if(worth < worstUtility)
					{
						worstUtility = worth;
					}
					mBoard[r][c] = 0; //Remove token
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
				if(mBoard[r][c] == 0)
				{
					//Play on that coordinate
					mBoard[r][c] = 1;
					//Get the worth value
					worth = searchPlay(r, c, depth+1, true);
					if(worth > bestUtility)
					{
						bestUtility = worth; 
					}
					mBoard[r][c] = 0; //Remove token
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
	int worth;
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
			if(mBoard[r][c] == 0)
			{
				//Play on that coordinate
				mBoard[r][c] = 1;
				//Get the worth value
				worth = searchPlay(r, c, depth, true);
				if(worth > bestUtility)
				{
					bestUtility = worth;
					bestCoordinate.x = c;
					bestCoordinate.y = r; 
				}
				mBoard[r][c] = 0; //Remove token
			}
		}
	}

	//Debugging
	//printBoard();
	std::cout << "\n" << "bestUtility=" << bestUtility << "\n" << "coords=(" << bestCoordinate.x << "," << bestCoordinate.y << ")\n";
	return bestCoordinate;
}

} // namespace ttt
