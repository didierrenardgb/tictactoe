#pragma once

#include "Board.h"
#include <string>

enum TokenValues // The token values that the board will use.
{
    emptyToken = 0,
    currentPlayerToken,
    rivalToken
};

enum GameStatesValues // The semantics value of all the possible game states that we can have.
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
        AlgorithmMatrix(Board const &board, std::string playerName);
        int getWidth() const;
        int getHeight() const;
        void executeMovement(Coordinates const &c, bool const &actualPlayer);
        void undoMovement(Coordinates const &c);
        std::vector<Coordinates> getMovs();
        int trivialGameOver();

    private:
        std::vector<std::vector<int>> mMatrix;
        int mWidth;
        int mHeight;
        int mWinCondition;
        void checkVictory(int &token, int &i, int &j, int &iCapturated, int &jCapturated, int &AmountOfEqualToken, bool &moveToNextWinControl);
        std::vector<std::vector<int>> initializeAlphaBeta(Board const &board, std::string const &name);
    };

} // namespace ttt
