#pragma once

#include <vector>
#include "Board.h"
#include "Tile.h"
#include "Coordinates.h"
#include "EnumPlayerFranLVals.h"

namespace ttt
{
	

	struct NextMove 
	{
		int depth = INVALID;
		Coordinates coords = { INVALID , INVALID };
	};

	class IntMatrix
	{
	public:
		IntMatrix(Board const& original, std::string const& name);
		IntMatrix(int size, unsigned int wincon);
		IntMatrix(int height, int width, unsigned int wincon);

		//no paramless constructor
		IntMatrix() = delete;

		~IntMatrix() = default;

		IntMatrix(IntMatrix const& other);
		
		bool willWin(int currentplayer) const;
		NextMove evaluateBoard(int maxDepth, int depth = 0 );
		NextMove evaluateBoardRecursive(int maxDepth, int depth, Coordinates const& lastMove = { INVALID, INVALID });
		NextMove scanNextMove(int maxDepth, int depth, int lowestDepth, int player);
		Coordinates finishOffArray(Coordinates const& excpt = { INVALID , INVALID });
		Coordinates defendArray(Coordinates const& excpt = { INVALID, INVALID });

		std::vector<int>& operator[](int i);
		const std::vector<int>& operator[](int i) const;


	private:
		unsigned int mWinCon;
		unsigned int mWidth;
		unsigned int mHeight;
		std::vector<std::vector<int>> mBoard;

	};
}
// namespace ttt
