#pragma once

#include "IPlayer.h"
#include "Coordinates.h"

#include <compare>
#include <vector>



namespace ttt
{
	using int_matrix = std::vector<std::vector<int>>;

	

	class PlayerFranL : public IPlayer
	{
	public:
		PlayerFranL(std::string const& name, int max_len=4, int min_marks=3, int max_depth=4);
		std::string const& name() const override;
		Coordinates play(Board const& board) const override;

	private:
		std::string mName;
		//Maximum Win condiditon Length that will be allowed for recursive strategy, if board.winCondition is longer, strategy layer will be skipped for performance reasons 
		int mMax_len;

		//Minimum pieces in winCondition distance from each other to be considered a "theater" (a winCondition x winCondition sized sub-board) to by analyzed recursively for strategy
		int mMin_marks;
		
		//Maximum recursive depth to be analyzed. If the recursive strategy goes deeper it will instantly return a random valid move in the theater, instead.
		int mMax_depth;

		//Internal structure used for recursion
		struct next_move {
			int depth;
			Coordinates coords;
			auto operator >(next_move x)
			{
				return depth > x.depth;
			}
		};
		

		int_matrix copyboard(Board const& board) const;
		bool willwin(int currentplayer, int_matrix const& board, int width, int height, int wincon) const;
		PlayerFranL::next_move PlayerFranL::evaluateboard(int_matrix& board, int wincon, int depth=0, Coordinates const last_move = {-1,-1}) const;
		Coordinates PlayerFranL::finish_off_array(int_matrix board, int const height, int const width, int const winCondition, Coordinates const& excpt = { -1,-1 }) const;
		Coordinates PlayerFranL::defend_array(int_matrix board, int const height, int const width, int const winCondition, Coordinates const& excpt = { -1,-1 }) const;


		Coordinates finish_off(Board const& board, Coordinates const& excpt={-1,-1}) const;
		Coordinates defend(Board const& board, Coordinates const& excpt = { -1,-1 }) const;
		Coordinates fork(Board const& board) const;
		Coordinates de_fork(Board const& board) const;
		Coordinates strategize(Board const& board) const;



	};
	
} // namespace ttt
