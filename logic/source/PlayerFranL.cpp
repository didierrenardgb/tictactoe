#include "PlayerFranL.h"

#include "Board.h"
#include "Coordinates.h"
#include "Tile.h"

#include <random>
#include <set>
#include <limits>

namespace ttt
{

	//Constructor
	PlayerFranL::PlayerFranL(std::string const& name, int maxLen, int minMarks, int maxDepth) :
		mName(name), mMaxLen(maxLen), mMinMarks(minMarks), mMaxDepth(maxDepth)
	{
	}
	//name getter
	std::string const& PlayerFranL::name() const
	{
		return mName;
	}

	//Take a board from the competition, make a simplified copy to analyze. If there's a winning move, returns its coordinates. 
	//If no such move exists, returns {-1,-1} instead
	//Optionally accepts a Coordinate to ignore (important for fork detection)
	Coordinates PlayerFranL::finishOff(Board const& board, Coordinates const& excpt) const{
		IntMatrix testboard(board,mName);
		
		Coordinates coords= testboard.finishOffArray(excpt);
		return coords;
	}

	//Take a board from the competition, make a simplified copy to analyze. If there's a move  that results in victory for the opponent, returns its coordinates.
	//If no such move exists, returns {-1,-1} instead
	//Optionally accepts a Coordinate to ignore (important for fork detection)
	Coordinates PlayerFranL::defend(Board const& board, Coordinates const& excpt) const{
		IntMatrix testboard(board, mName);
		Coordinates coords= testboard.defendArray(excpt);
		return coords;
	}


	//Take a board from the competition, make a simplified copy to analyze.
	//If there's a move that is guaranteed to win next turn, returns its coordinates.
	//If no such move exists, returns {-1,-1} instead
	Coordinates PlayerFranL::fork(Board const& board) const{
		IntMatrix tempboard(board, mName);
		Coordinates coords;
		for (int i = 0; i < board.height(); i++)
		{
			for (int j = 0; j < board.width(); j++)
			{
				if (tempboard[i][j] == 0) {
					tempboard[i][j] = 1;

					coords = tempboard.finishOffArray();
					if (coords.x != -1)
					{
						coords = tempboard.finishOffArray(coords);
						if (coords.x != -1)
						{
							return { i,j };
						}
					}
					
					tempboard[i][j] = 0;

				}
			}
		}
		return { -1,-1 };
	}

	//Take a board from the competition, make a simplified copy to analyze.
	//If there's a move that if taken by the opponent guarantees an opponent victory next turn, returns its coordinates.
	//If no such move exists, returns {-1,-1} instead
	Coordinates PlayerFranL::deFork(Board const& board) const{
		IntMatrix tempboard(board, mName);
		Coordinates coords;
		for (int i = 0; i < board.height(); i++)
		{
			for (int j = 0; j < board.width(); j++)
			{
				if (tempboard[i][j] == 0) {
					tempboard[i][j] = -1;

					coords = tempboard.defendArray();
					if (coords.x != -1)
					{
						coords = tempboard.defendArray(coords);
						if (coords.x != -1)
						{
							return { i,j };
						}
					}

					tempboard[i][j] = 0;

				}
			}
		}
		return { -1,-1 };
	}

	//Strategizes next move via a recursive function when Rules based system does not find a winning move for the current and next turn. 
	Coordinates PlayerFranL::strategize(Board const& board) const {
		if (board.valid())
		{
			IntMatrix tempboard(board, mName);
			Coordinates coords={-1,-1};
			

			bool availableTheaters = false;

			//Only apply solving algorithm if the length of the wincon is short enough for performace reasons

			if (board.winCondition() <= mMaxLen)
			{
				int lowestDepth = std::numeric_limits<int>::max();
				int totalMarks = 0;
				IntMatrix smallTempboard(board.winCondition(), board.winCondition());
				
				for (int offsetX = 0; offsetX <= (board.height() - board.winCondition()); offsetX++)
				{
					for (int offsetY = 0; offsetY <= (board.width() - board.winCondition()); offsetY++)
					{
						for (int i = 0; i < board.winCondition(); i++)
						{
						

								for (int j = 0; j < board.winCondition(); j++)
								{
								
										smallTempboard[i][j] = tempboard[i+offsetX][j+offsetY];
										totalMarks += std::abs(smallTempboard[i][j]);
								}
							

						}
					
						if (totalMarks >= mMinMarks)
						{
							availableTheaters = true;
							//start of recursive algorithm
							NextMove solution = smallTempboard.evaluateBoard(board.winCondition(),mMaxDepth);
							if (solution.depth < lowestDepth)
							{
								lowestDepth = solution.depth;
								coords.x = solution.coords.x+offsetX;
								coords.y = solution.coords.y + offsetY;
							}
						}
						totalMarks = 0;
					}

				}

			}

			//Generate random valid coordinate if there are no available theaters
			//a theater is a wincon x wincon sized subboard with at least mMin_marks pieces.
			if (!availableTheaters)
			{
				coords = { std::rand() % board.width(), std::rand() % board.height() };
				while ((board.tile(coords) == nullptr) || board.tile(coords)->owner().has_value())
				{
					coords = { std::rand() % board.width(), std::rand() % board.height() };
				}


			}
			//Final Sanity Check, if recursion stopped due to depth or a clogged board, return a random valid move
			if (coords.x == -1)
			{
				coords = { std::rand() % board.width(), std::rand() % board.height() };
				while ((board.tile(coords) == nullptr) || board.tile(coords)->owner().has_value())
				{
					coords = { std::rand() % board.width(), std::rand() % board.height() };
				}
			}
			return coords;
		}

		return { 0, 0 };
	}

	//Ai logic function, called by the game every turn.
	Coordinates PlayerFranL::play(Board const& board) const
	{
		if (board.valid())
		{
			
			//Tactics Layer
			Coordinates coords = finishOff(board);
			if (coords.x != (-1))
				return { coords.y,coords.x };

			coords = defend(board);
			if (coords.x != (-1))
				return { coords.y,coords.x };

			coords = fork(board);
			if (coords.x != (-1))
				return { coords.y,coords.x };

			coords = deFork(board);
			if (coords.x != (-1))
				return { coords.y,coords.x };

			//Strategic Layer, more costly
			coords = PlayerFranL::strategize(board);
			return {coords.y,coords.x};
		}

		return { 0, 0 };
	}

} // namespace ttt
