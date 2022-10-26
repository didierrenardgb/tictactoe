#pragma once

#include <vector>
#include "Board.h"
#include "Tile.h"
#include "Coordinates.h"

namespace ttt
{
	struct NextMove {
		int depth = -1;
		Coordinates coords = { -1,-1 };
	};

	class IntMatrix
	{
	public:
		IntMatrix(Board const& original, std::string const& name);
		IntMatrix(int size, unsigned int wincon);
		IntMatrix(int height, int width, unsigned int wincon);

		//no paramless constructor
		IntMatrix() = delete;

		IntMatrix(IntMatrix& other);
		
		bool willWin(int currentplayer);
		NextMove evaluateBoard(int maxDepth, int depth = 0, Coordinates const lastMove = { -1,-1 });
		Coordinates finishOffArray(Coordinates const& excpt = { -1,-1 });
		Coordinates defendArray(Coordinates const& excpt = { -1,-1 });

		std::vector<int>& operator[](int i);
		~IntMatrix();

	private:
		unsigned int mWinCon;
		unsigned int mWidth;
		unsigned int mHeight;
		std::vector<std::vector<int>> mBoard;

	};
}
// namespace ttt
