#include "AlgorithmMatrix.h"
#include "Coordinates.h"
#include "Tile.h"
#include <vector>


enum class TokenValues{
	emptyToken=0,
	currentPlayerToken,
	rivalToken
};

enum class GameOverValues{
	theGameIsNotOverYet=0,
	currentPlayerWins,
	currentPlayerDefeated,
	playersTie	
};


namespace ttt
{


/* We capture the game matrix in its current state with a reduced semantic, only for algorithm propourse.
   algorithmMatrix will the the pointer to the dynamic matrix that carries our board game.
   Token semantic:
		//1 --> Actual Player (the one that invokes the function in the first place).
		//0 --> Rival Player.
		//2 --> Empy Tile.*/


	AlgorithmMatrix::AlgorithmMatrix(Board const& board, std::string playerName){
        mWidth = board.width();
        mHeight = board.height();
        mWinCondition = board.winCondition();

        mMatrix.resize(mWidth,std::vector<int>(board.height()));
		for (int i = 0 ; i < mWidth ; i++){
			for (int j = 0 ; j < mHeight ; j++){
				Coordinates c{i,j};
				mMatrix[i][j] = (int) TokenValues::emptyToken; //I Asume that the tile is empty.
				if (board.tile(c)->owner().has_value()){
					if (playerName == board.tile(c)->owner()->get().name()){
						mMatrix[i][j] = (int) TokenValues::currentPlayerToken;	
					}
					else{
						mMatrix[i][j] = (int) TokenValues::rivalToken;	
					}
				}
			}
		}
    }

	
    int AlgorithmMatrix::getWidth() const{
        return mWidth;
    } 

    int AlgorithmMatrix::getHeight() const{
        return mHeight;
    } 


    void AlgorithmMatrix::executeMovement(Coordinates const& c, bool const& actualPlayer){ //Function that execute a movement.
        if (actualPlayer){
			mMatrix[c.x][c.y] = (int)TokenValues::currentPlayerToken;}
		else{
			mMatrix[c.x][c.y] = (int) TokenValues::rivalToken;}
    }

    void AlgorithmMatrix::undoMovement(Coordinates const& c){ //Function that undo a movement.
			mMatrix[c.x][c.y] = (int)TokenValues::emptyToken;
    }


    std::vector<Coordinates> AlgorithmMatrix::getMovs(){ //We get all the posible moves to make in the board.
        std::vector<Coordinates> output;
		output.clear();
		for (int i = 0 ; i < mWidth ; i++){
			for (int j = 0 ; j < mHeight ; j++){
				if (mMatrix[i][j]==(int)TokenValues::emptyToken){
					output.push_back(Coordinates{i,j});
				}
			}
		}
		return output;
    }


//Private void that detects if a victory occurred. In addition, it initializes all the variables of trivialGameOver.

void AlgorithmMatrix::foundAWin(int & token, int &i, int &j, int const& iCapturated, int const& jCapturated, int& amountOfEqualToken, bool& moveToNextWinControl)
{
	if (amountOfEqualToken==mWinCondition){
		token = mMatrix[i][j];
	}
	moveToNextWinControl=false;
	amountOfEqualToken=1;	
	i = iCapturated;
	j = jCapturated;
}


/*The semantics of the values ​​to obtain
	If the game isn't over, it will return 0.
	If the actual player wins, the function will return the value 1.
	If the actual player looses, the function will return the value 2.
	If there is a tie, it will return 3.*/


//Function that seeks a victory. 
    int AlgorithmMatrix::trivialGameOver(){		
		bool fullBoard = true; //We asume the Tie Game Over.
		bool moveToNextWinControl= false;
		int token = (int)TokenValues::emptyToken; //If token gets a value that is diferent of the empty token, a victory has cocurred!
		for (int i = 0 ; i < mWidth ; i++ )
		{
			for (int j = 0 ; j < mHeight ; j++)
			{
				int iCapturated = i; int jCapturated= j; int amountOfEqualToken=1;
				if (i + mWinCondition <=mWidth){  // If we have the minimun amount of tiles to check, then the algorithm will proceed to  check the actual row.
					while (!moveToNextWinControl && amountOfEqualToken < mWinCondition && i + 1 <mWidth){
						if (mMatrix[i][j] == mMatrix[i+1][j] && mMatrix[i][j] != (int)TokenValues::emptyToken){
							amountOfEqualToken++;
							i++;
						}else{
							moveToNextWinControl = true; //If this is true, then on this path there is no longer a possible victory. We must continue to the next control.
						}
					}
					foundAWin(token,i,j,iCapturated,jCapturated,amountOfEqualToken,moveToNextWinControl);
				}
				if (j+ mWinCondition <= mHeight && token==(int)TokenValues::emptyToken){ //Now, we also ask that if there is no victory yet. Column check
					while (!moveToNextWinControl && amountOfEqualToken<mWinCondition && j + 1 < mHeight){
						if (mMatrix[i][j] == mMatrix[i][j+1] && mMatrix[i][j] != (int) TokenValues::emptyToken){
							amountOfEqualToken++;
							j++;
						}else{
							moveToNextWinControl = true;
						}
					}
					foundAWin(token,i,j,iCapturated,jCapturated,amountOfEqualToken,moveToNextWinControl);
				}
				if (j + mWinCondition <= mHeight && i + mWinCondition <= mWidth && token==(int)TokenValues::emptyToken){  //We check the 2 possible diagonals wins. In this case Q1  to  Q4.
					j = j + mWinCondition - 1;
					while (!moveToNextWinControl && amountOfEqualToken<mWinCondition && j > 0 && i + 1 < mWidth){ //Increasing Diagonal.
						if (mMatrix[i][j] == mMatrix[i+1][j-1] && mMatrix[i][j]!=(int)TokenValues::emptyToken){
							amountOfEqualToken++;
							j--; i++;
						}else{
							moveToNextWinControl = true;
						}
					} 
					foundAWin(token,i,j,iCapturated,jCapturated,amountOfEqualToken,moveToNextWinControl);
					while (!moveToNextWinControl && amountOfEqualToken < mWinCondition && j + 1 < mHeight && i + 1 < mWidth && token==(int)TokenValues::emptyToken){//Decreasing Diagonal (Q2 to Q4).
						if (mMatrix[i][j] == mMatrix[i+1][j+1] && mMatrix[i][j]!=(int)TokenValues::emptyToken){
							amountOfEqualToken++;
							j++; i++;
						}else{
							moveToNextWinControl = true;
						}
					}
					foundAWin(token,i,j,iCapturated,jCapturated,amountOfEqualToken,moveToNextWinControl);
				}
				if ((mMatrix[i][j]) == (int)TokenValues::emptyToken){ //If we found at least one tile that isn't empty, then there is no tie yet!.
					fullBoard=false;
				}
				if (token!=(int)TokenValues::emptyToken){ //We found a victory!
					return token;
				}
			}

		}
		if (fullBoard){ 
			return (int) GameOverValues::playersTie; //In this case, we get a Tie Game Over. 
		}else{
			return (int) GameOverValues::theGameIsNotOverYet; //If there is, at least, one tile empty, the game will be not over yet.
		}
    }
	



}