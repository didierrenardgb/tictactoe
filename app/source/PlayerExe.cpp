#include "PlayerExe.h"
#include "Board.h"
#include "Coordinates.h"
#include "Tile.h"
#include <iostream>
#include <random>
#include <vector>
#include <algorithm> 
#include <ctime>
#include <cstdlib>

namespace ttt
{
	static const int DEPTH_VALUE = 5; //The value of depth that will be use in our algorithm.


	//Prints the current state of the board  -tracking meanings only-.
	static void printBoardState(Board const& board,int ** boardManagement){
		for (int i = 0 ; i < board.width() ; i++){
			for (int j = 0 ; j < board.height() ; j++){
				std::cout<<"{ "<<i<<" , "<<j<<" }: "<<boardManagement[i][j]<<" | ";
			}
			std::cout<<std::endl;
		}
		std::cout<<std::endl;std::cout<<std::endl;std::cout<<std::endl;
	}

	static void executeMovement(int ** boardManagement, Coordinates c, bool actualPlayer) //Function that execute a movement.
	{
		if (actualPlayer){
			boardManagement[c.x][c.y] = 1;}
		else{
			boardManagement[c.x][c.y] = 2;}
	}

//The board is passed as a parameter to know its dimensions. Thanks to that, we can use pointer notations in a simplified way, knowing the dimensions of the structure.

	static std::vector<Coordinates> getMovs(Board const& board,int ** boardManagement) //Obtenemos todos los posibles movimientos a realizar del tablero.
	{
		std::vector<Coordinates> output;
		output.clear();
		for (int i = 0 ; i < board.width() ; i++){
			for (int j = 0 ; j < board.height() ; j++){
				if (boardManagement[i][j]==0){
					output.push_back(Coordinates{i,j});
				}
			}
		}
		return output;
	}


/*The semantics of the values ​​to obtain
	If the game isn't over, it will return a heuristic value between -9 to -1 or  1 to 9, depending on the player that is playing. Could be 5 or randomized.
	If the actual player wins, the function will return the value 10.
	If the actual player looses, the function will return the value -10.
	If there is a tie, it will return 0.*/

	static int calculateValue(Board const& board, int ** boardManagement, int gameOverStatus, bool isActualPlayer)
	{
		int heuristicValue = 0;
		switch (gameOverStatus)
		{
			case 1:
				heuristicValue = 10;
			break;
			case 2:
				heuristicValue = -10;
			break;
			case 3:
				heuristicValue = 0;
			break;
			default:
				//srand(time(0));                                //We can choose to calculate the heuristic value of partial states randomly, to increments the odds to not get a tie. 
   				//heuristicValue = 1 + (rand()%10);              //between two instances of the same IAs. Also, this will increase the odds to prune more branches.
				heuristicValue = 5;
				if (!isActualPlayer){
					heuristicValue = - heuristicValue;
				}
			break;
		}
		return heuristicValue;
	}


/*The semantics of the values ​​to obtain
	If the game isn't over, it will return 0.
	If the actual player wins, the function will return the value 1.
	If the actual player looses, the function will return the value 2.
	If there is a tie, it will return 3.*/


static int trivialGameOver(Board const& board, int ** boardManagement) //This function can recognize if we are in a victory state.
	{																	//Its works with a trivial value of win Condition.

		bool fullBoard = true; //We asume the Tie Game Over.
		int width = board.width();
		int height = board.height();
		int winCondition = board.winCondition();
		bool notWinStatus= false;

		for (int i = 0 ; i < width ; i++ )
		{
			for (int j = 0 ; j < height ; j++)
			{
				int originalI = i; int originalJ= j; int amountOfEqualToken=0;

				if ((boardManagement[i][j])==0){
					fullBoard=false;
				}


				if (i+winCondition<=width){  //we check the actual row.
					while (!notWinStatus && amountOfEqualToken<winCondition && i+1<width){
						if (boardManagement[i][j] == boardManagement[i+1][j] && boardManagement[i][j]!=0){
							amountOfEqualToken++;
							i++;
						}else{
							notWinStatus = true;
						}
					}
					i = originalI;
					if (!notWinStatus){ 
						return boardManagement[i][j];
					}
					notWinStatus=false;
					amountOfEqualToken=0;
				}


				if (j+(winCondition)<=height){ //We check the actual column
					while (!notWinStatus && amountOfEqualToken<winCondition && j+1<height){
						if (boardManagement[i][j] == boardManagement[i][j+1] && boardManagement[i][j]!=0){
							amountOfEqualToken++;
							j++;
						}else{
							notWinStatus = true;
						}
					}
					j = originalJ; 
					if (!notWinStatus){
						return boardManagement[i][j];
					}
					notWinStatus=false;
					amountOfEqualToken=0;
					}
					
				if (j+(winCondition)<=height && i+(winCondition) <=width){  //We check the 2 possible diagonals wins.
					j = j + winCondition-1;
					while (!notWinStatus && amountOfEqualToken<winCondition && j>0 && i+1<width){ //Increasing Diagonal.
						if (boardManagement[i][j] == boardManagement[i+1][j-1] && boardManagement[i][j]!=0){
							amountOfEqualToken++;
							j--; 
							i++;
						}else{
							notWinStatus = true;
						}
					}
					i = originalI; j = originalJ;
					if (!notWinStatus){
						return boardManagement[i+1][j + winCondition-1];
					}
					notWinStatus=false;
					amountOfEqualToken=0;

					
					while (!notWinStatus && amountOfEqualToken<winCondition && j+1<height && i+1<width){//Decreasing Diagonal.
						if (boardManagement[i][j] == boardManagement[i+1][j+1] && boardManagement[i][j]!=0){
							amountOfEqualToken++;
							j++; 
							i++;
						}else{
							notWinStatus = true;
						}
					}
					i = originalI; j = originalJ;
					if (!notWinStatus){
						return boardManagement[i][j];
					}
				}
			}

		}
		if (fullBoard){ 
			return 3; //In this case, we get a Tie Game Over. 
		}else{
			return 0; //If there is, at least, one tile empty, the game will be not over yet.
		}
	}


