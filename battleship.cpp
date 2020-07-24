#include <iostream>
#include <ctype.h>  //isdigit(), tolower()
#include "battleship.h"

using namespace std;


// a mapping of the first letter of ship names to full ship names
const unordered_map<char, string> battleship::symbolToShip = {
	{'c', "carrier"},
	{'b', "battleship"},
	{'d', "destroyer"},
	{'s', "submarine"},
	{'p', "patrol boat"}
};


// battleship constructor
battleship::battleship() {
	getPlayerNames();
	isGameOver = false;
	isP1Turn = true;

	p1ShipStatus = {
		{"carrier", 5},
		{"battleship", 4},
		{"destroyer", 3},
		{"submarine", 3},
		{"patrol boat", 2}
	};

	p2ShipStatus = {
		{"carrier", 5},
		{"battleship", 4},
		{"destroyer", 3},
		{"submarine", 3},
		{"patrol boat", 2}
	};

	placeShips();  // placing p1 ships
	placeShips();  // placing p2 ships
}


// clears std::cout when switching turns to prevent seeing opponent's board
void battleship::flushCout(int numLines) {
	for (int i = 0; i < numLines; i++)
		cout << endl;
}


// returns (-1, -1) if invalid input else (row, col) of coordinate input
pair<int, int> battleship::parseCoordinatesInput(string input) {
	if (input.size() != 2) {
		cout << "Enter valid coordinates such as A3: " << endl;
		return make_pair(-1, -1);
	}

	bool hasDigit = false;
	int row;
	int col;

	// row number came first
	if (isdigit(input[0])) {
		hasDigit = true;
		col = (int) tolower(input[1]) - 97;  // ascii of lowercase a is 97
		row = (int) input[0] - 48;  // ascii of 0 is 48

	}
	// row number came second
	else if (isdigit(input[1])) {
		hasDigit = true;
		col = (int) tolower(input[0]) - 97;
		row = (int) input[1] - 48;
	}

	// not any of these formats 3a, 3A, a3, A3
	if (!hasDigit or col < 0 or col > 9) {
		cout << "Enter valid coordinates such as A3: " << endl;
		return make_pair(-1, -1);
	}

	// if here, valid input
	return make_pair(row, col);
}


// returns (row, col) coordinates upon successful input from user
pair <int, int> battleship::getCoordinates() {
	string response;
	cout << "Enter coordinates such as A3: " << endl;
	while (true) {
		getline(cin, response);
		pair<int, int> coord = parseCoordinatesInput(response);
		if (coord.first == -1)
			continue;  // invalid input, repeat

		cout << "Confirm " << response << "? (y/n)" << endl;
		getline(cin, response);
		if (response != "y") {
			cout << "Enter coordinates such as A3: " << endl;
			continue;  // user wants to change choice, repeat
		}

		return coord;
	}
}


// prompts user for player names and saves them
void battleship::getPlayerNames() {
	cout << "Enter player1 name: " << endl;
	getline(cin, p1);
	cout << "Enter player2 name: " << endl;
	getline(cin, p2);
	cout << endl;
}


// prompts player to place their ships and updates board accordingly
void battleship::placeShips() {
	board* ships;
	if (isP1Turn) {
		cout << p1 << "'s turn (make sure " << p2 << " can't see now)" << endl;
		ships = &p1Ships;
	}
	else {
		cout << p2 << "'s turn (make sure " << p1 << " can't see now)" << endl;
		ships = &p2Ships;
	}

	for (auto iter : p1ShipStatus) {
		ships->printBoard();
		cout << "Place " << iter.first << " (size " << iter.second << ")\n" << endl;

		while (true) {
			cout << "(start coordinate) ";
			pair<int, int> start = getCoordinates();
			cout << "(end coordinate) ";
			pair<int, int> end = getCoordinates();
			if (ships->placeShip(start.first, start.second, end.first, end.second, iter.second, iter.first[0]))
				break;
		}
	}
	isP1Turn = !isP1Turn;
	flushCout();
}


// execution of a player move (one iteration of game)
void battleship::gameStep() {
	string player;
	string enemy;
	board* guesses;
	board* ships;
	board* enemyShips;
	unordered_map<string, int>* enemyShipStatus;

	if (isP1Turn) {
		player = p1;
		enemy = p2;
		guesses = &p1Guesses;
		ships = &p1Ships;
		enemyShips = &p2Ships;
		enemyShipStatus = &p2ShipStatus;
	}
	else {
		player = p2;
		enemy = p1;
		guesses = &p2Guesses;
		ships = &p2Ships;
		enemyShips = &p1Ships;
		enemyShipStatus = &p1ShipStatus;
	}

	printTurnInfo(player, enemy, ships, guesses, enemyShipStatus);
	cout << "\n\n(Choose coordinate to fire at) ";
	while (true) {
		pair<int, int> coord = getCoordinates();
		if (fireAtCoordinates(coord.first, coord.second, player, enemy, guesses, enemyShips, enemyShipStatus))
			break;
	}

	if (isGameOver)
		return;

	string response;
	cout << "Press enter to hide your board before " << enemy << "'s turn" << endl;
	getline(cin, response);
	flushCout();
	isP1Turn = !isP1Turn;
}


// prints the relevant info to player at start of each turn
void battleship::printTurnInfo(string player, string enemy, board* ships,  board* guesses,
		unordered_map<string, int>* enemyShipStatus) {
	string response;
	cout << player << "'s turn" << endl;
	cout << "Press enter to see your board and guesses (make sure " << enemy << " can't see now)" << endl;
	getline(cin, response);
	cout << "Here's the status of your ships so far" << endl;
	ships->printBoard();
	cout << "Here's your fires so far" << endl;
	guesses->printBoard();
	cout << enemy << " still has their: ";
	for (auto iter : *enemyShipStatus)
		cout << iter.first << "\t";
}


// returns true and updates boards on a valid fire, false if already fired there
bool battleship::fireAtCoordinates(int y, int x, string player, string enemy, board* guesses,
		board* enemyShips, unordered_map<string, int>* enemyShipStatus) {

	// case - already hit
	if (guesses->grid[y][x] == 'x') {
		cout << "Already hit there! Choose again" << endl;
		return false;
	}
	// case - already miss
	else if (guesses->grid[y][x] == 'o') {
		cout << "Already missed there! Choose again" << endl;
		return false;
	}
	// case - new miss
	else if (enemyShips->grid[y][x] == '-') {
		guesses->grid[y][x] = 'o';
		enemyShips->grid[y][x] = 'o';
		cout << "Miss!" << endl;
		return true;
	}
	// case - new hit
	else {
		char shipSymbol = enemyShips->grid[y][x];
		string shipHit = symbolToShip.at(shipSymbol);
		guesses->grid[y][x] = 'x';
		enemyShips->grid[y][x] = 'x';
		cout << "Hit! " << shipHit << "!" << endl;
		enemyShipStatus->operator[](shipHit)--;

		// if hit sinks ship
		if (enemyShipStatus->operator[](shipHit) == 0) {
			enemyShipStatus->erase(shipHit);
			cout << "Congrats, you sunk " << enemy << "'s " << shipHit << endl;
			// game over if that was last ship
			if (enemyShipStatus->empty()) {
				cout << "Congrats, you sunk all " << enemy << "'s ships" << endl;
				cout << player << " wins!" << endl;
				isGameOver = true;
			}
		}
		return true;
	}
}
