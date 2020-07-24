#ifndef BATTLESHIP_H_
#define BATTLESHIP_H_


#include <string>
#include <unordered_map>
#include <utility>  // for std::pair
#include "board.h"

using namespace std;


class battleship {
	public:
		// game variables
		static const unordered_map<char, string> symbolToShip;
		bool isGameOver;
		bool isP1Turn;

		// player 1 variables
		string p1;
		board p1Ships;
		board p1Guesses;
		unordered_map<string, int> p1ShipStatus;

		// player 2 variables
		string p2;
		board p2Ships;
		board p2Guesses;
		unordered_map<string, int> p2ShipStatus;

		battleship();
		static void flushCout(int numLines = 50);
		static pair<int, int> parseCoordinatesInput(string input);
		static pair<int, int> getCoordinates();
		void getPlayerNames();
		void placeShips();
		void gameStep();
		void printTurnInfo(string player, string enemy, board* ships,  board* guesses,
				unordered_map<string, int>* enemyShipStatus);
		bool fireAtCoordinates(int y, int x, string player, string enemy, board* guesses,
				board* enemyShips, unordered_map<string, int>* enemyShipStatus);
};


#endif
