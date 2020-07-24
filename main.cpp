#include <iostream>
#include <string>
#include "battleship.h"

using namespace std;


int main() {
	cout << "Play battleship? (y/n)" << endl;
	string response;
	getline(cin, response);
	if (response != "y") {
		cout << "Farewell!" << endl;
		return 0;
	}

	battleship game;
	while (!game.isGameOver)
		game.gameStep();

	return 0;
}