	static int alpha_beta(int depth, int alpha, int beta, bool actualPlayer, Board const& board, int ** boardManagement)
	{
		std::vector<Coordinates> movs = getMovs(board,boardManagement);
		bool pruning =false;
		int gameOverValue =trivialGameOver(board,boardManagement);
		if ((depth == 0) || gameOverValue>0){
			return calculateValue(board,boardManagement,gameOverValue,actualPlayer);
		}
		else{
			std::vector<Coordinates> movs = getMovs(board,boardManagement); //We get all the available movements in the board.
			if (actualPlayer)
			{
				int value =-std::numeric_limits<int>::max();
				for (Coordinates candidate : movs){ 
					if (!pruning)
					{
						executeMovement(boardManagement,candidate,actualPlayer); //we execute the movement.
						value = std::max(value,alpha_beta(depth-1,alpha,beta,!actualPlayer,board,boardManagement)); //we search the maximun value for the current player.
						alpha = std::max(alpha,value);
						if (alpha>=beta){ //if we already found a better movement.
							pruning = true; //we dont need to keep analizing more posible states in this branch.
						}
						boardManagement[candidate.x][candidate.y] = 0; //We undo the movement.
					}
				}
				return value;
			} 
			else{
				int value =std::numeric_limits<int>::max();
				for (Coordinates candidate : movs){ 
					if (!pruning){
						executeMovement(boardManagement,candidate,actualPlayer);
						value = std::min(value,alpha_beta(depth-1,alpha,beta,!actualPlayer,board,boardManagement)); //we search the lesser defeat for the current player.
						beta = std::min(beta,value);
						if (beta<=alpha){
							pruning=true;
						}
						boardManagement[candidate.x][candidate.y] = 0; //Undo the last movement.
					}
				}
				return value;
			}
		}
	}


	static Coordinates play(Board const& board, int ** boardManagement, std::string name)  //Function that obtains the final move to make.
	{
		Coordinates coords{0,0};
		if (board.valid()){
			std::vector<Coordinates> movs = getMovs(board,boardManagement);

			bool actualPlayer = true; //It carries the context of which player is about to put a token. If it is true, then is the player who first called the method.
									   //Otherwise, it's your rival.
			int alpha = -std::numeric_limits<int>::max(); //-Infinite
			int beta = std::numeric_limits<int>::max(); //Infinite.
			int best_alpha = alpha;
			for (Coordinates candidate : movs){ 
				executeMovement(boardManagement,candidate,actualPlayer); //We consume a tile.
				alpha=std::max(alpha,alpha_beta(DEPTH_VALUE, alpha,beta,!actualPlayer, board,boardManagement));
				if (alpha > best_alpha){
					best_alpha = alpha;
					coords = candidate;
				}
				boardManagement[candidate.x][candidate.y] = 0; //Undo the last movement.
			}
			movs.clear();
		}
		for (int i = 0; i < board.width(); ++i){  //We free the memory allocated by the matrix.
   	 		delete [] boardManagement[i];
		}
		delete [] boardManagement;
		return coords;
	}




/* We capture the game matrix in its current state with a reduced semantic, only for algorithm propourse.
   boardManagement will the the pointer to the dynamic matrix that carries our board game.

   Token semantic:
		//1 --> Actual Player (the one that invokes the function in the first place).
		//0 --> Rival Player.
		//2 --> Empy Tile.*/

	static int** initializeAlphaBeta(Board const& board, std::string const& name)
	{
		int  ** boardManagement = new int *[board.width()];

		for (int i = 0 ; i < board.width() ; i++){
			boardManagement[i] = new int[board.height()];
			for (int j = 0 ; j < board.height() ; j++){
				Coordinates c{i,j};
				if (board.tile(c)->owner().has_value()){
					if (name == board.tile(c)->owner()->get().name()){
						boardManagement[i][j] = 1;	
					}
					else{
						boardManagement[i][j] = 2;	
					}
				}
				else{
					boardManagement[i][j] = 0;
				}

			}
		}
		return boardManagement;
	}

	PlayerExe::PlayerExe(std::string const& name):mName(name){
	}

	std::string const& PlayerExe::name() const{
		return mName;
	}

	Coordinates PlayerExe::play(Board const& board) const{
		int  ** boardManagement = ttt::initializeAlphaBeta(board,name()); 
		return ttt::play(board,boardManagement,name());
	}

} // namespace ttt



