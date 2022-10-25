#include "IntMatrix.h"
#include <string>
namespace ttt
{
	//Create a simplified copy of the board, passing from the object to a 2d array of 0s (free spaces), -1s (controlled by an opponent) and 1s (controlled by the player)
	IntMatrix::IntMatrix(Board const& original, std::string const& name)
	{
		mBoard = std::vector<std::vector<int>>(original.height());
		for (int i = 0; i < original.height(); i++)
		{
			mBoard[i] = std::vector<int>(original.width(), 0);
			for (int j = 0; j < original.width(); j++)
			{
				if (original.tile({ j,i })->owner().has_value())
				{
					if (original.tile({ j,i })->owner().value().get().name() == name) {
						mBoard[i][j] = 1;
					}
					else
					{
						mBoard[i][j] = -1;
					}
				}
				else
				{
					mBoard[i][j] = 0;
				}
			}
		}
	}

	//default initializers for all 0
	IntMatrix::IntMatrix(int height, int width)
	{
		mBoard = std::vector<std::vector<int>>(height);
		for (int i = 0; i < height; i++)
		{
			mBoard[i] = std::vector<int>(width, 0);
		}
	}
	//default initializar for all 0 of a square intmatrix
	IntMatrix::IntMatrix(int size): IntMatrix(size,size)
	{
	}

	//copy constructor
	IntMatrix::IntMatrix(IntMatrix& other)
	{
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

	

	//Destructor for IntMatrix
	IntMatrix::~IntMatrix()
	{
		mBoard.clear();
	}
	//access line layer
	std::vector<int>& IntMatrix::operator[](int i) {
		return mBoard[i];
	}

	//Check if player currentplayer will win in the current boardstate, returns true if currentplayer has won in board, false otherwise.
	bool IntMatrix::willWin(int currentplayer, int width, int height, int wincon) {

		for (int x = 0; x < height; x++)
		{
			for (int y = 0; y < width; y++)
			{
				int vertical = 1;
				int horizontal = 1;
				int diagonal = 1;
				int diagonal2 = 1;
				for (int w = 1; w < wincon; w++)
				{
					auto const& prev = mBoard[x][y];

					if (x + w < height && prev == mBoard[x + w][y] && prev == currentplayer)
					{
						horizontal++;
					}

					if (y + w < width && prev == mBoard[x][y + w] && prev == currentplayer)
					{
						vertical++;
					}

					if (y + w < width && x + w < height && prev == mBoard[x + w][y + w] && prev == currentplayer)
					{
						diagonal++;
					}

					if (y + w < width && x - w > 0 && prev == mBoard[x - w][y + w] && prev == currentplayer)
					{
						diagonal2++;
					}
				}

				if ((vertical == wincon) || (horizontal == wincon) || (diagonal == wincon) || (diagonal2 == wincon))
				{
					return true;
				}
			}
		}

		return false;
	}


	//Recursive funcion that scans a winning move from a wincon x wincon sized board
	NextMove IntMatrix::evaluateBoard(int wincon, int maxDepth, int depth, Coordinates const lastMove) {
		Coordinates coords = lastMove;
		int lowestDepth = std::numeric_limits<int>::max();
		//If no valid move was found after maxDepth searches, return a random valid move
		if (depth > maxDepth)
		{

			for (int i = 0; i < wincon; i++)
			{
				{
					for (int j = 0; j < wincon; j++)
					{
						if (mBoard[i][j] == 0)
						{
							return { std::numeric_limits<int>::max(), { i,j } };
						}
					}
				}

			}

			//Final sanity check in case of a clogged board with no valid moves
			return { std::numeric_limits<int>::max(), { -1,-1 } };


		}
		if (lastMove.x == -1)
		{
			for (int i = 0; i < wincon; i++)
			{
				{
					for (int j = 0; j < wincon; j++)
					{
						if (mBoard[i][j] == 0)
						{
							mBoard[i][j] = 1;
							NextMove boardevaluation = evaluateBoard(wincon, maxDepth, depth + 1, { i,j });
							if (boardevaluation.depth < lowestDepth)
							{
								coords = boardevaluation.coords;
								lowestDepth = boardevaluation.depth;
							}
							mBoard[i][j] = 0;
						}
					}
				}

			}
			//If no ending move was found after recursion, return a valid move as a sanity check.
			if (lowestDepth == std::numeric_limits<int>::max())
			{
				for (int i = 0; i < wincon; i++)
				{
					{
						for (int j = 0; j < wincon; j++)
						{
							if (mBoard[i][j] == 0)
							{
								return { std::numeric_limits<int>::max(),{ i,j } };
							}
						}
					}

				}
			}

		}
		else
		{
			if (depth % 2 == 0)
			{
				if (willWin(1, wincon, wincon, wincon))
				{
					coords = lastMove;
					lowestDepth = depth;
				}
				else
				{
					for (int i = 0; i < wincon; i++)
					{
						{
							for (int j = 0; j < wincon; j++)
							{
								if (mBoard[i][j] == 0)
								{
									mBoard[i][j] = 1;
									NextMove boardevaluation = evaluateBoard(wincon, maxDepth, depth + 1, { i,j });
									if (boardevaluation.depth < lowestDepth)
									{
										coords = boardevaluation.coords;
										lowestDepth = boardevaluation.depth;
									}
									mBoard[i][j] = 0;
								}
							}
						}

					}

				}
			}
			else
			{
				if (willWin(-1, wincon, wincon, wincon))
				{
					lowestDepth = std::numeric_limits<int>::max() - depth;
					coords = lastMove;
				}
				else
				{
					lowestDepth = 0;
					for (int i = 0; i < wincon; i++)
					{
						{
							for (int j = 0; j < wincon; j++)
							{
								if (mBoard[i][j] == 0)
								{
									mBoard[i][j] = -1;
									NextMove boardevaluation = evaluateBoard(wincon, maxDepth, depth + 1, { i,j });
									if (boardevaluation.depth > lowestDepth)
									{
										coords = boardevaluation.coords;
										lowestDepth = boardevaluation.depth;
									}
									mBoard[i][j] = 0;
								}
							}
						}

					}
				}

			}
		}
		return { lowestDepth,coords };
	}
	//Scan the array of -1s, 0s and 1s, if there is a winning move, returns its coordinates
	//If no such move exists, returns {-1,-1} instead
	//Optionally accepts a Coordinate to ignore (important for fork detection)
	Coordinates IntMatrix::finishOffArray(int const height, int const width, int const winCondition, Coordinates const& excpt) {
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (i != excpt.x && j != excpt.y)
				{

					if (mBoard[i][j] == 0) {
						mBoard[i][j] = 1;
						if (willWin(1, width, height, winCondition))
						{
							return { i,j };
						}
						mBoard[i][j] = 0;

					}
				}
			}
		}
		return { -1,-1 };
	}

	//Scan the array of -1s, 0s and 1s, if there is a winning move for the opponent, returns its coordinates
//If no such move exists, returns {-1,-1} instead
//Optionally accepts a Coordinate to ignore (important for fork detection)
	Coordinates IntMatrix::defendArray(int const height, int const width, int const winCondition, Coordinates const& excpt) {
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (i != excpt.x && j != excpt.y)
				{

					if (mBoard[i][j] == 0) {
						mBoard[i][j] = -1;
						if (willWin(-1, width, height, winCondition))
						{
							return { i,j };
						}
						mBoard[i][j] = 0;

					}
				}
			}
		}
		return { -1,-1 };
	}






}

