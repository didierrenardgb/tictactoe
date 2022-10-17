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

	static Coordinates randomCoordinate(Board const& board)
	{
		return { std::rand() % board.width(), std::rand() % board.height() };
	}

	static void printBoardState(Board const& board,int ** boardManagement) //Imprime el estado del tablero semántico.
	{
		std::cout<<"BUILD"<<std::endl;
		for (int i = 0 ; i < board.width() ; i++){
			for (int j = 0 ; j < board.height() ; j++){
				std::cout<<"{ "<<i<<" , "<<j<<" }: "<<boardManagement[i][j]<<" | ";
			}
			std::cout<<std::endl;
		}
		std::cout<<std::endl;
		std::cout<<std::endl;
		std::cout<<std::endl;
	}

	static void executeMovement(int ** boardManagement, Coordinates c, bool actualPlayer)
	{
		if (actualPlayer){
			boardManagement[c.x][c.y] = 1;}
		else{
			boardManagement[c.x][c.y] = 2;}

	}


	//Paso el board como parametro para conocer las dimensiones del mismo. De esta forma, puedo usar notaciones de punteros de una manera simplificada sabiendo
	//de antemano las dimensiones de la estructura.


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


	//Si gano el jugador, devuelvo 10
	//Si gano el contrincante, devuelvo -10.
	//Si hubo empate, devuelvo 0.
	//Si no termino aun, devuelvo dependiendo la heuristica de las fichas.

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

	//Si se tiene que seguir jugando, retorna 0.
	//Si gano el jugador, retorna 1.
	//Si gano el rival, retorna 2.
	//Si hubo empate, retorna 3.

	static int gameOver(Board const& board, int ** boardManagement)
	{

		bool fullBoard = true; //Supongo que el tablero esta completo.
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
		if (fullBoard){ //Si todas al menos una ficha sin colocar, esto dará falso. En caso que todas hayan estado ocupadas, se llego a un empate.
			return 3;
		}else{
			return 0;
		}
	}


	static int minimax(int depth, int alpha, int beta, bool actualPlayer, Board const& board, int ** boardManagement)
	{
		/*std::string mess;
		if (actualPlayer)
				mess = "JUGADOR ACTUAL";
			else
				mess ="JUGADOR RIVAL";

		std::cout<<"NIVEL MINIMAX: "<<depth<<" , VALOR ALPHA: "<<alpha<<" VALOR BETA: "<<beta<<" , ESTA JUGANDO: "<<mess<<" Y EL TABLERO ESTA AHORA: "<<std::endl;
		printBoardState(board,boardManagement);
		*/


		std::vector<Coordinates> movs = getMovs(board,boardManagement);
		bool pruning =false;
		int gameOverValue = gameOver(board,boardManagement);
		if ((depth == 0) || gameOverValue>0){
			if (depth==0)
					std::cout<<"Profundidad máxima alcanzada.";
				else
					std::cout<<"HUBO GAME OVER"<<std::endl;
			return calculateValue(board,boardManagement,gameOverValue,actualPlayer);

		}
		else{
			std::vector<Coordinates> movs = getMovs(board,boardManagement);
			
			if (actualPlayer)
			{
				int value =-std::numeric_limits<int>::max();
				for (Coordinates candidate : movs){ 
					if (!pruning)
					{
						executeMovement(boardManagement,candidate,actualPlayer);
						value = std::max(value,minimax(depth,alpha,beta,!actualPlayer,board,boardManagement));
						alpha = std::max(alpha,value);
						if (alpha>=beta){
							std::cout<<"Prouding..."<<std::endl;
							pruning = true;
						}
						boardManagement[candidate.x][candidate.y] = 0; //Quitamos el movimiento.
					}


				}
				return value;
			} 
			else{
				int value =std::numeric_limits<int>::max();
				for (Coordinates candidate : movs){ 
					if (!pruning){
						executeMovement(boardManagement,candidate,actualPlayer);
						value = std::min(value,minimax(depth,alpha,beta,!actualPlayer,board,boardManagement));
						beta = std::min(beta,value);
						if (beta<=alpha){
							std::cout<<"Prouding..."<<std::endl;
							pruning=true;
						}
						boardManagement[candidate.x][candidate.y] = 0; //Quitamos el movimiento.
					}
				}
				return value;

			}

		}
	}


	static Coordinates play(Board const& board, int ** boardManagement, std::string name)  //Obtenemos la coordenada final de movimiento.
	{

		if (board.valid()){
			std::vector<Coordinates> movs = getMovs(board,boardManagement);

			Coordinates coords = randomCoordinate(board);
			bool actualPlayer = true; //Acarrea el contexto de cual jugador esta por poner ficha. Si es verdadero, es aquel jugador que llamo en un primer momento al metodo.
			//De lo contrario, es su rival.

			int alpha = -std::numeric_limits<int>::max(); //-Infinite
			int beta = std::numeric_limits<int>::max(); //Infinite.
			int best_alpha = alpha;
			for (Coordinates candidate : movs){ 
				executeMovement(boardManagement,candidate,actualPlayer); //Consumimos el movimiento.
				alpha=std::max(alpha,minimax(5, alpha,beta,!actualPlayer, board,boardManagement));
				if (alpha > best_alpha){
					best_alpha = alpha;
					coords = candidate;
				}
				boardManagement[candidate.x][candidate.y] = 0; //Quitamos el movimiento.
			}
			std::cout<<"El candidato elegido es: "<<coords.x<<" , "<<coords.y<<std::endl;


			movs.clear();
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




/*
	static bool gameOver(Board const& board, int ** boardManagement)
	{

		bool fullBoard = true; //Supongo que el tablero esta completo.
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
						std::cout<<"GAME OVER 1, con { "<<i<<","<<j<<"} valor: "<<boardManagement[i][j]<<" , { "<<i+1<<","<<j<<"} valor: "<<boardManagement[i+1][j]<<" , { "<<i+2<<","<<j<<"} valor: "<<boardManagement[i+2][j]<< std::endl;
						return true;
					}
				}
				if (j+2< height){
					if (boardManagement[i][j] == boardManagement[i][j+1] && boardManagement[i][j+1] ==  boardManagement[i][j+2] && boardManagement[i][j]!=0){
						std::cout<<"GAME OVER 2, con { "<<i<<","<<j<<"} valor: "<<boardManagement[i][j]<<" , { "<<i<<","<<j+1<<"} valor: "<<boardManagement[i][j+1]<<" , { "<<i<<","<<j+2<<"} valor: "<<boardManagement[i][j+2]<< std::endl;
						return true;
					}
				}
				if (j+2<height && i+2 < width){
					if ((boardManagement[i][j] == boardManagement[i+1][j+1] && boardManagement[i+1][j+1] == boardManagement[i+2][j+2] && boardManagement[i][j]!=0)   ||
						(boardManagement[i][j+2] == boardManagement[i+1][j+1] && boardManagement[i+1][j+1] == boardManagement[i+2][j] && boardManagement[i][j+2]!=0)) {
						std::cout<<"GAME OVER 3, con { "<<i<<","<<j<<"} valor: "<<boardManagement[i][j]<<" , { "<<i+1<<","<<j+1<<"} valor: "<<boardManagement[i+1][j+1]<<" , { "<<i+2<<","<<j+2<<"} valor: "<<boardManagement[i+2][j+2]<< std::endl;
						std::cout<<"GAME OVER 4, con { "<<i<<","<<j+2<<"} valor: "<<boardManagement[i][j+2]<<" , { "<<i+1<<","<<j+1<<"} valor: "<<boardManagement[i+1][j+1]<<" , { "<<i+2<<","<<j<<"} valor: "<<boardManagement[i+2][j]<< std::endl;
	
							return true;
						}
				}
			}
		
		}
		if (fullBoard){
			std::cout<<"HAY EMPATE"<<std::endl;
		}
		return fullBoard; //Si todas al menos una ficha sin colocar, esto dará falso. En caso que todas hayan estado ocupadas, se llego a un empate.
	}


*/






















} // namespace ttt



