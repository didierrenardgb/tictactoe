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

//Constants Declarations.


//Heuristic values of each posible state.
enum class Heuristic_values{
	TIE=0,
	WIN=10,
	DEFEAT=-10,
	INDIFERENT=-5
};

//Token values on the board.
enum class Token_values{
	EMPTY_TOKEN=0,
	CURRENT_PLAYER_TOKEN,
	RIVAL_TOKEN
};

//Id's of each posible state.
enum class Game_Over_Values{
	THE_GAME_IS_NOT_OVER_YET=0,
	CURENT_PLAYER_WINS,
	CURRENT_PLAYER_DEFEATED,
	PLAYERS_TIE	
};


namespace ttt
{
	static const int DEPTH_VALUE = 5; //The value of depth that will be use in our algorithm.


	//Prints the current state of the board  -tracking meanings only-.
	static void printBoardState(Board const& board,std::vector<std::vector<int>>& algorithmMatrix){
		for (int i = 0 ; i < board.width() ; i++){
			for (int j = 0 ; j < board.height() ; j++){
				std::cout<<"{ "<<i<<" , "<<j<<" }: "<<algorithmMatrix[i][j]<<" | ";
			}
			std::cout<<std::endl;
		}
		std::cout<<std::endl;std::cout<<std::endl;std::cout<<std::endl;
	}

	static void executeMovement(std::vector<std::vector<int>> & algorithmMatrix, Coordinates const& c, bool const& actualPlayer) //Function that execute a movement.
	{
		if (actualPlayer){
			algorithmMatrix[c.x][c.y] = (int)Token_values::CURRENT_PLAYER_TOKEN;}
		else{
			algorithmMatrix[c.x][c.y] = (int) Token_values::RIVAL_TOKEN;}
	}

//The board is passed as a parameter to know its dimensions. Thanks to that, we can use pointer notations in a simplified way, knowing the dimensions of the structure.

