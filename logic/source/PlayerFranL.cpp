#include "PlayerFranL.h"
#include "EnumPlayerFranLVals.h"

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
	static Coordinates finishOff(Board const& board, std::string const& name, Coordinates const& except = { INVALID, INVALID })
	{
		IntMatrix testboard(board, name);
		
		Coordinates coords= testboard.finishOffArray(except);
		return coords;
	}

	//Take a board from the competition, make a simplified copy to analyze. If there's a move  that results in victory for the opponent, returns its coordinates.
	//If no such move exists, returns {-1,-1} instead
	//Optionally accepts a Coordinate to ignore (important for fork detection)
	static Coordinates defend(Board const& board, std::string const& name, Coordinates const& except = {INVALID , INVALID})
	{
		IntMatrix testboard(board, name);
		Coordinates coords= testboard.defendArray(except);
		return coords;
	}


	//Take a board from the competition, make a simplified copy to analyze.
	//If there's a move that is guaranteed to win next turn, returns its coordinates.
	//If no such move exists, returns {INVALID, INVALID} ({-1,-1}) instead
	static Coordinates fork(Board const& board, std::string const& name)
	{
		IntMatrix tempboard(board, name);
		Coordinates coords;
		for (int i = 0; i < board.height(); i++)
		{
			for (int j = 0; j < board.width(); j++)
			{
				if (tempboard[i][j] == EMPTY) {
					tempboard[i][j] = PLAYER;

					coords = tempboard.finishOffArray();
					if (coords.x != INVALID)
					{
						coords = tempboard.finishOffArray(coords);
						if (coords.x != INVALID)
						{
							return { i,j };
						}
					}
					
					tempboard[i][j] = EMPTY;

				}
			}
		}
		return { INVALID, INVALID };
	}

	//Take a board from the competition, make a simplified copy to analyze.
	//If there's a move that if taken by the opponent guarantees an opponent victory next turn, returns its coordinates.
	//If no such move exists, returns {INVALID, INVALID} ({-1,-1}) instead
	static Coordinates deFork(Board const& board, std::string const& name)
	{
		IntMatrix tempboard(board, name);
		Coordinates coords;
		for (int i = 0; i < board.height(); i++)
		{
			for (int j = 0; j < board.width(); j++)
			{
				if (tempboard[i][j] == EMPTY) {
					tempboard[i][j] = OPPONENT;

					coords = tempboard.defendArray();
					if (coords.x != INVALID)
					{
						coords = tempboard.defendArray(coords);
						if (coords.x != INVALID)
						{
							return { i,j };
						}
					}

					tempboard[i][j] = EMPTY;

				}
			}
		}
		return { INVALID, INVALID };
	}

	//Strategizes next move via a recursive function when Rules based system does not find a winning move for the current and next turn. 
	Coordinates PlayerFranL::strategize(Board const& board) const
	{
		if (board.valid())
		{
			IntMatrix tempboard(board, mName);
			Coordinates coords={INVALID,INVALID};
			

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
								coords.x = solution.coords.x + offsetX;
								coords.y = solution.coords.y + offsetY;
							}
						}
						totalMarks = 0;
					}

				}

			}

			//Generate random valid coordinate if there are no available theaters or if recursion stopped due to depth or a clogged board
			//a theater is a wincon x wincon sized subboard with at least mMin_marks pieces.
			if (!availableTheaters || coords.x == -1)
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
			Coordinates coords = finishOff(board, mName);
			if (coords.x != (INVALID))
				return { coords.y,coords.x };

			coords = defend(board, mName);
			if (coords.x != (INVALID))
				return { coords.y,coords.x };

			coords = fork(board, mName);
			if (coords.x != (INVALID))
				return { coords.y,coords.x };

			coords = deFork(board, mName);
			if (coords.x != (INVALID))
				return { coords.y,coords.x };

			//Strategic Layer, more costly
			coords = PlayerFranL::strategize(board);
			return {coords.y,coords.x};
		}

		return { 0, 0 };
	}

} // namespace ttt
