#pragma once
#include "Graph.h"
using namespace std;

inline void printStrs(short n, const char* s) {
	for (int i = 0; i < n; i++) cout << s;
}

class HexBoard {
	short size; // size of the board
	short** board; // field of action, 0 - empty, 1 - player ¹1 walked, 2 - player ¹2 walked
public:
	HexBoard(short size = 7) : size(size + 1), board(new short*[this->size]) {
		for (int i = 0; i < this->size; i++) {
			board[i] = new short[this->size];
			for (int j = 0; j < this->size; j++) board[i][j] = 0;
		}
		for (int i = 0; i < this->size; i++) board[i][0] = 1, board[i][this->size - 1] = 1;
		for (int j = 1; j < this->size - 1; j++) board[0][j] = 2, board[this->size - 1][j] = 2;
	}
	~HexBoard() {
		for (int i = 0; i < size; i++) delete[] board[i];
		delete[] board;
	}

	// Checking if someone has won
	short hasWon() {
		return 1;
	}

	// Takes a move to a given position
	bool makeMoveIn(short i, short j, short player) {
		if ((i < 1 || i > size - 2) && (j < 1 || j > size - 2)) {
			cerr << "Out of bounds move!" << endl;
			return false;
		}
		if (board[i][j]) {
			cerr << "There is already another player's move at position i:" << i << " j:" << j << endl;
			return false;
		}

		bool cond = true;

		if (isTooFar(i, j, player)) {
			cerr << "This move is too far from your cells!" << endl;
			return false;
		}

		board[i][j] = player;
		cout << "The move was made successfully!" << endl;
		return true;
	}

	// Check if this move is too far from your cells
	bool isTooFar(short i, short j, short player) {
		if (board[i][j - 1] == player ||
			board[i - 1][j] == player ||
			board[i - 1][j + 1] == player ||
			board[i][j + 1] == player ||
			board[i + 1][j - 1] == player ||
			board[i + 1][j] == player)
			return false;
		else return true;
	}

	// Displays the game board
	void print() {
		cout << "Hex board now:" << endl;
		for (int i = 0; i < size - 1; i++) {
			cout << "Ind I: " << setw(3) << i;
			printStrs(2 * i + 1, " ");
			for (int j = 0; j < size - 1; j++) cout << board[i][j] << " - ";
			cout << board[i][size - 1] << endl;
			printStrs(2 * i + 11, " ");
			printStrs(size - 1, " \\ /"); cout << " \\" << endl;
		}
		cout << "Ind I: " << setw(3) << this->size - 1;
		printStrs(2 * (size - 1) + 1, " ");
		for (int j = 0; j < size - 1; j++) cout << board[size - 1][j] << " - ";
		cout << board[size - 1][size - 1] << endl;
	}
};