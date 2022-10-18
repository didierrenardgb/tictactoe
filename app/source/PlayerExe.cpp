#include "PlayerExe.h"
#include "Board.h"
#include "Coordinates.h"
#include "Tile.h"
#include <iostream>
#include <random>
#include <vector>
#include <algorithm> 

namespace ttt
{
	static const int DEPTH_VALUE = 7; 


	//Prints the current state of the board, only for tracing meanings.
	static void printBoardState(Board const& board,int ** boardManagement){
		std::cout<<"BUILD"<<std::endl;
		for (int i = 0 ; i < board.width() ; i++){
			for (int j = 0 ; j < board.height() ; j++){
				std::cout<<"{ "<<i<<" , "<<j<<" }: "<<boardManagement[i][j]<<" | ";
			}
			std::cout<<std::endl;
		}
		std::cout<<std::endl;std::cout<<std::endl;std::cout<<std::endl;
	}

	static void executeMovement(int ** boardManagement, Coordinates c, bool actualPlayer)
	{
		if (actualPlayer){
			boardManagement[c.x][c.y] = 1;}
		else{
			boardManagement[c.x][c.y] = 2;}
	}

//The board is passed as a parameter to know its dimensions. Thanks to that i can use pointer notations in a simplified way, knowing the dimensions of the structure.

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
	If the game isn't over, it will return 5.
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

	static int gameOver(Board const& board, int ** boardManagement)
	{

		bool fullBoard = true; //We asume the Tie Game Over.
		int width = board.width();
		int height = board.height();
		for (int i = 0 ; i < width ; i++ )
		{
			for (int j = 0 ; j < height ; j++)
			{
				if ((boardManagement[i][j])==0){
					fullBoard=false;
				}
				if (i+2<width){
					if (boardManagement[i][j] == boardManagement[i+1][j] &&  boardManagement[i+1][j] == boardManagement[i+2][j] && boardManagement[i][j]!=0){
						return boardManagement[i][j];
					}
				}
				if (j+2< height){
					if (boardManagement[i][j] == boardManagement[i][j+1] && boardManagement[i][j+1] ==  boardManagement[i][j+2] && boardManagement[i][j]!=0){
						return boardManagement[i][j];
					}
				}
				if (j+2<height && i+2 < width){
					if ((boardManagement[i][j] == boardManagement[i+1][j+1] && boardManagement[i+1][j+1] == boardManagement[i+2][j+2] && boardManagement[i][j]!=0)   ||
						(boardManagement[i][j+2] == boardManagement[i+1][j+1] && boardManagement[i+1][j+1] == boardManagement[i+2][j] && boardManagement[i][j+2]!=0)) {
							return boardManagement[i+1][j+1];
						}
				}
			}
		
		}
		if (fullBoard){ 
			return 3; //If there is, at least, one tile empty, the game will be not over yet. 
		}else{
			return 0; //In this case, we get a Tie Game Over.
		}
	}


	static int alpha_beta(int depth, int alpha, int beta, bool actualPlayer, Board const& board, int ** boardManagement)
	{
		std::vector<Coordinates> movs = getMovs(board,boardManagement);
		bool pruning =false;
		int gameOverValue = gameOver(board,boardManagement);
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
						value = std::max(value,alpha_beta(depth,alpha,beta,!actualPlayer,board,boardManagement)); //we search the maximun value for the current player.
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
						value = std::min(value,alpha_beta(depth,alpha,beta,!actualPlayer,board,boardManagement)); //we search the lesser defeat for the current player.
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


	static Coordinates play(Board const& board, int ** boardManagement, std::string name)  //Obtenemos la coordenada final de movimiento.
	{
		Coordinates coords{0,0};
		if (board.valid()){
			std::vector<Coordinates> movs = getMovs(board,boardManagement);

			bool actualPlayer = true; //It carries the context of which player is about to put a token. If it is true, it is the player who first called the method.
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




/* We capture the game matrix in its current state with a reduced semantic, only for algorithms propours.
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



