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

#include "IPlayer.h"
#include <vector>

namespace ttt
{

//This class is used for the implementation of the minimax algorithm. 
class MiniMax
{
public:
    MiniMax(Board const& board);
    Coordinates play();
    void printBoard();

private:
    //Methods
    void copyBoard(Board const& board);
    int searchPlay(int const& row, int const& column, int depth, bool isMaximizer);
    const int evaluate(int const& row, int const& column); //Heuristic function.

    //Attributes
    std::vector<std::vector<int>> myBoard;
    int MAXR;
    int MAXC;
    bool isEmpty;
    const int pointsForWin = 5;
    const int maxDepth = 4;

};

}