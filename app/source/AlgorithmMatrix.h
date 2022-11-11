#pragma once

#include "Board.h"
#include <string>

enum class TokenValues // The token values that the board will use.
{
    emptyToken = 0,
    currentPlayerToken,
    rivalToken
};

enum class GameStatesValues // The semantics value of all the possible game states that we can have.
{
    theGameIsNotOverYet = 0,
    currentPlayerWins,
    currentPlayerDefeated,
    playersTie
};

namespace ttt
{
    class AlgorithmMatrix
    {
    public:
        AlgorithmMatrix(Board const &board, std::string const&  playerName);
        int getWidth() const;
        int getHeight() const;
        void executeMovement(Coordinates const &c, bool actualPlayer);
        void undoMovement(Coordinates const &c);
        std::vector<Coordinates> getMovs();
        int trivialGameOver();

    private:
        std::vector<std::vector<int>> mMatrix;
        int mWidth;
        int mHeight;
        int mWinCondition;
        void checkVictory(int &token, int &i, int &j, int &iCapturated, int &jCapturated, int &AmountOfEqualToken, bool &moveToNextWinControl);
        int checkAxisVictory(int i , int y, int iIncrement, int jIncrement, int actualGameState);        
        std::vector<std::vector<int>> initializeAlphaBeta(Board const &board, std::string const &name);
        bool inRange(int i, int j , int iIncrement , int jIncrement);
    };

} // namespace ttt
