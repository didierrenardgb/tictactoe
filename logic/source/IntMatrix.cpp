#include "IntMatrix.h"
#include <string>

namespace ttt

{
	
	//Constructor by creating a simplified copy of the board, passing from the object to a 2d array of 0s (free spaces),
	//-1s (controlled by an opponent) and 1s (controlled by the player)
	IntMatrix::IntMatrix(Board const& original, std::string const& name)
	{
		mHeight = original.height();
		mWidth = original.width();
		mWinCon = original.winCondition();
		mBoard = std::vector<std::vector<int>>(original.height());
		for (int i = 0; i < original.height(); i++)
		{
			mBoard[i] = std::vector<int>(original.width(), 0);
			for (int j = 0; j < original.width(); j++)
			{
				if (original.tile({ j,i })->owner().has_value())
				{
					if (original.tile({ j,i })->owner().value().get().name() == name) 
					{
						mBoard[i][j] = PLAYER;
					}
					else
					{
						mBoard[i][j] = OPPONENT;
					}
				}
				else
				{
					mBoard[i][j] = EMPTY;
				}
			}
		}
	}

	//default initializers for all 0
	IntMatrix::IntMatrix(int height, int width,unsigned int wincon)
	{
		mHeight = height;
		mWidth = width;
		mWinCon = wincon;
		mBoard = std::vector<std::vector<int>>(height);
		for (int i = 0; i < height; i++)
		{
			mBoard[i] = std::vector<int>(width, EMPTY);
		}
	}
	//default initializer for all 0s of a square intmatrix
	IntMatrix::IntMatrix(int size, unsigned int wincon): IntMatrix(size,size, wincon)
	{
	}

	//copy constructor
	IntMatrix::IntMatrix(IntMatrix const& other)
	{
		mHeight = other.mHeight;
		mWidth = other.mWidth;
		mWinCon = other.mWinCon;
		size_t otherHeight = other.mBoard.size();
		size_t otherWidth = other.mBoard[0].size();
		mBoard = std::vector<std::vector<int>>(otherHeight);
		for (size_t i = 0; i < otherHeight; i++)
		{
			mBoard[i]= std::vector<int>(otherWidth);
			for (size_t j = 0; j < otherWidth; j++)
			{
				mBoard[i][j] = other.mBoard[i][j];
			}
		}
	}

	//access line layer
	std::vector<int>& IntMatrix::operator[](int i) {
		return mBoard[i];
	}
	const std::vector<int>&  IntMatrix::operator[](int i) const{
		return mBoard[i];
	}

	//Check if player currentplayer will win in the current boardstate, returns true if currentplayer has won in board, false otherwise.
	bool IntMatrix::willWin(int currentplayer) const {

		for (int x = 0; x < mHeight; x++)
		{
			for (int y = 0; y < mWidth; y++)
			{
				int vertical = 1;
				int horizontal = 1;
				int diagonal = 1;
				int diagonal2 = 1;
				for (int w = 1; w < mWinCon; w++)
				{
					auto const& prev = mBoard[x][y];

					if (x + w < mHeight && prev == mBoard[x + w][y] && prev == currentplayer)
					{
						horizontal++;
					}

					if (y + w < mWidth && prev == mBoard[x][y + w] && prev == currentplayer)
					{
						vertical++;
					}

					if (y + w < mWidth && x + w < mHeight && prev == mBoard[x + w][y + w] && prev == currentplayer)
					{
						diagonal++;
					}

					if (y + w < mWidth && x - w > 0 && prev == mBoard[x - w][y + w] && prev == currentplayer)
					{
						diagonal2++;
					}
				}

				if ((vertical == mWinCon) || (horizontal == mWinCon) || (diagonal == mWinCon) || (diagonal2 == mWinCon))
				{
					return true;
				}
			}
		}

		return false;
	}

	NextMove IntMatrix::evaluateBoard(int maxDepth, int depth)
	{
		Coordinates coords;
		int lowestDepth = std::numeric_limits<int>::max();

			for (int i = 0; i < mWinCon; i++)
			{

				for (int j = 0; j < mWinCon; j++)
				{
					if (mBoard[i][j] == EMPTY)
					{
						mBoard[i][j] = PLAYER;
						NextMove boardevaluation = evaluateBoardRecursive(maxDepth, depth + 1, { i,j });
						if (boardevaluation.depth < lowestDepth)
						{
							coords = boardevaluation.coords;
							lowestDepth = boardevaluation.depth;
						}
						mBoard[i][j] = EMPTY;
					}
				}


			}
			//If no ending move was found after recursion, return a valid move as a sanity check.
			if (lowestDepth == std::numeric_limits<int>::max())
			{
				for (int i = 0; i < mWinCon; i++)
				{
					for (int j = 0; j < mWinCon; j++)
					{
						if (mBoard[i][j] == EMPTY)
						{
							return { std::numeric_limits<int>::max(),{ i,j } };
						}
					}

				}
			}

			return { lowestDepth,coords };


		

	}


