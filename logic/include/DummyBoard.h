#pragma once

#include "Board.h"
#include "Constants.h"

namespace ttt
{	
class DummyBoard
{
public:
	

	/*
	 *Returns Spot at given Coordinates
	 */
	Spot& at(Coordinates const& coord);
	
	/*
	 * Given coords and dimentions,
	 * returns if spot at coords is valid
	 */
	bool checkValid(int x, int y);
	
	/*
	 * Given a board and a player, creates a dummyboard
	 * assigning FIRST value to FIRST owned spots, SECOND value
	 * to non FIRST owned spots and NONE value to ownerless Spots
	 */
	DummyBoard(Board const& board, IPlayer const& player);
	
	/*
	 * Given coords, dimentions and an owner,
	 * returns if owner owns the Spot at coords in the dummyboard
	 * if spot invalid also returns false
	 */
	bool check(int x, int y, constants::PLAYERS who);
	
	/*
	 * Given an owner and a coord, assings owner to Spot at coord in board
	 * then updates the score of neighbouring Spots
	 */
	void addAt(constants::PLAYERS owner, Coordinates const& coord);
	
	/*
	 * Given a player and dimentions,
	 * if winning coords exists it returns them else
	 * if they don't exist it returns invalid coords
	 */
	Coordinates winningCoords(constants::PLAYERS who, int winCon);

private:
	SpotMat m_board;
	int m_width;
	int m_height;
};
} // namespace ttt
