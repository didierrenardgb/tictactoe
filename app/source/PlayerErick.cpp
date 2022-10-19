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

static void printBoard(int** const& myBoard){
	std::cout << std::endl;
	for(size_t i=0; i < 3; i++){
		for(size_t j=0; j < 3; j++){
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
	return 5;
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
		toReturn = 7;
	}

	//Check if you're preventing the oponent's victory DONE (in between the checks for wincon).
	
	//Check wincon for row DONE.
	if(column == 0){ //Leftmost col
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
				toReturn = 9;
			}
		}
	}
	else if(column == MAXC){ //Rightmost col
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
				toReturn = 9;
			}
		}
	}
	else{ //Both sides now
		//One left-One right, Two left, Two right

		//Make all this a little more efficient
		if((myBoard[row][column-1] == myBoard[row][column+1])){
			//Check who won
			if((myBoard[row][column] == 1)&&(myBoard[row][column] == myBoard[row][column+1])){
				toReturn = 10; //It was me
			}
			else if((myBoard[row][column] == -1)&&(myBoard[row][column] == myBoard[row][column+1])){
				toReturn = -10; //It was the oponent
			}
			else{
				//Prevented the oponent's victory
				toReturn = 9;
			}
		}

		if((column-2 >= 0) && (myBoard[row][column-1] == myBoard[row][column-2])){
			//Check who won
			if((myBoard[row][column] == myBoard[row][column-1]) && (myBoard[row][column] == 1)){
				toReturn = 10; //It was me
			}
			else if((myBoard[row][column] == myBoard[row][column-1]) && (myBoard[row][column] == -1)){
				toReturn = -10; //It was the oponent
			}
			else{
				//Prevented the oponent's victory
				toReturn = 9;
			}
		}

		if((column+2 <= MAXC)&&(myBoard[row][column+1] == myBoard[row][column+2])){
			//Check who won
			if((myBoard[row][column] == myBoard[row][column+1]) && (myBoard[row][column] == 1)){
				toReturn = 10; //It was me
			}
			else if((myBoard[row][column] == myBoard[row][column+1]) && (myBoard[row][column] == -1)){
				toReturn = -10; //It was the oponent
			}
			else{
				//Prevented the oponent's victory
				toReturn = 9;
			}
		}
	}

	//Check wincon for column DONE.
	if(row == 0){ //Upmost row
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
				toReturn = 9;
			}
		}
	}
	else if(row == MAXR){ //Downmost row
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
				toReturn = 9;
			}
		}
	}
	else{ //Both sides now
		//One up-One down, Two up, Two down

		//Make all this a little more efficient
		if(myBoard[row+1][column] == myBoard[row-1][column]){
			//Check who won
			if((myBoard[row][column] == 1)&&(myBoard[row][column] == myBoard[row+1][column])){
				toReturn = 10; //It was me
			}
			else if((myBoard[row][column] == -1)&&(myBoard[row][column] == myBoard[row+1][column])){
				toReturn = -10; //It was the oponent
			}
			else{
				//Prevented the oponent's victory
				toReturn = 9;				
			}
		}

		if((row-2 >= 0) && (myBoard[row-1][column] == myBoard[row-2][column])){
			//Check who won
			if((myBoard[row][column] == 1)&&(myBoard[row][column] == myBoard[row-1][column])){
				toReturn = 10; //It was me
			}
			else if((myBoard[row][column] == -1)&&(myBoard[row][column] == myBoard[row-1][column])){
				toReturn = -10; //It was the oponent
			}
			else{
				//Prevented the oponent's victory
				toReturn = 9;
			}
		}

		if((row+2 <= MAXR) && (myBoard[row+1][column] == myBoard[row+2][column])){
			//Check who won
			if((myBoard[row][column] == 1)&&(myBoard[row][column] == myBoard[row+1][column])){
				toReturn = 10; //It was me
			}
			else if((myBoard[row][column] == -1)&&(myBoard[row][column] == myBoard[row+1][column])){
				toReturn = -10; //It was the oponent
			}
			else{
				//Prevented the oponent's victory
				toReturn = 9;
			}
		}
	}

	//Diagonals DONE.
	//Two down right
	if((row+2 <= MAXR) && (column+2 <= MAXC) 
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
			toReturn = 9;
		}
	}
	//Two down left
	if((row+2 <= MAXR) && (column-2 >= 0) 
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
			toReturn = 9;
		}
	}
	//Two up right
	if((row-2 >= 0) && (column+2 <= MAXC) 
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
			toReturn = 9;
		}
	}
	//Two up left
	if((row-2 >= 0) && (column-2 >= 0) 
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
			toReturn = 9;
		}
	}
	//Others
	if((row-1 >= 0) && (row+1 <= MAXR) && (column-1 >= 0) && (column+1 <= MAXC)){
		//One up left, one down right
		if(myBoard[row-1][column-1] == myBoard[row+1][column+1]){
			//Check who won
			if((myBoard[row][column] == 1) && (myBoard[row][column] == myBoard[row-1][column-1])){
				toReturn = 10; //It was me
			}
			else if((myBoard[row][column] == -1) && (myBoard[row][column] == myBoard[row-1][column-1])){
				toReturn = -10; //It was the oponent
			}
			else{
				//Prevented the oponent's victory
				toReturn = 9;
			}
		}
		//One down left, one up right
		if(myBoard[row+1][column-1] == myBoard[row-1][column+1]){
			//Check who won
			if((myBoard[row][column] == 1) && (myBoard[row][column] == myBoard[row+1][column-1])){
				toReturn = 10; //It was me
			}
			else if((myBoard[row][column] == -1) && (myBoard[row][column] == myBoard[row+1][column-1])){
				toReturn = -10; //It was the oponent
			}
		}
	}

	//Check if the token is next to another one of yours, so add points
	for(size_t i=row-1; i <= row+1; i++){
		for(size_t j=column-1; j <= column+1; j++){
			if((i==row) && (j==column)){
				continue;
			}
			if((i>=0)&&(i<=MAXR)&&(j>=0)&&(j<=MAXC)){ //Check limits
				if(myBoard[i][j]==1){
					toReturn+=5;
					break;
				}
				else if(myBoard[i][j]==-1){
					toReturn-=5;
					break;
				}
			}
		}
	}


	return toReturn;
}

