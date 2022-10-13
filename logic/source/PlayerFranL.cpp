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


	Coordinates PlayerFranL::finish_off(Board const& board, Coordinates excpt) const{
		auto testboard = copyboard(board);
		for (int i = 0; i < board.width(); i++)
		{
			for (int j = 0; j < board.height(); j++)
			{
				if (i != excpt.x && j != excpt.y)
				{

				if (testboard[i][j] == 0){
					testboard[i][j] = 1;
					if (willwin(1, testboard, board.width(), board.height(), board.winCondition()))
					{
						return { i,j };
					}
					testboard[i][j] = 0;

				}
				}
			}
		}
		return { -1,-1 };
	}

	Coordinates PlayerFranL::defend(Board const& board, Coordinates excpt) const{
		auto testboard = copyboard(board);
		for (int i = 0; i < board.width(); i++)
		{
			for (int j = 0; j < board.height(); j++)
			{
				if (i != excpt.x && j != excpt.y)
				{

					if (testboard[i][j] == 0) {
						testboard[i][j] = 1;
						if (willwin(-1, testboard, board.width(), board.height(), board.winCondition()))
						{
							return { i,j };
						}
						testboard[i][j] = 0;

					}
				}
			}
		}
		return { -1,-1 };
	}

	Coordinates PlayerFranL::fork(Board const& board) const{
		Board tempboard = board;
		for (int i = 0; i < board.width(); i++)
		{
			for (int j = 0; j < board.height(); j++)
			{
				
			}
		}
		return { -1,-1 };
	}

	Coordinates PlayerFranL::de_fork(Board const& board) const{
		for (int i = 0; i < board.width(); i++)
		{
			for (int j = 0; j < board.height(); j++)
			{
				return { 0,0 };
			}
		}
	}

	PlayerFranL::next_move PlayerFranL::strategize(int depth, Board const& board) const {
		return { -1,{0,0} };
	}

	Coordinates PlayerFranL::play(Board const& board) const
	{
		if (board.valid())
		{
			PlayerExample player("test");
			return player.play(board);

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
		int** array = 0;
		array = new int* [board.height()];
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

					if (prev==board[x + w][y] && prev==currentplayer)
					{
						horizontal++;
					}

					if (prev == board[x][y+w] && prev == currentplayer)
					{
						vertical++;
					}

					if (prev == board[x + w][y+w] && prev == currentplayer)
					{
						diagonal++;
					}

					if (prev == board[x - w][y+w] && prev == currentplayer)
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
