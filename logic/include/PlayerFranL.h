#pragma once

#include "IPlayer.h"
#include "Coordinates.h"

#include <compare>



namespace ttt
{
	

	class PlayerFranL : public IPlayer
	{
	public:
		PlayerFranL(std::string const& name, int max_len=4, int min_marks=3);
		std::string const& name() const override;
		Coordinates play(Board const& board) const override;

	private:
		std::string mName;
		int mMax_len;
		int mMin_marks;

		struct next_move {
			int depth;
			Coordinates coords;
			auto operator >(next_move x)
			{
				if (depth > x.depth)
					return 1;
				else
					return 0;
			}
		};
		

		int** copyboard(Board const& board) const;
		bool willwin(int currentplayer, int** const& board, int width, int height, int wincon) const;
		PlayerFranL::next_move PlayerFranL::evaluateboard(int** & board, int wincon, int depth=0, Coordinates const last_move = {-1,-1}) const;
		Coordinates PlayerFranL::finish_off_array(int** board, int const height, int const width, int const winCondition, Coordinates const& excpt = { -1,-1 }) const;
		Coordinates PlayerFranL::defend_array(int** board, int const height, int const width, int const winCondition, Coordinates const& excpt = { -1,-1 }) const;


		Coordinates finish_off(Board const& board, Coordinates const& excpt={-1,-1}) const;
		Coordinates defend(Board const& board, Coordinates const& excpt = { -1,-1 }) const;
		Coordinates fork(Board const& board) const;
		Coordinates de_fork(Board const& board) const;
		Coordinates strategize(Board const& board) const;



	};
	
} // namespace ttt