static int searchPlay(Board const& board, int ** myBoard, int row, int column, int depth, bool isMaximizer){
	int worth = evaluate(board, myBoard, row, column) - depth; //This is the heuristic
	
	//Check for TERMINAL STATES (the first two about the board, the other about the algorythm).

	//If it plays on this position, it wins.
	//Or the maximum depth has been reached, so it's the end of the search on that branch.
	if((worth >= pointsForWin) || (depth >= maxDepth)){ 
		return worth;
	}

	//Check for a full board and not wincon, so it's a draw.
	//Do not use the original board, use the copy!!!!!!!1!!11!
	int MAXR = board.height();
	int MAXC = board.width();
	bool full = true;
	for(size_t i=0; i<MAXR; i++){
		for(size_t j=0; j<MAXR; j++){
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
					worth = searchPlay(board, myBoard, r, c, depth+1, false);
					if(worth < worstUtility){
						worstUtility = worth;
					}
					//Do some more things
					myBoard[r][c] = 0; //Remove token
				}
			}
		}
		toReturn = worstUtility;
	}
	else{
		int bestUtility = INT_MIN;
		for(size_t r = 0; r < MAXR; r++){ //Care with this as it could be a bigger board.
			for(size_t c = 0; c < MAXC; c++){ //Care with this as it could be a bigger board.
				//Check if the coordinate is occupied or not.
				if(myBoard[r][c] == 0){
					//Play on that coordinate
					myBoard[r][c] = 1;
					//Get the worth value
					worth = searchPlay(board, myBoard, r, c, depth+1, true);
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
				worth = searchPlay(board, myBoard, r, c, depth, true);
				if(worth > bestUtility){
					bestUtility = worth;
					bestCoordinate.x = c;
					bestCoordinate.y = r; 
				}
				myBoard[r][c] = 0; //Remove token
			}
		}
	}

	//Free the memory cells
	for(int i = 0; i < board.height(); i++){
		delete [] myBoard[i];
	}
	delete [] myBoard;

	//Debugging
	std::cout << "\n" << "worth=" << worth << "\n" << "coords=(" << bestCoordinate.x << "," << bestCoordinate.y << ")\n";
	return bestCoordinate;

}


static Coordinates play(Board const& board)
{
	if (board.valid())
	{
		//My part
		return minimax(board);

		/*
		//Didi's
		Coordinates coords = randomCoordinate(board);
		while ((board.tile(coords) == nullptr) || board.tile(coords)->owner().has_value())
		{
			coords = randomCoordinate(board);
		}
		return coords;
		*/
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
	//My part
	//Debugging
	/*
	int ** myBoard = copyBoard(board);
	printBoard(myBoard);
	for(int i = 0; i < board.height(); i++){
		delete [] myBoard[i];
	}
	delete [] myBoard;
	*/

	//return ttt::minimax(board);

	//Didi's
	return ttt::play(board);
}

} // namespace ttt
