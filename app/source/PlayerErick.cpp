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

static Coordinates randomCoordinate(Board const& board)
{
	return { std::rand() % board.width(), std::rand() % board.height() };
}

static Coordinates play(Board const& board)
{
	if (board.valid())
	{
		//My part

		//Didi's
		Coordinates coords = randomCoordinate(board);
		while ((board.tile(coords) == nullptr) || board.tile(coords)->owner().has_value())
		{
			coords = randomCoordinate(board);
		}
		return coords;
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
	int ** myBoard = copyBoard(board);
	printBoard(myBoard);
	for(int i = 0; i < board.height(); i++){
		delete [] myBoard[i];
	}
	delete [] myBoard;
	//Didi's
	return ttt::play(board);
}

Coordinates PlayerErick::minimax(Board const& board){
	//Do the algorythm here.
	Coordinates bestCoordinate;
	int worth;
	int bestUtility = -INFINITY;
	int depth = 0;

	//The board here is a matrix where you can put a token anywhere that's not occupied.
	//Considering that, we need to roam for all the posible places to check their worth value, and select the best one.

	for(size_t r = 0; r < MAXR; r++){ //Care with this as it could be a bigger board.
		for(size_t c = 0; c < MAXC; c++){ //Care with this as it could be a bigger board.
			//Check if the coordinate is occupied or not.

			//Play on that coordinate

			//Get the worth value
			worth = minimize(board, r, c, depth);
			if(worth > bestUtility){
				bestUtility = worth;
				bestCoordinate.x = c;
				bestCoordinate.y = r; 
			}

			//Do some more things
		}
	}

	return bestCoordinate;

}

const int PlayerErick::minimize(Board const& board, int row, int column, int depth){
	int ** myBoard = copyBoard(board);
	int worth = evaluate(board, myBoard, row, column); //This is the heuristic
	
	//Check for TERMINAL STATES (the first two about the board, the other about the algorythm).

	//Check for a full board and not wincon, so it's a draw.
	if(board.filled()){
		return 0;
	}
	//If it plays on this position, it wins.
	//Or the maximum depth has been reached, so it's the end of the search on that branch.
	if((worth >= pointsForWin) || (depth >= maxDepth)){ 
		return worth;
	}

	//Start of oponent path.
	int worstUtility = INFINITY;
	for(size_t r = 0; r < MAXR; r++){ //Care with this as it could be a bigger board.
		for(size_t c = 0; c < MAXC; c++){ //Care with this as it could be a bigger board.
			//Check if the coordinate is occupied or not.

			//Play on that coordinate

			//Get the worth value
			worth = maximize(board, r, c, depth+1);
			if(worth < worstUtility){
				worstUtility = worth;
			}

			//Do some more things
		}
	}
	
	return worstUtility; 
}

const int PlayerErick::maximize(Board const& board, int row, int column, int depth){
	int ** myBoard = copyBoard(board);
	int worth = evaluate(board,myBoard,row,column); //This is the heuristic.
	
	if((worth >= pointsForWin) || (depth >= maxDepth)){
		return -worth; //Needs to be negative because the nature of the game.
	}

	if(board.filled()){ //Draw
		return 0;
	}

	int bestUtility = -INFINITY;
	for(size_t r = 0; r < MAXR; r++){ //Care with this as it could be a bigger board.
		for(size_t c = 0; c < MAXC; c++){ //Care with this as it could be a bigger board.
			//Check if the coordinate is occupied or not.

			//Play on that coordinate

			//Get the worth value
			worth = minimize(board, r, c, depth+1);
			if(worth > bestUtility){
				bestUtility = worth;
			}

			//Do some more things
		}
	}

	return bestUtility; 
}

const int PlayerErick::evaluate(Board const& board, int** const& myBoard, int row, int column){
	/*
	//Center
	return 5;
	//Next to one of yours
	return 5;
	//Wincon
	return 10;
	//Draw
	return 0; //necesary?
	*/

	//Start of the code
	int MAXR = board.height();
	int MAXC = board.width();
	int toReturn = 0;

	//Check wincon for row
	if(column == 0){ //Leftmost col
		//Only check two to the right
		if((myBoard[row][column] == myBoard[row][column+1]) && (myBoard[row][column+1] == myBoard[row][column+2])){
			//Check who won
			if(myBoard[row][column] == 1){
				toReturn = 10; //It was me
			}
			else if(myBoard[row][column] == -1){
				toReturn = -10; //It was the oponent
			}
		}
	}
	else if(column == MAXC){ //Rightmost col
		//Only check two to the left
		if((myBoard[row][column] == myBoard[row][column-1]) && (myBoard[row][column-1] == myBoard[row][column-2])){
			//Check who won
			if(myBoard[row][column] == 1){
				toReturn = 10; //It was me
			}
			else if(myBoard[row][column] == -1){
				toReturn = -10; //It was the oponent
			}
		}
	}
	else{ //Both sides now
		//One left-One right, Two left, Two right

		//Make all this a little more efficient
		if((myBoard[row][column] == myBoard[row][column+1]) && (myBoard[row][column-1] == myBoard[row][column+1])){
			//Check who won
			if(myBoard[row][column] == 1){
				toReturn = 10; //It was me
			}
			else if(myBoard[row][column] == -1){
				toReturn = -10; //It was the oponent
			}
		}

		if((myBoard[row][column] == myBoard[row][column-1]) && (myBoard[row][column+1] == myBoard[row][column-2])){
			//Check who won
			if(myBoard[row][column] == 1){
				toReturn = 10; //It was me
			}
			else if(myBoard[row][column] == -1){
				toReturn = -10; //It was the oponent
			}
		}

		if((myBoard[row][column] == myBoard[row][column+1]) && (myBoard[row][column+1] == myBoard[row][column+2])){
			//Check who won
			if(myBoard[row][column] == 1){
				toReturn = 10; //It was me
			}
			else if(myBoard[row][column] == -1){
				toReturn = -10; //It was the oponent
			}
		}
	}

	//Check wincon for column
	if(row == 0){ //Upmost row
		//Only check two to the bottom
		if((myBoard[row][column] == myBoard[row+1][column]) && (myBoard[row+1][column] == myBoard[row+2][column])){
			//Check who won
			if(myBoard[row][column] == 1){
				toReturn = 10; //It was me
			}
			else if(myBoard[row][column] == -1){
				toReturn = -10; //It was the oponent
			}
		}
	}
	else if(column == MAXC){ //Downmost row
		//Only check two to the top
		if((myBoard[row][column] == myBoard[row-1][column]) && (myBoard[row-1][column] == myBoard[row-2][column])){
			//Check who won
			if(myBoard[row][column] == 1){
				toReturn = 10; //It was me
			}
			else if(myBoard[row][column] == -1){
				toReturn = -10; //It was the oponent
			}
		}
	}
	else{ //Both sides now
		//One up-One down, Two up, Two down

		//Make all this a little more efficient
		if((myBoard[row][column] == myBoard[row+1][column]) && (myBoard[row+1][column] == myBoard[row-1][column])){
			//Check who won
			if(myBoard[row][column] == 1){
				toReturn = 10; //It was me
			}
			else if(myBoard[row][column] == -1){
				toReturn = -10; //It was the oponent
			}
		}

		if((myBoard[row][column] == myBoard[row-1][column]) && (myBoard[row-1][column] == myBoard[row-2][column])){
			//Check who won
			if(myBoard[row][column] == 1){
				toReturn = 10; //It was me
			}
			else if(myBoard[row][column] == -1){
				toReturn = -10; //It was the oponent
			}
		}

		if((myBoard[row][column] == myBoard[row+1][column]) && (myBoard[row+1][column] == myBoard[row+2][column])){
			//Check who won
			if(myBoard[row][column] == 1){
				toReturn = 10; //It was me
			}
			else if(myBoard[row][column] == -1){
				toReturn = -10; //It was the oponent
			}
		}
	}

	//Diagonals
	if((row == 0) && (column == 0)){
		//Top left
		if((myBoard[row][column] == myBoard[row+1][column+1]) && (myBoard[row+1][column+1] == myBoard[row+2][column+2])){
			//Check who won
			if(myBoard[row][column] == 1){
				toReturn = 10; //It was me
			}
			else if(myBoard[row][column] == -1){
				toReturn = -10; //It was the oponent
			}
		}
	}
	else if((row == 0) && (column == MAXC)){
		//Top right
		if((myBoard[row][column] == myBoard[row+1][column-1]) && (myBoard[row+1][column-1] == myBoard[row+2][column-2])){
			//Check who won
			if(myBoard[row][column] == 1){
				toReturn = 10; //It was me
			}
			else if(myBoard[row][column] == -1){
				toReturn = -10; //It was the oponent
			}
		}
	}
	else if((row == MAXR) && (column == 0)){
		//Bottom left
		if((myBoard[row][column] == myBoard[row-1][column+1]) && (myBoard[row-1][column+1] == myBoard[row-2][column+2])){
			//Check who won
			if(myBoard[row][column] == 1){
				toReturn = 10; //It was me
			}
			else if(myBoard[row][column] == -1){
				toReturn = -10; //It was the oponent
			}
		}
	}
	else if((row == MAXR) && (column == MAXC)){
		//Bottom right
		if((myBoard[row][column] == myBoard[row-1][column-1]) && (myBoard[row-1][column-1] == myBoard[row-2][column-2])){
			//Check who won
			if(myBoard[row][column] == 1){
				toReturn = 10; //It was me
			}
			else if(myBoard[row][column] == -1){
				toReturn = -10; //It was the oponent
			}
		}
	}
	else{
		//Others: two right-down, one left-up one right-down, two left-up, two left-down, one left-down one right-up, two right-up 
		if((myBoard[row][column] == myBoard[row+1][column+1]) && (myBoard[row+1][column+1] == myBoard[row+2][column+2])){
			//Check who won
			if(myBoard[row][column] == 1){
				toReturn = 10; //It was me
			}
			else if(myBoard[row][column] == -1){
				toReturn = -10; //It was the oponent
			}
		}

		if((myBoard[row][column] == myBoard[row-1][column-1]) && (myBoard[row-1][column-1] == myBoard[row+1][column+1])){
			//Check who won
			if(myBoard[row][column] == 1){
				toReturn = 10; //It was me
			}
			else if(myBoard[row][column] == -1){
				toReturn = -10; //It was the oponent
			}
		}

		if((myBoard[row][column] == myBoard[row-1][column-1]) && (myBoard[row-1][column-1] == myBoard[row-2][column-2])){
			//Check who won
			if(myBoard[row][column] == 1){
				toReturn = 10; //It was me
			}
			else if(myBoard[row][column] == -1){
				toReturn = -10; //It was the oponent
			}
		}

		if((myBoard[row][column] == myBoard[row+1][column-1]) && (myBoard[row+1][column-1] == myBoard[row+2][column-2])){
			//Check who won
			if(myBoard[row][column] == 1){
				toReturn = 10; //It was me
			}
			else if(myBoard[row][column] == -1){
				toReturn = -10; //It was the oponent
			}
		}

		if((myBoard[row][column] == myBoard[row+1][column-1]) && (myBoard[row+1][column-1] == myBoard[row-1][column+1])){
			//Check who won
			if(myBoard[row][column] == 1){
				toReturn = 10; //It was me
			}
			else if(myBoard[row][column] == -1){
				toReturn = -10; //It was the oponent
			}
		}

		if((myBoard[row][column] == myBoard[row-1][column+1]) && (myBoard[row-1][column+1] == myBoard[row-2][column+2])){
			//Check who won
			if(myBoard[row][column] == 1){
				toReturn = 10; //It was me
			}
			else if(myBoard[row][column] == -1){
				toReturn = -10; //It was the oponent
			}
		}
	}
	return toReturn;

}

int** PlayerErick::copyBoard(Board const& board){
	//int myBoard[MAXR][MAXC];
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

void PlayerErick::printBoard(int** const& myBoard){
	std::cout << std::endl;
	for(size_t i=0; i < 3; i++){
		for(size_t j=0; j < 3; j++){
			std::cout << myBoard[i][j] << " | ";
		}
		std::cout << std::endl;
	}
}

} // namespace ttt
