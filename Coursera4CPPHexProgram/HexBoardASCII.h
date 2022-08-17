#pragma once
#include "Graph.h"
inline void printStrs(short n, const char* s) { for (int i = 0; i < n; i++) cout << s; }
enum class player : short { NONE, BLUE, RED };
ostream& operator<<(ostream& output, const player& p) {
	char c;
	switch (p) {
	case player::NONE:
		c = '.';
		break;
	case player::BLUE:
		c = 'X';
		break;
	case player::RED:
		c = 'O';
		break;
	default:
		c = 'U';
		break;
	}
	output << c;
	return output;
}

class HexBoard {
	short size;
	player** board;
	Graph<short, short> game;
public:
	HexBoard(short size = 7) : size(size + 2), board(new player* [this->size]), game(this->size * this->size, 0) {
		for (int i = 0; i < this->size; i++) {
			board[i] = new player[this->size];
			for (int j = 0; j < this->size; j++) board[i][j] = player::NONE;
		}
		// Bounds
		for (int k = 0; k < this->size; k++) {
			addCell(k, 0, player::BLUE);
			addCell(this->size - 1, k, player::RED);
			addCell(0, k, player::RED);
			addCell(k, this->size - 1, player::BLUE);
		}
	}
	~HexBoard() {
		for (int i = 0; i < size; i++) delete[] board[i];
		delete[] board;
	}

	// Checking if someone has won
	player hasWon() {
		if (game.dijkstra(size, 0, 0, 1).first[size * size - 1] != INF) return player::BLUE;
		if (game.dijkstra(0, 1, 0, 0).first[size*(size-1)-1] != INF) return player::RED;
		return player::NONE;
	}

	// Takes a move to a given position
	bool makeMoveIn(short i, short j, player p) {
		if (i < 1 || i > size - 2 || j < 1 || j > size - 2 || board[i][j] != player::NONE) {
			cerr << "Invalid move!" << endl;
			return false;
		}
		addCell(i, j, p);
		cout << "The move was made successfully!" << endl;
		return true;
	}

	// Add edge to a game graph
	inline void addEdgeToGame(short i1, short j1, short i2, short j2, player player) {
		game.addEdge({ 1, size * i1 + j1, size * i2 + j2, static_cast<color>(player) });
	}

	// Add a cell - add paths (edges) to all 6 neighbors
	void addCell(short i, short j, player p) {
		short jDec = max(0, j - 1),
			  jInc = min(this->size-1, j + 1),
			  iDec = max(0, i - 1),
			  iInc = min(this->size - 1, i + 1);
		addEdgeToGame(i, j, i, jDec, p);
		addEdgeToGame(i, j, iDec, j, p);
		addEdgeToGame(i, j, iDec, jInc, p);
		addEdgeToGame(i, j, i, jInc, p);
		addEdgeToGame(i, j, iInc, j, p);
		addEdgeToGame(i, j, iInc, jDec, p);
		board[i][j] = p;
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