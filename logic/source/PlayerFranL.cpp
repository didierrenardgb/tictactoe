#include "PlayerFranL.h"

#include "Board.h"
#include "Coordinates.h"
#include "Tile.h"

#include "PlayerExample.h"

#include <random>

namespace ttt
{




	PlayerFranL::PlayerFranL(std::string const& name, int depth) :
		mName(name), mDepth(depth)
	{
	}

	std::string const& PlayerFranL::name() const
	{
		return mName;
	}

	Coordinates PlayerFranL::finish_off_array(int** board, int const height, int const width, int const winCondition, Coordinates const& excpt) const {
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
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

	Coordinates PlayerFranL::defend_array(int** board, int const height, int const width, int const winCondition, Coordinates const& excpt) const {
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				if (i != excpt.x && j != excpt.y)
				{

					if (board[i][j] == 0) {
						board[i][j] = 1;
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


	Coordinates PlayerFranL::finish_off(Board const& board, Coordinates const& excpt) const{
		auto testboard = copyboard(board);
		
		return finish_off_array(testboard, board.height(), board.width(), board.winCondition(), excpt);
	}

	Coordinates PlayerFranL::defend(Board const& board, Coordinates const& excpt) const{
		auto testboard = copyboard(board);
		return defend_array(testboard, board.height(), board.width(), board.winCondition(), excpt);
	}

	Coordinates PlayerFranL::fork(Board const& board) const{
		auto tempboard = copyboard(board);
		Coordinates coords;
		for (int i = 0; i < board.width(); i++)
		{
			for (int j = 0; j < board.height(); j++)
			{
				if (tempboard[i][j] == 0) {
					tempboard[i][j] = 1;

					coords = finish_off_array(tempboard, board.height(), board.width(), board.winCondition());
					if (coords.x != -1)
					{
						coords = finish_off_array(tempboard, board.height(), board.width(), board.winCondition(),coords);
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

	Coordinates PlayerFranL::de_fork(Board const& board) const{
		auto tempboard = copyboard(board);
		Coordinates coords;
		for (int i = 0; i < board.width(); i++)
		{
			for (int j = 0; j < board.height(); j++)
			{
				if (tempboard[i][j] == 0) {
					tempboard[i][j] = 1;

					coords = defend_array(tempboard, board.height(), board.width(), board.winCondition());
					if (coords.x != -1)
					{
						coords = defend_array(tempboard, board.height(), board.width(), board.winCondition(), coords);
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

	PlayerFranL::next_move PlayerFranL::strategize(int depth, Board const& board) const {
		if (board.valid())
		{
			Coordinates coords = { std::rand() % board.width(), std::rand() % board.height() };
			while ((board.tile(coords) == nullptr) || board.tile(coords)->owner().has_value())
			{
				coords = { std::rand() % board.width(), std::rand() % board.height() };
			}
			return { 100,coords };
		}

		return { 0, 0 };
	}

	Coordinates PlayerFranL::play(Board const& board) const
	{
		if (board.valid())
		{
			

			Coordinates coords = finish_off(board);
			if (coords.x != (-1))
				return coords;

			coords = finish_off(board);
			if (coords.x != (-1))
				return coords;

			coords = defend(board);
			if (coords.x != (-1))
				return coords;

			coords = fork(board);
			if (coords.x != (-1))
				return coords;

			coords = de_fork(board);
			if (coords.x != (-1))
				return coords;


			coords = PlayerFranL::strategize(mDepth,board).coords;
			return coords;
		}

		return { 0, 0 };
	}


	int** PlayerFranL::copyboard(Board const& board) const {
		//Me cuesta trabajar sobre una copia del board, asi que hago una copia tonta yo, de nuevo, no me parece muy elegante.
		int** array = new int* [board.height()];
		for (int i = 0; i < board.height(); i++)
		{
			array[i] = new int[board.width()];
			for (int j = 0; j < board.width(); j++)
			{
				if (board.tile({ i,j })->owner().has_value())
				{
					if (board.tile({ i,j })->owner().value().get().name() == this->name()) {
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


	bool PlayerFranL::willwin(int currentplayer, int** const& board,int width, int height, int wincon) const {

		//Funcion copiada y modificada de Match, tiene que haber una mejor forma de hacer esto ;_;
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
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


	int** PlayerFranL::evaluateboard(int** const& board, int width, int height, int wincon) const {
		int** array = 0;
		return array;
	}


} // namespace ttt
