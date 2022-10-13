#pragma once

#include "IPlayer.h"
#include "Coordinates.h"

#include <compare>



namespace ttt
{
	

	class PlayerFranL : public IPlayer
	{
	public:
		PlayerFranL(std::string const& name, int depth=2);
		std::string const& name() const override;
		Coordinates play(Board const& board) const override;

	private:
		std::string mName;
		int mDepth;

		struct next_move {
			int points;
			Coordinates coords;
		};
		

		int** copyboard(Board const& board) const;
		bool willwin(int currentplayer, int** const& board, int width, int height, int wincon) const;
		int** evaluateboard(int** const& board, int width, int height, int wincon) const;

		Coordinates finish_off(Board const& board, Coordinates excpt={-1,-1}) const;
		Coordinates defend(Board const& board, Coordinates excpt = { -1,-1 }) const;
		Coordinates fork(Board const& board) const;
		Coordinates de_fork(Board const& board) const;
		next_move strategize(int dephth, Board const& board) const;

	};
	
} // namespace ttt
