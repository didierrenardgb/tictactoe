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

	for(size_t r = 0; r < 2; r++){ //Care with this as it could be a bigger board.
		for(size_t c = 0; c < 2; c++){ //Care with this as it could be a bigger board.
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
	int worth = evaluate(row, column); //This is the heuristic
	
	//If it plays on this position, it wins.
	//Or the maximum depth has been reached, so it's the end of the search on that branch.
	if((worth >= pointsForWin) || (depth >= maxDepth)){ 
		return worth;
	}

	//Check for a full board and not wincon, so it's a draw.
	if(board.filled()){
		return 0;
	}

	//Start of oponent path.
	int worstUtility = INFINITY;
	for(size_t r = 0; r < 2; r++){ //Care with this as it could be a bigger board.
		for(size_t c = 0; c < 2; c++){ //Care with this as it could be a bigger board.
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
	int worth = evaluate(row,column); //This is the heuristic.
	
	if((worth >= pointsForWin) || (depth >= maxDepth)){
		return -worth; //Needs to be negative because the nature of the game.
	}

	if(board.filled()){ //Draw
		return 0;
	}

	int bestUtility = -INFINITY;
	for(size_t r = 0; r < 2; r++){ //Care with this as it could be a bigger board.
		for(size_t c = 0; c < 2; c++){ //Care with this as it could be a bigger board.
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

const int PlayerErick::evaluate(int row, int column){
	//Center
	return 5;
	//Next to one of yours
	return 5;
	//Wincon
	return 10;
}

} // namespace ttt
