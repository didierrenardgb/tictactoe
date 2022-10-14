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

	static void printBoardState(Board const& board,int ** boardManagement)
	{
		for (int i = 0 ; i < board.height() ; i++){
			for (int j = 0 ; j < board.width() ; j++){
				std::cout<<boardManagement[j][i]<<" | ";
			}
			std::cout<<std::endl;
		}
		std::cout<<std::endl;
		std::cout<<std::endl;
		std::cout<<std::endl;
	}


	static Coordinates play(Board const& board, int ** boardManagement, std::string name) 
	{
		//std::cout<<"Nueva Build"<<std::endl;
		
		
		if (board.valid())                                                                   
		{
			printBoardState(board,boardManagement);
			Coordinates coords = randomCoordinate(board);
			while ((board.tile(coords) == nullptr) || board.tile(coords)->owner().has_value())
			{
				coords = randomCoordinate(board);
			}
			return coords;
		}

		return { 0, 0 };
	}



//Board Management: Hacemos la captura de la matriz de juego en su estado actual.
//De esta forma, para el algormitmo implementado, podemos trabajar con una copia de la matriz con una abstraccion semantica distinta a la desplegada en el menu.
	
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
		int  ** boardManagement = ttt::initializeAlphaBeta(board,name()); //En este momento hacemos la captura de la matriz actual.
		return ttt::play(board,boardManagement,name());
	}






} // namespace ttt



