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
	PlayerFranL::PlayerFranL(std::string const& name, int max_len, int min_marks, int max_depth) :
		mName(name), mMax_len(max_len), mMin_marks(min_marks), mMax_depth(max_depth)
	{
	}
	//name getter
	std::string const& PlayerFranL::name() const
	{
		return mName;
	}

	//Scan the array of -1s, 0s and 1s, if there is a winning move, returns its coordinates
	//If no such move exists, returns {-1,-1} instead
	//Optionally accepts a Coordinate to ignore (important for fork detection)
	Coordinates PlayerFranL::finish_off_array(int_matrix board, int const height, int const width, int const winCondition, Coordinates const& excpt) const {
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (i != excpt.x && j != excpt.y)
				{

					if (board[i][j] == 0) {
						board[i][j] = 1;
						if (willwin(1, board, width, height, winCondition))
						{
							return { i,j };
						}
						board[i][j] = 0;

					}
				}
			}
		}
		return { -1,-1 };
	}
	//Scan the array of -1s, 0s and 1s, if there is a winning move for the opponent, returns its coordinates
	//If no such move exists, returns {-1,-1} instead
	//Optionally accepts a Coordinate to ignore (important for fork detection)
	Coordinates PlayerFranL::defend_array(int_matrix board, int const height, int const width, int const winCondition, Coordinates const& excpt) const {
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (i != excpt.x && j != excpt.y)
				{

					if (board[i][j] == 0) {
						board[i][j] = -1;
						if (willwin(-1, board, width, height, winCondition))
						{
							return { i,j };
						}
						board[i][j] = 0;

					}
				}
			}
		}
		return { -1,-1 };
	}

	//Take a board from the competition, make a simplified copy to analyze. If there's a winning move, returns its coordinates. 
	//If no such move exists, returns {-1,-1} instead
	//Optionally accepts a Coordinate to ignore (important for fork detection)
	Coordinates PlayerFranL::finish_off(Board const& board, Coordinates const& excpt) const{
		auto testboard = copyboard(board);
		
		Coordinates coords= finish_off_array(testboard, board.height(), board.width(), board.winCondition(), excpt);
		testboard.clear();
		return coords;
	}

	//Take a board from the competition, make a simplified copy to analyze. If there's a move  that results in victory for the opponent, returns its coordinates.
	//If no such move exists, returns {-1,-1} instead
	//Optionally accepts a Coordinate to ignore (important for fork detection)
	Coordinates PlayerFranL::defend(Board const& board, Coordinates const& excpt) const{
		auto testboard = copyboard(board);
		Coordinates coords= defend_array(testboard, board.height(), board.width(), board.winCondition(), excpt);
		testboard.clear();
		return coords;
	}


	//Take a board from the competition, make a simplified copy to analyze.
	//If there's a move that is guaranteed to win next turn, returns its coordinates.
	//If no such move exists, returns {-1,-1} instead
	Coordinates PlayerFranL::fork(Board const& board) const{
		auto tempboard = copyboard(board);
		Coordinates coords;
		for (int i = 0; i < board.height(); i++)
		{
			for (int j = 0; j < board.width(); j++)
			{
				if (tempboard[i][j] == 0) {
					tempboard[i][j] = 1;

					coords = finish_off_array(tempboard, board.height(), board.width(), board.winCondition());
					if (coords.x != -1)
					{
						coords = finish_off_array(tempboard, board.height(), board.width(), board.winCondition(),coords);
						if (coords.x != -1)
						{
							tempboard.clear();
							return { i,j };
						}
					}
					
					tempboard[i][j] = 0;

				}
			}
		}
		tempboard.clear();
		return { -1,-1 };
	}

	//Take a board from the competition, make a simplified copy to analyze.
	//If there's a move that if taken by the opponent guarantees an opponent victory next turn, returns its coordinates.
	//If no such move exists, returns {-1,-1} instead
	Coordinates PlayerFranL::de_fork(Board const& board) const{
		auto tempboard = copyboard(board);
		Coordinates coords;
		for (int i = 0; i < board.height(); i++)
		{
			for (int j = 0; j < board.width(); j++)
			{
				if (tempboard[i][j] == 0) {
					tempboard[i][j] = -1;

					coords = defend_array(tempboard, board.height(), board.width(), board.winCondition());
					if (coords.x != -1)
					{
						coords = defend_array(tempboard, board.height(), board.width(), board.winCondition(), coords);
						if (coords.x != -1)
						{
							tempboard.clear();
							return { i,j };
						}
					}

					tempboard[i][j] = 0;

				}
			}
		}
		tempboard.clear();
		return { -1,-1 };
	}

	//Strategizes next move via a recursive function when Rules based system does not find a winning move for the current and next turn. 
	Coordinates PlayerFranL::strategize(Board const& board) const {
		if (board.valid())
		{
			auto tempboard = copyboard(board);
			Coordinates coords={-1,-1};
			

			bool available_theaters = false;

			//Only apply solving algorithm if the length of the wincon is short enough for performace reasons

			if (board.winCondition() <= mMax_len)
			{
				int lowest_depth = std::numeric_limits<int>::max();
				int total_marks = 0;
				int_matrix smalltempboard(board.winCondition());
				for (int i = 0; i < board.winCondition(); i++)
				{
					std::vector<int> temp(board.winCondition(),0);
						smalltempboard[i] = temp;
				}
				
				for (int offsetX = 0; offsetX < (board.height() - board.winCondition()); offsetX++)
				{
					for (int offsetY = 0; offsetY < (board.width() - board.winCondition()); offsetY++)
					{
						for (int i = 0; i < board.winCondition(); i++)
						{
						

								for (int j = 0; j < board.winCondition(); j++)
								{
								
										smalltempboard[i][j] = tempboard[i+offsetX][j+offsetY];
										total_marks += abs(smalltempboard[i][j]);
								}
							

						}
					
						if (total_marks >= mMin_marks)
						{
							available_theaters = true;
							//start of recursive algorithm
							PlayerFranL::next_move solution = evaluateboard(smalltempboard, board.winCondition());
							if (solution.depth < lowest_depth)
							{
								lowest_depth = solution.depth;
								coords = solution.coords;
							}
						}
						total_marks = 0;
					}

				}
				
				smalltempboard.clear();



			}

			//Generate random valid coordinate if there are no available theaters
			//a theater is a wincon x wincon sized subboard with at least mMin_marks pieces.
			if (!available_theaters)
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
			tempboard.clear();
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
			Coordinates coords = finish_off(board);
			if (coords.x != (-1))
				return { coords.y,coords.x };

			coords = defend(board);
			if (coords.x != (-1))
				return { coords.y,coords.x };

			coords = fork(board);
			if (coords.x != (-1))
				return { coords.y,coords.x };

			coords = de_fork(board);
			if (coords.x != (-1))
				return { coords.y,coords.x };

			//Strategic Layer, more costly
			coords = PlayerFranL::strategize(board);
			return {coords.y,coords.x};
		}

		return { 0, 0 };
	}

	//Create a simplified copy of the board, passing from the object to a 2d array of 0s (free spaces), -1s (controlled by an opponent) and 1s (controlled by the player)
	int_matrix PlayerFranL::copyboard(Board const& board) const {
		int_matrix array (board.height());
		for (int i = 0; i < board.height(); i++)
		{
			array[i] = std::vector<int>(board.width(),0);
			for (int j = 0; j < board.width(); j++)
			{
				if (board.tile({ j,i })->owner().has_value())
				{
					if (board.tile({ j,i })->owner().value().get().name() == this->name()) {
						array[i][j]=1;
					}
					else
					{
						array[i][j]=-1;
					}
				}
				else
				{
					array[i][j] = 0;
				}
			}
		}
		return array;
	}

	//Check if player currentplayer will win in the current boardstate, returns true if currentplayer has won in board, false otherwise.
	bool PlayerFranL::willwin(int currentplayer, int_matrix const& board,int width, int height, int wincon) const {

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
					auto const& prev = board[x][y];

					if (x+w<height && prev==board[x + w][y] && prev==currentplayer)
					{
						horizontal++;
					}

					if (y + w < width && prev == board[x][y+w] && prev == currentplayer)
					{
						vertical++;
					}

					if (y + w < width && x + w < height && prev == board[x + w][y+w] && prev == currentplayer)
					{
						diagonal++;
					}

					if (y + w < width && x - w > 0 && prev == board[x - w][y+w] && prev == currentplayer)
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
	PlayerFranL::next_move PlayerFranL::evaluateboard(int_matrix& board, int wincon, int depth, Coordinates const last_move) const {
		Coordinates coords = last_move;
		int lowest_depth = std::numeric_limits<int>::max();
		//If no valid move was found after mMax_depth searches, return a random valid move
		if (depth > mMax_depth)
		{
		
				for (int i = 0; i < wincon; i++)
				{
					{
						for (int j = 0; j < wincon; j++)
						{
							if (board[i][j] == 0)
							{
								return { std::numeric_limits<int>::max(), { i,j } };
							}
						}
					}

				}

				//Final sanity check in case of a clogged board with no valid moves
				return { 9999, { -1,-1 } };


		}
			if (last_move.x == -1)
			{
				for (int i = 0; i < wincon; i++)
				{
					{
						for (int j = 0; j < wincon; j++)
						{
							if (board[i][j] == 0)
							{
								board[i][j] = 1;
								PlayerFranL::next_move boardevaluation = evaluateboard(board, wincon, depth + 1, { i,j });
								if (boardevaluation.depth < lowest_depth)
								{
									coords = boardevaluation.coords;
									lowest_depth = boardevaluation.depth;
								}
								board[i][j] = 0;
							}
						}
					}

				}
				//If no ending move was found after recursion, return a valid move as a sanity check.
				if (lowest_depth == std::numeric_limits<int>::max())
				{
					for (int i = 0; i < wincon; i++)
					{
						{
							for (int j = 0; j < wincon; j++)
							{
								if (board[i][j] == 0)
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
					if (willwin(1, board, wincon, wincon, wincon))
					{
						coords = last_move;
						lowest_depth = depth;
					}
					else
					{
						for (int i = 0; i < wincon; i++)
						{
							{
								for (int j = 0; j < wincon; j++)
								{
									if (board[i][j] == 0)
									{
										board[i][j] = 1;
										PlayerFranL::next_move boardevaluation = evaluateboard(board, wincon, depth + 1, { i,j });
										if (boardevaluation.depth < lowest_depth)
										{
											coords = boardevaluation.coords;
											lowest_depth = boardevaluation.depth;
										}
										board[i][j] = 0;
									}
								}
							}

						}

					}
				}
				else
				{
					if (willwin(-1, board, wincon, wincon, wincon))
					{
						lowest_depth = std::numeric_limits<int>::max() - depth;
						coords = last_move;
					}
					else
					{
						lowest_depth = 0;
						for (int i = 0; i < wincon; i++)
						{
							{
								for (int j = 0; j < wincon; j++)
								{
									if (board[i][j] == 0)
									{
										board[i][j] = -1;
										PlayerFranL::next_move boardevaluation = evaluateboard(board, wincon, depth + 1, { i,j });
										if (boardevaluation.depth > lowest_depth)
										{
											coords = boardevaluation.coords;
											lowest_depth = boardevaluation.depth;
										}
										board[i][j] = 0;
									}
								}
							}

						}
					}

				}
			}
			return { lowest_depth,coords };
		}

} // namespace ttt
