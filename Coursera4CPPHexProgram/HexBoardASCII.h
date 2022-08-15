#pragma once
#include <iostream>
#include <vector>
using namespace std;

inline void printStrs(short n, const char* s) {
	for (int i = 0; i < n; i++) cout << s;
}

class HexBoard {
	short size; // size of the board
	short** board; // field of action, 0 - empty, 1 - player ¹1 walked, 2 - player ¹2 walked
public:
	HexBoard(short size = 7) : size(size), board(new short*[size]) {
		for (int i = 0; i < size; i++) {
			board[i] = new short[size];
			for (int j = 0; j < size; j++) board[i][j] = 0;
		}
	}
	~HexBoard() {
		for (int i = 0; i < size; i++) delete[] board[i];
		delete[] board;
	}

	// Checking if someone has won
	short hasWon() {

	}

	// Takes a move to a given position
	bool makeMoveIn(short i, short j, short player) {
		if (i > size - 1 || j > size - 1) {
			cerr << "Out of bounds move!" << endl;
			return false;
		}
		if (board[i][j]) {
			cerr << "There is already another player's move at position i:" << i << " j:" << j << endl;
			return false;
		}
		board[i][j] = player;
		cout << "The move was made successfully!" << endl;
		return true;
	}

	// Displays the game board
	void print() {
		cout << "Hex board now:" << endl;
		for (int i = 0; i < size - 1; i++) {
			printStrs(2 * i + 1, " ");
			for (int j = 0; j < size - 1; j++) cout << board[i][j] << " - ";
			cout << board[i][size - 1] << endl;
			printStrs(2 * i + 1, " ");
			printStrs(size - 1, " \\ /"); cout << " \\" << endl;
		}
		printStrs(2 * (size - 1) + 1, " ");
		for (int j = 0; j < size - 1; j++) cout << board[size - 1][j] << " - ";
		cout << board[size - 1][size - 1] << endl;
	}
};