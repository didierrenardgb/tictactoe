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
		

		

		Coordinates finish_off(Board const& board) const;
		Coordinates defend(Board const& board) const;
		Coordinates fork(Board const& board) const;
		Coordinates de_fork(Board const& board) const;
		next_move strategize(int dephth, Board const& board) const;

	};
	
} // namespace ttt
