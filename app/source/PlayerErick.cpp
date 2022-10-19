/***************************************************************
*                          $$$$$$$                             *
*                      $$$$$$$$$$$$$$                          *
*                   $$$$$$$$$$$$$$$$$$                         *
*                  $$$$$$$$$$$$$$$$$$$$                        *
*                 $$$$$$$$$$$$$$$O$$$$$    $$$$$$              *
*                 $$$$$$$$O$$$$$$$$$$$$   $$$  $$$             *
*                 $$$$$$$$$$$$$$$$$$$$  $$$$    $$             *
*                  $$$$$$$$$$$$$$$$$$$$$$$$                    *
*                   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$             *
*           $$$$$$     $$$$$$$$$$$$$$$$$$        $$            *
*          $$   $$$$$$$$$$$$$$$$$$$$$$$         $$             *
*           $$$    $$$$$$$$$$$$$$$$$$$$$$$$$$$$                *
*               $$$$$$$$  $$$ $$$$$$          $$               *
*             $$$$       $$$  $$$ $$$      $$$$                *
*            $$$       $$$$   $$$  $$$                         *
*             $$$$$   $$$     $$$   $$$$    $$                 *
*              $$$    $$$$$$  $$$    $$$$$$$$                  *
*                      $$$     $$$$$   $$$$                    *
*                                                              *
*                      Erick Andersson                         *
*                erick.andersson98@gmail.com                   *
*                                                              *
****************************************************************/

#include "PlayerErick.h"

#include "Board.h"
#include "Coordinates.h"
#include "Tile.h"

#include <random>

#include <iostream>

