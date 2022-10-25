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

#include "MiniMax.h"

#include "Board.h"
#include "Coordinates.h"
#include "Tile.h"

//Debugging
#include <iostream>

namespace ttt
{

MiniMax::MiniMax(Board const& board){
    this->MAXC = board.width();
    this->MAXR = board.height();
    copyBoard(board);
    isEmpty = board.empty();
}

void MiniMax::copyBoard(Board const& board){
	for(size_t r = 0; r < MAXR; r++){
        std::vector<int> row;
		for(size_t c = 0; c < MAXC; c++){
			//Copy everything into an int matrix
			Coordinates coords;
			coords.x = c;
			coords.y = r;
			if(!board.tile(coords)->owner().has_value()){
				row.push_back(0); //It's empty.
			}
			else if(board.tile(coords)->owner()->get().name() == "Erick"){
				row.push_back(1); //It's mine.
			}
			else{
				row.push_back(-1); //It's not mine.
			}
		}
        myBoard.push_back(row);
	}
}

void MiniMax::printBoard(){
	std::cout << "\nMINIMAX CLASS \nMETHOD PRINT BOARD"<<std::endl;
	for(size_t i=0; i < MAXR; i++){
		for(size_t j=0; j < MAXC; j++){
			std::cout << myBoard[i][j] << " | ";
		}
		std::cout << std::endl;
	}
	std::cout << "----------" << std::endl;
}

const int MiniMax::evaluate(int const& row, int const& column){
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
	int toReturn = 0;

	//First turn, always right at the middle
	if((isEmpty) && (row == MAXR/2 - 1) && (column == MAXC/2 - 1)){ //Like that or maybe this->isEmpty ?
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
	else if((column == MAXC-1) && (myBoard[row][column-1] != 0)){ //Rightmost col
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
	else if((column != 0) && (column != MAXC-1)){ //Both sides now
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

		if((column+2 < MAXC)&&(myBoard[row][column+1] != 0)&&(myBoard[row][column+1] == myBoard[row][column+2])){
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
	else if((row == MAXR-1) && (myBoard[row-1][column] != 0)){ //Downmost row
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
	else if((row != 0) && (row != MAXR-1)){ //Both sides now
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

		if((row+2 < MAXR) && (myBoard[row+1][column] != 0) && (myBoard[row+1][column] == myBoard[row+2][column])){
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
	if((row+2 < MAXR) && (column+2 < MAXC) && (myBoard[row+1][column+1] != 0)
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
	if((row+2 < MAXR) && (column-2 >= 0) && (myBoard[row+1][column-1] != 0)
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
	if((row-2 >= 0) && (column+2 < MAXC) && (myBoard[row-1][column+1] != 0)
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
	if((row-1 >= 0) && (row+1 < MAXR) && (column-1 >= 0) && (column+1 < MAXC)){
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
			if((i>=0)&&(i<MAXR)&&(j>=0)&&(j<MAXC)){ //Check limits
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

int MiniMax::searchPlay(int const& row, int const& column, int depth, bool isMaximizer){
    int worth = evaluate(row, column) - depth; //This is the heuristic
	
	//Check for TERMINAL STATES (the first two about the board, the other about the algorythm).

	//If it plays on this position, it wins.
	//Or the maximum depth has been reached, so it's the end of the search on that branch.
	if((worth >= pointsForWin) || (depth >= maxDepth)){ 
		return worth;
	}

	//Check for a full board and not wincon, so it's a draw.
	bool full = true;
	for(int i = 0; i < MAXR; i++){
		for(int j = 0; j < MAXR; j++){
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
		int worstUtility = std::numeric_limits<int>::max();
		for(int r = 0; r < MAXR; r++){
			for(int c = 0; c < MAXC; c++){
				//Check if the coordinate is occupied or not.
				if(myBoard[r][c] == 0){
					//Play on that coordinate
					myBoard[r][c] = -1;
					//Get the worth value
					worth = searchPlay(r, c, depth+1, false);
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
		int bestUtility = std::numeric_limits<int>::min();
		for(int r = 0; r < MAXR; r++){
			for(int c = 0; c < MAXC; c++){
				//Check if the coordinate is occupied or not.
				if(myBoard[r][c] == 0){
					//Play on that coordinate
					myBoard[r][c] = 1;
					//Get the worth value
					worth = searchPlay(r, c, depth+1, true);
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

Coordinates MiniMax::play(){
    //Set utility variables.
	Coordinates bestCoordinate;
	int worth;
	int bestUtility = std::numeric_limits<int>::min();
	int depth = 0;
	bestCoordinate.x = MAXC/2;
	bestCoordinate.y = MAXR/2;

	//The board here is a matrix where you can put a token anywhere that's not occupied.
	//Considering that, we need to roam for all the posible places to check their worth value, and select the best one.
	for(int r = 0; r < MAXR; r++){
		for(int c = 0; c < MAXC; c++){
			//Check if the coordinate is occupied or not.
			if(myBoard[r][c] == 0){
				//Play on that coordinate
				myBoard[r][c] = 1;
				//Get the worth value
				worth = searchPlay(r, c, depth, true);
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
	//printBoard(myBoard, MAXR, MAXC);

	//Debugging
	std::cout << "\n" << "bestUtility=" << bestUtility << "\n" << "coords=(" << bestCoordinate.x << "," << bestCoordinate.y << ")\n";
	return bestCoordinate;
}

} // namespace ttt
