#include "PlayerExe.h"
#include "Board.h"
#include "Coordinates.h"
#include "Tile.h"
#include <iostream>
#include <random>
#include <vector>

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
						return true;
					}
				}
				if (j+2< height){
					if (boardManagement[i][j] == boardManagement[i][j+1] && boardManagement[i][j+1] ==  boardManagement[i][j+2] && boardManagement[i][j]!=0){
						return true;
					}
				}
				if (j+2<height && i+2 < width){
					if ((boardManagement[i][j] == boardManagement[i+1][j+1] && boardManagement[i+1][j+1] == boardManagement[i+2][j+2] && boardManagement[i][j]!=0)   ||
						(boardManagement[i][j+2] == boardManagement[i+1][j+1] && boardManagement[i+1][j+1] == boardManagement[i+2][j] && boardManagement[i][j+2]!=0)) {
							return true;
						}
				}
			}
		
		}
		return fullBoard; //Si todas al menos una ficha sin colocar, esto dará falso. En caso que todas hayan estado ocupadas, se llego a un empate.
	}


	static int minimax(int depth, int alpha, int beta, bool isCurrentPlayer, Board const& board, int ** boardManagement)
	{
		std::vector<Coordinates> movs = getMovs(board,boardManagement);
		bool pruning =false;
		//if ((depth = 0)) 
		
		
		

	}


	static Coordinates play(Board const& board, int ** boardManagement, std::string name)  //Obtenemos la coordenada final de movimiento.
	{



		if (board.valid()){
			//std::cout<<"Nueva Build"<<std::endl;
			std::vector<Coordinates> movs = getMovs(board,boardManagement);
			/*for (Coordinates c : movs){ std::cout<<"Lista de movimientos, posible candidato: "<<c.x<<" , "<<c.y<<" ."<<std::endl;}*/
			//printBoardState(board,boardManagement);

			Coordinates coords = randomCoordinate(board);
			bool actualPlayer = true; //Acarrea el contexto de cual jugador esta por poner ficha. Si es verdadero, es aquel jugador que llamo en un primer momento al metodo.
			//De lo contrario, es su rival.

			int alpha = -std::numeric_limits<int>::max(); //-Infinite
			int beta = std::numeric_limits<int>::max(); //Infinite.
			int best_alpha = alpha;
			/*for (Coordinates candidate : movs){ 
				executeMovement(boardManagement,candidate,actualPlayer); //Consumimos el movimiento.
				//alpha=max(alpha,minimax(depth-1, alpha,beta,!actualPlayer, board,boardManagement));
				if (alpha > best_alpha){
					best_alpha = alpha;
					coords = candidate;
				}
				boardManagement[candidate.x][candidate.y] = 0; //Quitamos el movimiento.
			}*/


			








			while ((board.tile(coords) == nullptr) || board.tile(coords)->owner().has_value())
			{
				coords = randomCoordinate(board);
			}

			executeMovement(boardManagement,coords,actualPlayer);
			printBoardState(board,boardManagement);
			gameOver(board,boardManagement);







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



