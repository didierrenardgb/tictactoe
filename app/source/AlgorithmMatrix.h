#pragma once

#include "Board.h"
#include <string>

namespace ttt
{

class AlgorithmMatrix 
{
public:
	AlgorithmMatrix (Board const& board, std::string playerName);                         
    int getWidth() const;
    int getHeight() const;
    void executeMovement(Coordinates const& c, bool const& actualPlayer);
    void undoMovement(Coordinates const& c);
    std::vector<Coordinates> getMovs();
    int trivialGameOver();
    int trivialGameOverX();



private:
	std::vector<std::vector<int>> mMatrix;
    int mWidth;
    int mHeight;
    int mWinCondition;
    void foundAWin(int &token, int &i, int &j, int const& iCapturated, int const& jCapturated, int&AmountOfEqualToken, bool& moveToNextWinControl);
    std::vector<std::vector<int>> initializeAlphaBeta(Board const& board, std::string const& name);


};

} // namespace ttt