	static std::vector<Coordinates> getMovs(Board const& board,std::vector<std::vector<int>> & algorithmMatrix) //Obtenemos todos los posibles movimientos a realizar del tablero.
	{
		std::vector<Coordinates> output;
		output.clear();
		for (int i = 0 ; i < board.width() ; i++){
			for (int j = 0 ; j < board.height() ; j++){
				if (algorithmMatrix[i][j]==0){
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


	static int calculateValue(Board const& board, std::vector<std::vector<int>>& algorithmMatrix, int const& gameOverStatus, bool const&isActualPlayer)
	{
		int heuristicValue;
		switch (gameOverStatus)
		{
			case 1:
				heuristicValue =(int) Heuristic_values::WIN;
			break;
			case 2:
				heuristicValue =(int) Heuristic_values::DEFEAT;
			break;
			case 3:
				heuristicValue =(int) Heuristic_values::TIE;
			break;
			default:
				//srand(time(0));                                //We can choose to calculate the heuristic value of partial states randomly, to increments the odds to not get a tie 
   				//heuristicValue = 1 + (rand()%10);              //between two instances of the same IAs. Also, this will increase the odds to prune more branches.
				heuristicValue =(int) Heuristic_values::INDIFERENT;
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


static int trivialGameOver(Board const& board, std::vector<std::vector<int>>& algorithmMatrix) //This function can recognize if we are in a victory state.
	{																							//Its works with a trivial value of win Condition.
		bool fullBoard = true; //We asume the Tie Game Over.
		int width = board.width();
		int height = board.height();
		int winCondition = board.winCondition();
		bool nextControl= false;
		int token = (int)Token_values::EMPTY_TOKEN; //If token gets a value that is diferent of the empty token, a victory has cocurred!

		for (int i = 0 ; i < width ; i++ )
		{
			for (int j = 0 ; j < height ; j++)
			{
				int xCapturated = i; int yCapturated= j; int amountOfEqualToken=0;

				// R O W    C O N T R O L .
				
				if (i + winCondition <=width){  //we check the actual row.
					while (!nextControl && amountOfEqualToken < winCondition && i + 1 <width){
						if (algorithmMatrix[i][j] == algorithmMatrix[i+1][j] && algorithmMatrix[i][j] != (int)Token_values::EMPTY_TOKEN){
							amountOfEqualToken++;
							i++;
						}else{
							nextControl = true;
						}
					}
					i = xCapturated;
					if (!nextControl){ 
						token = algorithmMatrix[i][j];
					}
					nextControl=false; amountOfEqualToken=0;
				}

				// C O L U M N    C O N T R O L .


				if (j+ winCondition <= height && token==(int)Token_values::EMPTY_TOKEN){ //We check the actual column
					while (!nextControl && amountOfEqualToken<winCondition && j + 1 < height){
						if (algorithmMatrix[i][j] == algorithmMatrix[i][j+1] && algorithmMatrix[i][j] != (int) Token_values::EMPTY_TOKEN){
							amountOfEqualToken++;
							j++;
						}else{
							nextControl = true;
						}
					}
					j = yCapturated; 
					if (!nextControl){
						token = algorithmMatrix[i][j];
					}
					nextControl=false; amountOfEqualToken=0;
					}

				// D I A G O N A L  --> Q 1  t o  Q 3

				
				if (j + winCondition <= height && i + winCondition <= width && token==(int)Token_values::EMPTY_TOKEN){  //We check the 2 possible diagonals wins.
					j = j + winCondition - 1;
					while (!nextControl && amountOfEqualToken<winCondition && j > 0 && i + 1 < width){ //Increasing Diagonal.
						if (algorithmMatrix[i][j] == algorithmMatrix[i+1][j-1] && algorithmMatrix[i][j]!=(int)Token_values::EMPTY_TOKEN){
							amountOfEqualToken++;
							j--; i++;
						}else{
							nextControl = true;
						}
					}
					i = xCapturated; j = yCapturated;
					if (!nextControl){
						token = algorithmMatrix[i+1][j + winCondition-1];
					}
					nextControl=false; amountOfEqualToken=0;

					// D I A G O N A L  --> Q 2  t o  Q 4

					while (!nextControl && amountOfEqualToken < winCondition && j + 1 < height && i + 1 < width){//Decreasing Diagonal.
						if (algorithmMatrix[i][j] == algorithmMatrix[i+1][j+1] && algorithmMatrix[i][j]!=(int)Token_values::EMPTY_TOKEN){
							amountOfEqualToken++;
							j++; 
							i++;
						}else{
							nextControl = true;
						}
					}
					i = xCapturated; j = yCapturated;
					if (!nextControl){
						token = algorithmMatrix[i][j];
					}
				}

				if ((algorithmMatrix[i][j]) == (int)Token_values::EMPTY_TOKEN){
					fullBoard=false;
				}
				if (token!=(int)Token_values::EMPTY_TOKEN) //We found a victory!
				{
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

	static int alpha_beta(int depth, int alpha, int beta, bool actualPlayer, Board const& board, std::vector<std::vector<int>>& algorithmMatrix)
		{
		std::vector<Coordinates> movs = getMovs(board,algorithmMatrix);
		bool pruning =false;
		int gameOverValue =trivialGameOver(board,algorithmMatrix);
		if ((depth == 0) || gameOverValue>0){
			return calculateValue(board,algorithmMatrix,gameOverValue,actualPlayer);
		}
		else{
			std::vector<Coordinates> movs = getMovs(board,algorithmMatrix); //We get all the available movements in the board.
			if (actualPlayer)
			{
				int value =-std::numeric_limits<int>::max();
				for (Coordinates candidate : movs){ 
					if (!pruning)
					{
						executeMovement(algorithmMatrix,candidate,actualPlayer); //we execute the movement.
						value = std::max(value,alpha_beta(depth-1,alpha,beta,!actualPlayer,board,algorithmMatrix)); //we search the maximun value for the current player.
						alpha = std::max(alpha,value);
						if (alpha>=beta){ //if we already found a better movement.
							pruning = true; //we dont need to keep analizing more posible states in this branch.
						}
						algorithmMatrix[candidate.x][candidate.y] = (int) Token_values::EMPTY_TOKEN; //We undo the movement.
					}
				}
				return value;
			} 
			else{
				int value =std::numeric_limits<int>::max();
				for (Coordinates candidate : movs){ 
					if (!pruning){
						executeMovement(algorithmMatrix,candidate,actualPlayer);
						value = std::min(value,alpha_beta(depth-1,alpha,beta,!actualPlayer,board,algorithmMatrix)); //we search the lesser defeat for the current player.
						beta = std::min(beta,value);
						if (beta<=alpha){
							pruning=true;
						}
						algorithmMatrix[candidate.x][candidate.y] =(int) Token_values::EMPTY_TOKEN; //Undo the last movement.
					}
				}
				return value;
			}
		}
	}


	static Coordinates play(Board const& board, std::vector<std::vector<int>>& algorithmMatrix)  //Function that obtains the final move to make.
	{
		Coordinates coords{0,0};
		if (board.valid()){
			std::vector<Coordinates> movs = getMovs(board,algorithmMatrix);
			bool actualPlayer = true; //It carries the context of which player is about to put a token. If it is true, then is the player who first called the method.
									   //Otherwise, it's your rival.
			int alpha = -std::numeric_limits<int>::max(); //-Infinite
			int beta = std::numeric_limits<int>::max(); //Infinite.
			int best_alpha = alpha;
			for (Coordinates candidate : movs){ 
				executeMovement(algorithmMatrix,candidate,actualPlayer); //We consume a tile.
				alpha=std::max(alpha,alpha_beta(DEPTH_VALUE, alpha,beta,!actualPlayer, board,algorithmMatrix));
				if (alpha > best_alpha){
					best_alpha = alpha;
					coords = candidate;
				}
				algorithmMatrix[candidate.x][candidate.y] = (int) Token_values::EMPTY_TOKEN; //Undo the last movement.
			}
			movs.clear();
		}
		return coords;
	}

/* We capture the game matrix in its current state with a reduced semantic, only for algorithm propourse.
   algorithmMatrix will the the pointer to the dynamic matrix that carries our board game.
   Token semantic:
		//1 --> Actual Player (the one that invokes the function in the first place).
		//0 --> Rival Player.
		//2 --> Empy Tile.*/

	static std::vector<std::vector<int>> initializeAlphaBeta(Board const& board, std::string const& name)
	{
		std::vector<std::vector<int>> algorithmMatrix(board.width(),std::vector<int>(board.height()));
		for (int i = 0 ; i < board.width() ; i++){
			for (int j = 0 ; j < board.height() ; j++){
				Coordinates c{i,j};
				algorithmMatrix[i][j] = (int) Token_values::EMPTY_TOKEN; //I Asume that the tile is empty.
				if (board.tile(c)->owner().has_value()){
					if (name == board.tile(c)->owner()->get().name()){
						algorithmMatrix[i][j] = (int) Token_values::CURRENT_PLAYER_TOKEN;	
					}
					else{
						algorithmMatrix[i][j] = (int) Token_values::RIVAL_TOKEN;	
					}
				}
			}
		}
		return algorithmMatrix;
	}

	//PlayerExe.h Implementations.

	PlayerExe::PlayerExe(std::string const& name):mName(name){
	}

	std::string const& PlayerExe::name() const{
		return mName;
	}

	Coordinates PlayerExe::play(Board const& board) const{
		std::vector<std::vector<int>> algorithmMatrix = ttt::initializeAlphaBeta(board,name()); 
		return ttt::play(board,algorithmMatrix);
	}

} // namespace ttt