	//Recursive funcion that scans a winning move from a wincon x wincon sized board
	NextMove IntMatrix::evaluateBoardRecursive(int maxDepth, int depth, Coordinates const& lastMove)
	{
		Coordinates coords = lastMove;
		int lowestDepth = std::numeric_limits<int>::max();
		//If no valid move was found after maxDepth searches, return a random valid move
		if (depth > maxDepth)
		{
			for (int i = 0; i < mWinCon; i++)
			{
					for (int j = 0; j < mWinCon; j++)
					{
						if (mBoard[i][j] == EMPTY)
						{
							return { std::numeric_limits<int>::max(), { i,j } };
						}
					}
			}

			//Final sanity check in case of a clogged board with no valid moves
			return { std::numeric_limits<int>::max(), { INVALID,INVALID } };

		}
		
			if (depth % 2 == 0)
			{
				if (willWin(PLAYER))
				{
					coords = lastMove;
					lowestDepth = depth;
				}
				else
				{
					NextMove boardevaluation = scanNextMove(maxDepth, depth, lowestDepth, PLAYER);
					if (boardevaluation.depth < lowestDepth)
					{
						coords = boardevaluation.coords;
						lowestDepth = boardevaluation.depth;
					}
				}
			}
			else
			{
				if (willWin(OPPONENT))
				{
					lowestDepth = std::numeric_limits<int>::max() - depth;
					coords = lastMove;
				}
				else
				{
					lowestDepth = 0;
					NextMove boardevaluation= scanNextMove(maxDepth, depth, lowestDepth, PLAYER);
					if (boardevaluation.depth > lowestDepth)
					{
									coords = boardevaluation.coords;
									lowestDepth = boardevaluation.depth;
					}
					
				}

			}
		
		return { lowestDepth,coords };
	}

	//part of recursive function
	NextMove IntMatrix::scanNextMove(int maxDepth, int depth, int lowestDepth, int player)
	{
		NextMove returnVal{depth,{INVALID,INVALID}};
		for (int i = 0; i < mWinCon; i++)
		{

			for (int j = 0; j < mWinCon; j++)
			{
				if (mBoard[i][j] == EMPTY)
				{
					mBoard[i][j] = player;
					NextMove boardevaluation = evaluateBoardRecursive(maxDepth, depth + 1, { i,j });
					if ((player==PLAYER && boardevaluation.depth < lowestDepth) || (player==OPPONENT && boardevaluation.depth>lowestDepth))
					{
						returnVal = boardevaluation;
					}
					mBoard[i][j] = EMPTY;
				}
			}


		}
		return returnVal;

	}

	//Scan the array of -1s, 0s and 1s, if there is a winning move, returns its coordinates
	//If no such move exists, returns {INVALID,INVALID} ({-1,-1}) instead
	//Optionally accepts a Coordinate to ignore (important for fork detection)
	Coordinates IntMatrix::finishOffArray(Coordinates const& except) {
		for (int i = 0; i < mHeight; i++)
		{
			for (int j = 0; j < mWidth; j++)
			{
				if (i != except.x && j != except.y)
				{

					if (mBoard[i][j] == EMPTY) {
						mBoard[i][j] = PLAYER;
						if (willWin(PLAYER))
						{
							return { i,j };
						}
						mBoard[i][j] = EMPTY;

					}
				}
			}
		}
		return { INVALID,INVALID };
	}

	//Scan the array of -1s, 0s and 1s, if there is a winning move for the opponent, returns its coordinates
//If no such move exists, returns {INVALID, INVALID} ({-1,-1}) instead
//Optionally accepts a Coordinate to ignore (important for fork detection)
	Coordinates IntMatrix::defendArray(Coordinates const& except) {
		for (int i = 0; i < mHeight; i++)
		{
			for (int j = 0; j < mWidth; j++)
			{
				if (i != except.x && j != except.y)
				{

					if (mBoard[i][j] == EMPTY) {
						mBoard[i][j] = OPPONENT;
						if (willWin(OPPONENT))
						{
							return { i,j };
						}
						mBoard[i][j] = EMPTY;

					}
				}
			}
		}
		return { INVALID,INVALID };
	}



	


}

