#pragma once

#include "IPlayer.h"
#include "Coordinates.h"
#include "IntMatrix.h"

#include <compare>
#include <vector>



namespace ttt
{
	class PlayerFranL : public IPlayer
	{
	public:
		PlayerFranL(std::string const& name, int maxLen=4, int minMarks=3, int maxDepth=4);
		std::string const& name() const override;
		Coordinates play(Board const& board) const override;

	private:
		std::string mName;
		//Maximum Win condiditon Length that will be allowed for recursive strategy, if board.winCondition is longer, strategy layer will be skipped for performance reasons 
		int mMaxLen;

		//Minimum pieces in winCondition distance from each other to be considered a "theater" (a winCondition x winCondition sized sub-board) to by analyzed recursively for strategy
		int mMinMarks;
		
		//Maximum recursive depth to be analyzed. If the recursive strategy goes deeper it will instantly return a random valid move in the theater, instead.
		int mMaxDepth;

		Coordinates strategize(Board const& board) const;



	};
	
} // namespace ttt
