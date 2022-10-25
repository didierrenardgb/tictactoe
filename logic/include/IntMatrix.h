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
		IntMatrix(int size);
		IntMatrix(int height, int width);

		//no paramless constructor
		IntMatrix() = delete;

		IntMatrix(IntMatrix& other);

		bool willWin(int currentplayer, int width, int height, int wincon);
		NextMove evaluateBoard(int wincon, int maxDepth, int depth = 0, Coordinates const lastMove = { -1,-1 });
		Coordinates finishOffArray(int const height, int const width, int const winCondition, Coordinates const& excpt = { -1,-1 });
		Coordinates defendArray(int const height, int const width, int const winCondition, Coordinates const& excpt = { -1,-1 });

		std::vector<int>& operator[](int i);
		~IntMatrix();

	private:

		std::vector<std::vector<int>> mBoard;

	};
}
// namespace ttt
