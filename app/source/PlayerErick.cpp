/***************************************************************
*                          $$$$$$$                             *
*                      $$$$$$$$$$$$$$                          *
*                   $$$$$$$$$$$$$$$$$$                         *
*                  $$$$$$$$$$$$$$$$$$$$                        *
*                 $$$$$$$$$$$$$$$O$$$$$    $$$$$$              *
*                 $$$$$$$$O$$$$$$$$$$$$   $$$  $$$             *
*                 $$$$$$$$$$$$$$$$$$$$  $$$$    $$             *
*                  $$$$$$$$$$$$$$$$$$$$$$$$                    *
*                   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$             *
*           $$$$$$     $$$$$$$$$$$$$$$$$$        $$            *
*          $$   $$$$$$$$$$$$$$$$$$$$$$$         $$             *
*           $$$    $$$$$$$$$$$$$$$$$$$$$$$$$$$$                *
*               $$$$$$$$  $$$ $$$$$$          $$               *
*             $$$$       $$$  $$$ $$$      $$$$                *
*            $$$       $$$$   $$$  $$$                         *
*             $$$$$   $$$     $$$   $$$$    $$                 *
*              $$$    $$$$$$  $$$    $$$$$$$$                  *
*                      $$$     $$$$$   $$$$                    *
*                                                              *
*                      Erick Andersson                         *
*                erick.andersson98@gmail.com                   *
*                                                              *
****************************************************************/

#include "PlayerErick.h"

#include "Board.h"
#include "Coordinates.h"
#include "Tile.h"
#include "MiniMax.h"

#include <random>

#include <iostream>
#include <limits>

namespace ttt
{
const int pointsForWin = 5;
const int maxDepth = 4;

static Coordinates play(Board const& board, std::string player)
{
	if (board.valid())
	{
		MiniMax minimaxObj(board, player);
		//Debugging
		minimaxObj.printBoard();
		
		return minimaxObj.play();
	}

	return { 0, 0 };
}

PlayerErick::PlayerErick():
	mName("Erick")
{
}

std::string const& PlayerErick::name() const
{
	return mName;
}

Coordinates PlayerErick::play(Board const& board) const
{
	return ttt::play(board, name());
}

} // namespace ttt
