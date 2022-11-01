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

#include "Board.h"
#include <string>
#include <vector>

namespace ttt
{

// This class is used for the implementation of the minimax algorithm.
class MiniMax
{
public:
    MiniMax(Board const &board, std::string const &player);
    Coordinates play();
    void printBoard();

private:
    // Methods
    void copyBoard(Board const &board);
    int checkWincon(int row, int column, int rowIncrement, int colIncrement);
    int evaluate(int row, int column); // Heuristic function.
    int searchPlay(int row, int column, int depth, bool isMaximizer);

    // Attributes
    std::string mPlayer;
    std::vector<std::vector<int>> mBoard;
    int mMaxR;
    int mMaxC;
    bool mIsEmpty;
    int mWinCondition;
};

}