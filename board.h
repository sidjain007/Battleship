#ifndef BOARD_H_
#define BOARD_H_


const int BOARDSIZE = 10;

class board {
	public:
		char grid[BOARDSIZE][BOARDSIZE];
		board();
		void printBoard();
		bool placeShip(int y1, int x1, int y2, int x2, int shipSize, char shipSymbol);
};


#endif
