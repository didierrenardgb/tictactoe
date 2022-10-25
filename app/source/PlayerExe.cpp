#include "PlayerExe.h"
#include "Board.h"
#include "Coordinates.h"
#include "AlgorithmMatrix.h"
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
};


namespace ttt
{
	static const int DEPTH_VALUE = 5; //The value of depth that will be use in our algorithm.




//The board is passed as a parameter to know its dimensions. Thanks to that, we can use pointer notations in a simplified way, knowing the dimensions of the structure.



/*The semantics of the values ​​to obtain
	If the game isn't over, it will return the depth value. Bigger the value, bigger the odds to find a victory path.
	We add to the win and defeat value the depth value, to ashure that iven the worst win will be always better that any partial state.
		If the actual player wins, the function will return the value (10+depth) minus the amount of levels of depth that we generated.
		If the actual player looses, the function will return the value (-10-depth) plus the amount of levels of depth that we generated.
	If there is a tie, it will return 0.*/


	static int calculateValue(int const& gameOverStatus, bool const&isActualPlayer, int const& depth)
	{
		int heuristicValue;
		switch (gameOverStatus)
		{
			case 1:
				heuristicValue =((int) Heuristic_values::WIN + depth)  - (DEPTH_VALUE - depth); //
			break;
			case 2:
				heuristicValue =((int) Heuristic_values::DEFEAT - depth) + (DEPTH_VALUE - depth);
			break;
			case 3:
				heuristicValue =(int) Heuristic_values::TIE;
			break;
			default:	
				heuristicValue = depth;
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


//This function checks that if the amount of equal token found in the board its the same as the win condition.
//Also, its initialice all the variables needed for the trivialGameOver.






	static int alpha_beta(int depth, int alpha, int beta, bool actualPlayer,AlgorithmMatrix matrix)
		{
		std::vector<Coordinates> movs = matrix.getMovs();
		bool pruning =false;
		int gameOverValue = matrix.trivialGameOver();
		if ((depth == 0) || gameOverValue>0){
			return calculateValue(gameOverValue,actualPlayer,depth);
		}
		else{
			std::vector<Coordinates> movs = matrix.getMovs(); //We get all the available movements in the board.
			if (actualPlayer)
			{
				int value =-std::numeric_limits<int>::max();
				for (Coordinates candidate : movs){ 
					if (!pruning)
					{
						matrix.executeMovement(candidate,actualPlayer); //we execute the movement.
						value = std::max(value,alpha_beta(depth-1,alpha,beta,!actualPlayer,matrix)); //we search the maximun value for the current player.
						alpha = std::max(alpha,value);
						if (alpha>=beta){ //if we already found a better movement.
							pruning = true; //we dont need to keep analizing more posible states in this branch.
						}
						matrix.undoMovement(candidate); //We undo the movement.
					}
				}
				return value;
			} 
			else{
				int value =std::numeric_limits<int>::max();
				for (Coordinates candidate : movs){ 
					if (!pruning){
						matrix.executeMovement(candidate,actualPlayer);
						value = std::min(value,alpha_beta(depth-1,alpha,beta,!actualPlayer,matrix)); //we search the lesser defeat for the current player.
						beta = std::min(beta,value);
						if (beta<=alpha){
							pruning=true;
						}
						matrix.undoMovement(candidate); //We undo the movement.
					}
				}
				return value;
			}
		}
	}


	static Coordinates play(Board const& board,AlgorithmMatrix matrix)  //Function that obtains the final move to make.
	{
		Coordinates coords{0,0};
		if (board.valid()){
			std::vector<Coordinates> movs = matrix.getMovs();
			bool actualPlayer = true; //It carries the context of which player is about to put a token. If it is true, then is the player who first called the method.
									   //Otherwise, it's your rival.
			int alpha = -std::numeric_limits<int>::max(); //-Infinite
			int beta = std::numeric_limits<int>::max(); //Infinite.
			int best_alpha = alpha;
			for (Coordinates candidate : movs){ 
				matrix.executeMovement(candidate,actualPlayer); //We consume a tile.
				alpha=std::max(alpha,alpha_beta(DEPTH_VALUE, alpha,beta,!actualPlayer,matrix));
				if (alpha > best_alpha){
					best_alpha = alpha;
					coords = candidate;
				}
				matrix.undoMovement(candidate); //We undo the movement.
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



	//PlayerExe.h Implementations.

	PlayerExe::PlayerExe(std::string const& name):mName(name){
	}

	std::string const& PlayerExe::name() const{
		return mName;
	}

	Coordinates PlayerExe::play(Board const& board) const{
		AlgorithmMatrix matrix(board,name()); 
		return ttt::play(board,matrix);
	}

} // namespace ttt