namespace ttt
{
const int pointsForWin = 5;
const int maxDepth = 4;

static int** copyBoard(Board const& board){
	int MAXR = board.height();
	int MAXC = board.width();

	int ** myBoard = new int*[MAXR];
	for(size_t r = 0; r < MAXR; r++){
		myBoard[r] = new int[MAXC];
		for(size_t c = 0; c < MAXC; c++){
			//Copy everything into an int matrix
			Coordinates coords;
			coords.x = c;
			coords.y = r;
			if(!board.tile(coords)->owner().has_value()){
				myBoard[r][c] = 0; //It's empty.
			}
			else if(board.tile(coords)->owner()->get().name() == "Erick"){
				myBoard[r][c] = 1; //It's mine.
			}
			else{
				myBoard[r][c] = -1; //It's not mine.
			}
		}
	}
	return myBoard;
}

static void printBoard(int** const& myBoard, int const& MAXR, int const& MAXC){
	std::cout << std::endl;
	for(size_t i=0; i < MAXR; i++){
		for(size_t j=0; j < MAXC; j++){
			std::cout << myBoard[i][j] << " | ";
		}
		std::cout << std::endl;
	}
}

static const int evaluate(Board const& board, int** const& myBoard, int row, int column){
	/*
	//Center
	return 7;
	//Next to one of yours
	return 1;
	//Wincon
	return 10;
	//Draw
	return 0;
	*/

	//Start of the code
	int MAXR = board.height()-1;
	int MAXC = board.width()-1;
	int toReturn = 0;

	//First turn, always right at the middle
	if((board.empty()) && (row == MAXR/2) && (column == MAXC/2)){
		return 7; //Early return, preventing useless checks
	}

	//Check if you're preventing the oponent's victory (in between the checks for wincon).

	//Check wincon for row.
	if((column == 0) && (myBoard[row][column+1] != 0)){ //Leftmost col
		//Only check two to the right
		if((myBoard[row][column+1] == myBoard[row][column+2])){
			//Check who won
			if((myBoard[row][column] == 1) && (myBoard[row][column] == myBoard[row][column+1])){
				toReturn = 10; //It was me
			}
			else if((myBoard[row][column] == -1) && (myBoard[row][column] == myBoard[row][column+1])){
				toReturn = -10; //It was the oponent
			}
			else{
				//Prevented the oponent's victory
				toReturn = (myBoard[row][column] == 1) ? 9 : -9;
			}
		}
	}
	else if((column == MAXC) && (myBoard[row][column-1] != 0)){ //Rightmost col
		//Only check two to the left
		if((myBoard[row][column-1] == myBoard[row][column-2])){
			//Check who won
			if((myBoard[row][column] == 1)&&(myBoard[row][column] == myBoard[row][column-1])){
				toReturn = 10; //It was me
			}
			else if((myBoard[row][column] == -1)&&(myBoard[row][column] == myBoard[row][column-1])){
				toReturn = -10; //It was the oponent
			}
			else{
				//Prevented the oponent's victory
				toReturn = (myBoard[row][column] == 1) ? 9 : -9;
			}
		}
	}
	else if((column != 0) && (column != MAXC)){ //Both sides now
		//One left-One right, Two left, Two right
		if((myBoard[row][column-1] != 0) && (myBoard[row][column-1] == myBoard[row][column+1])){
			//Check who won
			if((myBoard[row][column] == 1)&&(myBoard[row][column] == myBoard[row][column+1])){
				toReturn = 10; //It was me
			}
			else if((myBoard[row][column] == -1)&&(myBoard[row][column] == myBoard[row][column+1])){
				toReturn = -10; //It was the oponent
			}
			else{
				//Prevented the oponent's victory
				toReturn = (myBoard[row][column] == 1) ? 9 : -9;
			}
		}

		if((column-2 >= 0) && (myBoard[row][column-1] != 0) && (myBoard[row][column-1] == myBoard[row][column-2])){
			//Check who won
			if((myBoard[row][column] == myBoard[row][column-1]) && (myBoard[row][column] == 1)){
				toReturn = 10; //It was me
			}
			else if((myBoard[row][column] == myBoard[row][column-1]) && (myBoard[row][column] == -1)){
				toReturn = -10; //It was the oponent
			}
			else{
				//Prevented the oponent's victory
				toReturn = (myBoard[row][column] == 1) ? 9 : -9;
			}
		}

		if((column+2 <= MAXC)&&(myBoard[row][column+1] != 0)&&(myBoard[row][column+1] == myBoard[row][column+2])){
			//Check who won
			if((myBoard[row][column] == myBoard[row][column+1]) && (myBoard[row][column] == 1)){
				toReturn = 10; //It was me
			}
			else if((myBoard[row][column] == myBoard[row][column+1]) && (myBoard[row][column] == -1)){
				toReturn = -10; //It was the oponent
			}
			else{
				//Prevented the oponent's victory
				toReturn = (myBoard[row][column] == 1) ? 9 : -9;
			}
		}
	}

	//Check wincon for column.
	if((row == 0) && (myBoard[row+1][column] != 0)){ //Upmost row
		//Only check two to the bottom
		if(myBoard[row+1][column] == myBoard[row+2][column]){
			//Check who won
			if((myBoard[row][column] == 1)&&(myBoard[row][column] == myBoard[row+1][column])){
				toReturn = 10; //It was me
			}
			else if((myBoard[row][column] == -1)&&(myBoard[row][column] == myBoard[row+1][column])){
				toReturn = -10; //It was the oponent
			}
			else{
				//Prevented the oponent's victory
				toReturn = (myBoard[row][column] == 1) ? 9 : -9;
			}
		}
	}
	else if((row == MAXR) && (myBoard[row-1][column] != 0)){ //Downmost row
		//Only check two to the top
		if(myBoard[row-1][column] == myBoard[row-2][column]){
			//Check who won
			if((myBoard[row][column] == 1)&&(myBoard[row][column] == myBoard[row-1][column])){
				toReturn = 10; //It was me
			}
			else if((myBoard[row][column] == -1)&&(myBoard[row][column] == myBoard[row-1][column])){
				toReturn = -10; //It was the oponent
			}
			else{
				//Prevented the oponent's victory
				toReturn = (myBoard[row][column] == 1) ? 9 : -9;
			}
		}
	}
	else if((row != 0) && (row != MAXR)){ //Both sides now
		//One up-One down, Two up, Two down
		if((myBoard[row+1][column] != 0) && (myBoard[row+1][column] == myBoard[row-1][column])){
			//Check who won
			if((myBoard[row][column] == 1)&&(myBoard[row][column] == myBoard[row+1][column])){
				toReturn = 10; //It was me
			}
			else if((myBoard[row][column] == -1)&&(myBoard[row][column] == myBoard[row+1][column])){
				toReturn = -10; //It was the oponent
			}
			else{
				//Prevented the oponent's victory
				toReturn = (myBoard[row][column] == 1) ? 9 : -9;				
			}
		}

		if((row-2 >= 0) && (myBoard[row-1][column] != 0) && (myBoard[row-1][column] == myBoard[row-2][column])){
			//Check who won
			if((myBoard[row][column] == 1)&&(myBoard[row][column] == myBoard[row-1][column])){
				toReturn = 10; //It was me
			}
			else if((myBoard[row][column] == -1)&&(myBoard[row][column] == myBoard[row-1][column])){
				toReturn = -10; //It was the oponent
			}
			else{
				//Prevented the oponent's victory
				toReturn = (myBoard[row][column] == 1) ? 9 : -9;
			}
		}

		if((row+2 <= MAXR) && (myBoard[row+1][column] != 0) && (myBoard[row+1][column] == myBoard[row+2][column])){
			//Check who won
			if((myBoard[row][column] == 1)&&(myBoard[row][column] == myBoard[row+1][column])){
				toReturn = 10; //It was me
			}
			else if((myBoard[row][column] == -1)&&(myBoard[row][column] == myBoard[row+1][column])){
				toReturn = -10; //It was the oponent
			}
			else{
				//Prevented the oponent's victory
				toReturn = (myBoard[row][column] == 1) ? 9 : -9;
			}
		}
	}

	//Diagonals.
	//Two down right
	if((row+2 <= MAXR) && (column+2 <= MAXC) && (myBoard[row+1][column+1] != 0)
		&& (myBoard[row+1][column+1] == myBoard[row+2][column+2])){
		//Check who won
		if((myBoard[row][column] == 1) && (myBoard[row][column] == myBoard[row+1][column+1])) {
			toReturn = 10; //It was me
		}
		else if((myBoard[row][column] == -1) && (myBoard[row][column] == myBoard[row+1][column+1])){
			toReturn = -10; //It was the oponent
		}
		else{
			//Prevented the oponent's victory
			toReturn = (myBoard[row][column] == 1) ? 9 : -9;
		}
	}
	//Two down left
	if((row+2 <= MAXR) && (column-2 >= 0) && (myBoard[row+1][column-1] != 0)
		&& (myBoard[row+1][column-1] == myBoard[row+2][column-2])){
		//Check who won
		if((myBoard[row][column] == 1) && (myBoard[row][column] == myBoard[row+1][column-1])){
			toReturn = 10; //It was me
		}
		else if((myBoard[row][column] == -1) && (myBoard[row][column] == myBoard[row+1][column-1])){
			toReturn = -10; //It was the oponent
		}
		else{
			//Prevented the oponent's victory
			toReturn = (myBoard[row][column] == 1) ? 9 : -9;
		}
	}
	//Two up right
	if((row-2 >= 0) && (column+2 <= MAXC) && (myBoard[row-1][column+1] != 0)
		&& (myBoard[row-1][column+1] == myBoard[row-2][column+2])){
		//Check who won
		if((myBoard[row][column] == 1) && (myBoard[row][column] == myBoard[row-1][column+1])){
			toReturn = 10; //It was me
		}
		else if((myBoard[row][column] == -1) && (myBoard[row][column] == myBoard[row-1][column+1])){
			toReturn = -10; //It was the oponent
		}
		else{
			//Prevented the oponent's victory
			toReturn = (myBoard[row][column] == 1) ? 9 : -9;
		}
	}
	//Two up left
	if((row-2 >= 0) && (column-2 >= 0) && (myBoard[row-1][column-1] != 0)
		&& (myBoard[row-1][column-1] == myBoard[row-2][column-2])){
		//Check who won
		if((myBoard[row][column] == 1) && (myBoard[row][column] == myBoard[row-1][column-1])){
			toReturn = 10; //It was me
		}
		else if((myBoard[row][column] == -1) && (myBoard[row][column] == myBoard[row-1][column-1])){
			toReturn = -10; //It was the oponent
		}
		else{
			//Prevented the oponent's victory
			toReturn = (myBoard[row][column] == 1) ? 9 : -9;
		}
	}
	//Others
	if((row-1 >= 0) && (row+1 <= MAXR) && (column-1 >= 0) && (column+1 <= MAXC)){
		//One up left, one down right
		if((myBoard[row-1][column-1] != 0) && (myBoard[row-1][column-1] == myBoard[row+1][column+1])){
			//Check who won
			if((myBoard[row][column] == 1) && (myBoard[row][column] == myBoard[row-1][column-1])){
				toReturn = 10; //It was me
			}
			else if((myBoard[row][column] == -1) && (myBoard[row][column] == myBoard[row-1][column-1])){
				toReturn = -10; //It was the oponent
			}
			else{
				//Prevented the oponent's victory
				toReturn = (myBoard[row][column] == 1) ? 9 : -9;
			}
		}
		//One down left, one up right
		if((myBoard[row+1][column-1] != 0) && (myBoard[row+1][column-1] == myBoard[row-1][column+1])){
			//Check who won
			if((myBoard[row][column] == 1) && (myBoard[row][column] == myBoard[row+1][column-1])){
				toReturn = 10; //It was me
			}
			else if((myBoard[row][column] == -1) && (myBoard[row][column] == myBoard[row+1][column-1])){
				toReturn = -10; //It was the oponent
			}
			else{
				//Prevented the oponent's victory
				toReturn = (myBoard[row][column] == 1) ? 9 : -9;
			}
		}
	}

	//Additional points for placing the token next to another of the same type.
	for(int i=row-1; i <= row+1; i++){
		for(int j=column-1; j <= column+1; j++){
			if((i==row) && (j==column)){
				continue;
			}
			if((i>=0)&&(i<=MAXR)&&(j>=0)&&(j<=MAXC)){ //Check limits
				if(myBoard[i][j]==1){
					toReturn+=1;
					break;
				}
				else if(myBoard[i][j]==-1){
					toReturn-=1;
					break;
				}
			}
		}
	}

	return toReturn;
}

static int searchPlay(Board const& board, int ** myBoard, int const& row, int const& column, int depth, bool isMaximizer, int const& MAXR, int const& MAXC){
	int worth = evaluate(board, myBoard, row, column) - depth; //This is the heuristic
	
	//Check for TERMINAL STATES (the first two about the board, the other about the algorythm).

	//If it plays on this position, it wins.
	//Or the maximum depth has been reached, so it's the end of the search on that branch.
	if((worth >= pointsForWin) || (depth >= maxDepth)){ 
		return worth;
	}

	//Check for a full board and not wincon, so it's a draw.
	bool full = true;
	for(size_t i = 0; i < MAXR; i++){
		for(size_t j = 0; j < MAXR; j++){
			if(myBoard[i][j] == 0){
				full=false;
				break;
			}
		}
		if(!full){
			break;
		}
	}
	if(full){
		return 0;
	}

	//Keep looking because the current state of the game is not a leaf
	int toReturn;
	if(isMaximizer){
		//Start of oponent path.
		int worstUtility = INT_MAX;
		for(size_t r = 0; r < MAXR; r++){
			for(size_t c = 0; c < MAXC; c++){
				//Check if the coordinate is occupied or not.
				if(myBoard[r][c] == 0){
					//Play on that coordinate
					myBoard[r][c] = -1;
					//Get the worth value
					worth = searchPlay(board, myBoard, r, c, depth+1, false, MAXR, MAXC);
					if(worth < worstUtility){
						worstUtility = worth;
					}
					myBoard[r][c] = 0; //Remove token
				}
			}
		}
		toReturn = worstUtility;
	}
	else{
		int bestUtility = INT_MIN;
		for(size_t r = 0; r < MAXR; r++){
			for(size_t c = 0; c < MAXC; c++){
				//Check if the coordinate is occupied or not.
				if(myBoard[r][c] == 0){
					//Play on that coordinate
					myBoard[r][c] = 1;
					//Get the worth value
					worth = searchPlay(board, myBoard, r, c, depth+1, true, MAXR, MAXC);
					if(worth > bestUtility){
						bestUtility = worth; 
					}
					myBoard[r][c] = 0; //Remove token
				}
			}
		}
		toReturn = bestUtility;
	}
	
	return toReturn; 
}

static Coordinates minimax(Board const& board){
	
	//Set utility variables.
	Coordinates bestCoordinate;
	int worth;
	int bestUtility = INT_MIN;
	int depth = 0;
	int MAXR = board.height();
	int MAXC = board.width();
	bestCoordinate.x = MAXC/2;
	bestCoordinate.y = MAXR/2; 

	//Copy the current state of the board (before playing).
	int ** myBoard = copyBoard(board);

	//The board here is a matrix where you can put a token anywhere that's not occupied.
	//Considering that, we need to roam for all the posible places to check their worth value, and select the best one.
	for(size_t r = 0; r < MAXR; r++){
		for(size_t c = 0; c < MAXC; c++){
			//Check if the coordinate is occupied or not.
			if(myBoard[r][c] == 0){
				//Play on that coordinate
				myBoard[r][c] = 1;
				//Get the worth value
				worth = searchPlay(board, myBoard, r, c, depth, true, MAXR, MAXC);
				if(worth > bestUtility){
					bestUtility = worth;
					bestCoordinate.x = c;
					bestCoordinate.y = r; 
				}
				myBoard[r][c] = 0; //Remove token
			}
		}
	}

	//Debugging
	printBoard(myBoard, MAXR, MAXC);

	//Free the memory cells
	for(int i = 0; i < board.height(); i++){
		delete [] myBoard[i];
	}
	delete [] myBoard;

	//Debugging
	std::cout << "\n" << "bestUtility=" << bestUtility << "\n" << "coords=(" << bestCoordinate.x << "," << bestCoordinate.y << ")\n";
	return bestCoordinate;

}


static Coordinates play(Board const& board)
{
	if (board.valid())
	{
		return minimax(board);
	}

	return { 0, 0 };
}

PlayerErick::PlayerErick():
	mName("Erick")
{
}

std::string const& PlayerErick::name() const
{
	return mName;
}

Coordinates PlayerErick::play(Board const& board) const
{
	return ttt::play(board);
}

} // namespace ttt
