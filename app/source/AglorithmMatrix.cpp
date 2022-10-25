#include "AlgorithmMatrix.h"
#include "Coordinates.h"
#include "Tile.h"
#include <vector>
#include <iostream>


enum class Token_values{
	EMPTY_TOKEN=0,
	CURRENT_PLAYER_TOKEN,
	RIVAL_TOKEN
};

enum class Game_Over_Values{
	THE_GAME_IS_NOT_OVER_YET=0,
	CURENT_PLAYER_WINS,
	CURRENT_PLAYER_DEFEATED,
	PLAYERS_TIE	
};


namespace ttt
{
	AlgorithmMatrix::AlgorithmMatrix(Board const& board, std::string playerName){

        width = board.width();
        height = board.height();
        winCondition = board.winCondition();

        matrix.resize(board.width(),std::vector<int>(board.height()));
		for (int i = 0 ; i < board.width() ; i++){
			for (int j = 0 ; j < board.height() ; j++){
				Coordinates c{i,j};
				matrix[i][j] = (int) Token_values::EMPTY_TOKEN; //I Asume that the tile is empty.
				if (board.tile(c)->owner().has_value()){
					if (playerName == board.tile(c)->owner()->get().name()){
						matrix[i][j] = (int) Token_values::CURRENT_PLAYER_TOKEN;	
					}
					else{
						matrix[i][j] = (int) Token_values::RIVAL_TOKEN;	
					}
				}
			}
		}
    }

    int AlgorithmMatrix::getWidth() const{
        return width;
    } 

    int AlgorithmMatrix::getHeight() const{
        return height;
    } 


	void AlgorithmMatrix::printBoardState(){
		for (int i = 0 ; i < width ; i++){
			for (int j = 0 ; j < height ; j++){
				std::cout<<"{ "<<i<<" , "<<j<<" }: "<<matrix[i][j]<<" | ";
			}
			std::cout<<std::endl;
		}
		std::cout<<std::endl;std::cout<<std::endl;std::cout<<std::endl;
	}


    void AlgorithmMatrix::executeMovement(Coordinates const& c, bool const& actualPlayer){
        if (actualPlayer){
			matrix[c.x][c.y] = (int)Token_values::CURRENT_PLAYER_TOKEN;}
		else{
			matrix[c.x][c.y] = (int) Token_values::RIVAL_TOKEN;}
    }

    void AlgorithmMatrix::undoMovement(Coordinates const& c){
			matrix[c.x][c.y] = (int)Token_values::EMPTY_TOKEN;
    }


    std::vector<Coordinates> AlgorithmMatrix::getMovs(){
        std::vector<Coordinates> output;
		output.clear();
		for (int i = 0 ; i < width ; i++){
			for (int j = 0 ; j < height ; j++){
				if (matrix[i][j]==(int)Token_values::EMPTY_TOKEN){
					output.push_back(Coordinates{i,j});
				}
			}
		}
		return output;
    }




int AlgorithmMatrix::foundAWin(int &i, int &j, int const& iCapturated, int const& jCapturated, int&AmountOfEqualToken, int const& winCondition, bool& moveToNextWinControl)
{
	int tokenValue = (int) Token_values::EMPTY_TOKEN;
	if (AmountOfEqualToken==winCondition){
		tokenValue = matrix[i][j];
	}
	i = iCapturated;
	j = jCapturated;
	moveToNextWinControl=false;
	AmountOfEqualToken=1;
	return tokenValue;
}



    int AlgorithmMatrix::trivialGameOver(){																							//Its works with a trivial value of win Condition.
		bool fullBoard = true; //We asume the Tie Game Over.
		bool moveToNextWinControl= false;
		int token = (int)Token_values::EMPTY_TOKEN; //If token gets a value that is diferent of the empty token, a victory has cocurred!
		for (int i = 0 ; i < width ; i++ )
		{
			for (int j = 0 ; j < height ; j++)
			{
				int iCapturated = i; int jCapturated= j; int amountOfEqualToken=1;
				if (i + winCondition <=width){  // If we have the minimun amount of tiles to check, then the algorithm will proceed to  check the actual row.
					while (!moveToNextWinControl && amountOfEqualToken < winCondition && i + 1 <width){
						if (matrix[i][j] == matrix[i+1][j] && matrix[i][j] != (int)Token_values::EMPTY_TOKEN){
							amountOfEqualToken++;
							i++;
						}else{
							moveToNextWinControl = true;
						}
					}
					token = foundAWin(i,j,iCapturated,jCapturated,amountOfEqualToken,winCondition,moveToNextWinControl);
				}
				if (j+ winCondition <= height && token==(int)Token_values::EMPTY_TOKEN){ //Now, we also ask that if there is no victory yet. Column check
					while (!moveToNextWinControl && amountOfEqualToken<winCondition && j + 1 < height){
						if (matrix[i][j] == matrix[i][j+1] && matrix[i][j] != (int) Token_values::EMPTY_TOKEN){
							amountOfEqualToken++;
							j++;
						}else{
							moveToNextWinControl = true;
						}
					}
					token = foundAWin(i,j,iCapturated,jCapturated,amountOfEqualToken,winCondition,moveToNextWinControl);
				}
				if (j + winCondition <= height && i + winCondition <= width && token==(int)Token_values::EMPTY_TOKEN){  //We check the 2 possible diagonals wins. In this case Q1  to  Q4.
					j = j + winCondition - 1;
					while (!moveToNextWinControl && amountOfEqualToken<winCondition && j > 0 && i + 1 < width){ //Increasing Diagonal.
						if (matrix[i][j] == matrix[i+1][j-1] && matrix[i][j]!=(int)Token_values::EMPTY_TOKEN){
							amountOfEqualToken++;
							j--; i++;
						}else{
							moveToNextWinControl = true;
						}
					}
					token = foundAWin(i,j,iCapturated,jCapturated,amountOfEqualToken,winCondition,moveToNextWinControl);
					while (!moveToNextWinControl && amountOfEqualToken < winCondition && j + 1 < height && i + 1 < width && token==(int)Token_values::EMPTY_TOKEN){//Decreasing Diagonal (Q2 to Q4).
						if (matrix[i][j] == matrix[i+1][j+1] && matrix[i][j]!=(int)Token_values::EMPTY_TOKEN){
							amountOfEqualToken++;
							j++; i++;
						}else{
							moveToNextWinControl = true;
						}
					}
					token = foundAWin(i,j,iCapturated,jCapturated,amountOfEqualToken,winCondition,moveToNextWinControl);
				}

				if ((matrix[i][j]) == (int)Token_values::EMPTY_TOKEN){ //If we found at least one tile that isn't empty, then there is no tie yet!.
					fullBoard=false;
				}
				if (token!=(int)Token_values::EMPTY_TOKEN){ //We found a victory!
					return token;
				}
			}

		}
		if (fullBoard){ 
			return (int) Game_Over_Values::PLAYERS_TIE; //In this case, we get a Tie Game Over. 
		}else{
			return (int) Game_Over_Values::THE_GAME_IS_NOT_OVER_YET; //If there is, at least, one tile empty, the game will be not over yet.
		}


    }

}