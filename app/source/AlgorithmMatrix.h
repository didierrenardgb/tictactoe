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
    void printBoardState();
    void executeMovement(Coordinates const& c, bool const& actualPlayer);
    void undoMovement(Coordinates const& c);
    std::vector<Coordinates> getMovs();
    int trivialGameOver();


private:
	std::vector<std::vector<int>> matrix;
    int width;
    int height;
    int winCondition;

    
    int foundAWin(int &i, int &j, int const& iCapturated, int const& jCapturated, int&AmountOfEqualToken, int const& winCondition, bool& moveToNextWinControl);
    std::vector<std::vector<int>> initializeAlphaBeta(Board const& board, std::string const& name);


};

} // namespace ttt
