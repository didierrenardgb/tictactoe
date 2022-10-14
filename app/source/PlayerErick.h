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

	//Methods
	Coordinates minimax(Board const& board);
	const int minimize(Board const& board, int row, int column, int depth);
	const int maximize(Board const& board, int row, int column, int depth);
	const int evaluate(int row, int column);

};

} // namespace ttt
