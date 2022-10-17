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

#pragma once

#include "../include/IPlayer.h"

namespace ttt
{

class PlayerErick : public IPlayer
{
public:
	PlayerErick();
	std::string const& name() const override;
	Coordinates play(Board const& board) const override;

private:
	std::string mName;
	int maxDepth = 9; //Check about this.
	int pointsForWin = 9; //Temporal

	//MMM
	const int MAXR = 3;
	const int MAXC = 3;

	//Methods
	Coordinates minimax(Board const& board);
	const int minimize(Board const& board, int** myBoard, int row, int column, int depth);
	const int maximize(Board const& board, int** myBoard, int row, int column, int depth);
	const int evaluate(Board const& board, int** const& myBoard, int row, int column);

	static int** copyBoard(Board const& board);
	static void printBoard(int** const& myBoard);

};

} // namespace ttt
