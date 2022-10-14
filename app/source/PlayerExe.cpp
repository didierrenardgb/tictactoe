#include "PlayerExe.h"
#include "Board.h"
#include "Coordinates.h"
#include "Tile.h"
#include <iostream>
#include <random>

namespace ttt
{

	static Coordinates randomCoordinate(Board const& board)
	{
		return { std::rand() % board.width(), std::rand() % board.height() };
	}


	static Coordinates play(Board const& board, int ** boardManagement) 
	{
		if (board.valid())                                                                   
		{
			std::cout<<"KLASJFLKASJLFKASJFKLASKFJASKF"<<std::endl;
			for (int i = 0 ; i < board.width() ; i++){
				for (int j = 0 ; j < board.height() ; j++){
					std::cout<<boardManagement[i][j]<<" | ";
				}
				std::cout<<std::endl;
			}
			std::cout<<std::endl;
			std::cout<<std::endl;
			std::cout<<std::endl;



			Coordinates coords = randomCoordinate(board);
			while ((board.tile(coords) == nullptr) || board.tile(coords)->owner().has_value())
			{
				coords = randomCoordinate(board);
			}
			return coords;
		}

		return { 0, 0 };
	}



	//semantica del tablero:

		//1 --> Jugador actual.
		//0 --> No lo tiene nadie.
		//2 --> Jugador contrincante.
	
	
	
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

	





	PlayerExe::PlayerExe(std::string const& name):mName(name)
	{
	}

	std::string const& PlayerExe::name() const
	{
		return mName;
	}

	Coordinates PlayerExe::play(Board const& board) const
	{
		int  ** boardManagement = ttt::initializeAlphaBeta(board,name());
		return ttt::play(board,boardManagement);
	}






} // namespace ttt



