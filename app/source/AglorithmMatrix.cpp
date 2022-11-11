#include "AlgorithmMatrix.h"
#include "Coordinates.h"
#include "Tile.h"
#include <vector>
#include <iostream>

namespace ttt
{
    AlgorithmMatrix::AlgorithmMatrix(Board const &board, std::string const& playerName) // The constructor creates the semantic matrix that will be used in the Alpha Beta Algorithm.
    : mWidth(board.width()) , mHeight(board.height()) , mWinCondition(board.winCondition())
    {
        mMatrix.resize(mWidth, std::vector<int>(board.height()));
        for (int i = 0; i < mWidth; i++)
        {
            for (int j = 0; j < mHeight; j++)
            {
                Coordinates c{i, j};
                mMatrix[i][j] = (int) TokenValues::emptyToken; // I Asume that the tile is empty.
                if (board.tile(c)->owner().has_value())
                {
                    if (playerName == board.tile(c)->owner()->get().name()) // Depending on the owner of our tile, the function will put the token asociated.
                    {
                        mMatrix[i][j] = (int) TokenValues::currentPlayerToken;
                    }
                    else
                    {
                        mMatrix[i][j] = (int) TokenValues::rivalToken;
                    }
                }
            }
        }
    }

    int AlgorithmMatrix::getWidth() const
    {
        return mWidth;
    }

    int AlgorithmMatrix::getHeight() const
    {
        return mHeight;
    }

    void AlgorithmMatrix::executeMovement(Coordinates const &c, bool actualPlayer) // Function that execute a movement.
    {
        mMatrix[c.x][c.y] = actualPlayer? (int) TokenValues::currentPlayerToken : (int) TokenValues::rivalToken;
    }

    void AlgorithmMatrix::undoMovement(Coordinates const &c) // Function that undo a movement.
    {
        mMatrix[c.x][c.y] = (int) TokenValues::emptyToken;
    }

    std::vector<Coordinates> AlgorithmMatrix::getMovs() // We get all the posible moves to make in the board.
    {
        std::vector<Coordinates> output;
        output.clear();
        for (int i = 0; i < mWidth; i++)
        {
            for (int j = 0; j < mHeight; j++)
            {
                if (mMatrix[i][j] == (int) TokenValues::emptyToken)
                {
                    output.push_back(Coordinates{i, j});
                }
            }
        }
        return output;
    }

    bool AlgorithmMatrix::inRange(int i, int j , int iAddition , int jAddition)
    {
        if (i+iAddition < mWidth && j+jAddition < mHeight && ((jAddition>=0) || (jAddition<0 && j>0 && j<mHeight)))
        {
           return true;
        }
        return false;
    }

    int  AlgorithmMatrix::checkAxisVictory(int i , int j, int iAddition, int jAddition)
    {   
        int actualGameState = (int) GameStatesValues::theGameIsNotOverYet; //We supouse that we don't have a win.
        int amountOfEqualToken = 1;
        bool moveToNextWinControl = false;
        while (!moveToNextWinControl && amountOfEqualToken < mWinCondition && inRange(i,j,iAddition,jAddition))
        {
            if (mMatrix[i][j] == mMatrix[i + iAddition] [j + jAddition] && mMatrix[i][j] != (int) TokenValues::emptyToken)
            {
                amountOfEqualToken++;
                i=i+iAddition;
                j=j+jAddition;
            }
            else
            {
                moveToNextWinControl = true; // If this is true, then on this path there is no longer a possible victory. We must continue to the next control.
            }
        }
        if (amountOfEqualToken == mWinCondition)
            {
                actualGameState = (mMatrix[i][j] == (int) TokenValues::currentPlayerToken)? (int) (int) GameStatesValues::currentPlayerWins: (int) GameStatesValues::currentPlayerDefeated;
            }
    return actualGameState;
    }

    int AlgorithmMatrix::trivialGameOver() // Function that seeks a victory.
    {
        bool fullBoard = true; // We asume the Tie Game Over.
        bool moveToNextWinControl = false;
        int actualGameState = (int) GameStatesValues::theGameIsNotOverYet; // If the variable gets a value that is different of the partial state value, a victory has cocurred!
        for (int i = 0; i < mWidth; i++)
        {
            for (int j = 0; j < mHeight; j++)
            {
               
                actualGameState = checkAxisVictory(i , j , 1 , 0);    //Row control:  {i + 1 , j + 0}.

                if (actualGameState == (int) GameStatesValues::theGameIsNotOverYet)    //Collumn  control: {i + 0 , j + 1}.
                {
                    actualGameState = checkAxisVictory(i , j , 0 , 1);
                }

                if (actualGameState == (int) GameStatesValues::theGameIsNotOverYet)    //Increasing Diagonal {Q1  to  Q4}: {i + 1 , j - 1} with initial index correction: j + winCondition - 1 .
                {
                    actualGameState = checkAxisVictory(i , j + mWinCondition -1 , 1, -1);
                }

                if (actualGameState== (int) GameStatesValues::theGameIsNotOverYet)     //Decreasing Diagonal {Q2 to Q4}: {i + 1 , j + 1}.
                {
                    actualGameState = checkAxisVictory(i , j , 1, 1);
                }

                if (actualGameState != (int) GameStatesValues::theGameIsNotOverYet) // We found a victory!
                {
                    return actualGameState;
                }

                if ((mMatrix[i][j]) == (int) TokenValues::emptyToken) // If we found at least one tile that isn't empty, then there is no tie yet!.
                {
                    fullBoard = false;
                }
            }
        }
        if (fullBoard)
        {
            return (int) GameStatesValues::playersTie; // In this case, we get a Tie Game Over.
        }
        return (int) GameStatesValues::theGameIsNotOverYet; // If there is, at least, one tile empty, the game will be not over yet.
    }

} // namespace ttt