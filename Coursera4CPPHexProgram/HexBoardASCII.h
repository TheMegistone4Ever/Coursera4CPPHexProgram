#pragma once
#include "Graph.h"
using namespace std;
inline void printStrs(short n, const char* s) { for (int i = 0; i < n; i++) cout << s; }
class HexBoard {
	short size; // size of the board
	short** board; // field of action, 0 - empty, 1 - player ¹1 walked, 2 - player ¹2 walked
	Graph<short, short> game;
public:
	HexBoard(short size = 7) : size(size + 2), board(new short*[this->size]), game(this->size* this->size, 0) {
		for (int i = 0; i < this->size; i++) {
			board[i] = new short[this->size];
			for (int j = 0; j < this->size; j++) board[i][j] = 0;
		}
		for (int i = 0; i < this->size; i++) board[i][0] = 1, board[i][this->size - 1] = 1;
		for (int j = 1; j < this->size - 1; j++) board[0][j] = 2, board[this->size - 1][j] = 2;
		for (int k = 1; k < this->size; k++) {
			addCell(0, k, 2);
			addCell(this->size - 1, k, 2);
			addCell(k, 0, 1);
			addCell(k, this->size - 1, 1);
		}
	}
	~HexBoard() {
		for (int i = 0; i < size; i++) delete[] board[i];
		delete[] board;
	}

	// Add edge to a game graph
	inline void addEdgeToGame(short i1, short j1, short i2, short j2, short player) {
		game.addEdge({ 1, this->size * i1 + j1, this->size * i2 + j2, static_cast<color>(player) });
	}

	// Checking if someone has won
	short hasWon() {
		if (game.dijkstra(this->size, 0, 0, 1).first[this->size - 1] != INF) return 1;
		if (game.dijkstra(this->size / 2, 1, 0, 0).first[this->size * this->size - this->size / 2] != INF) return 2;
		return 0;
	}

	// Takes a move to a given position
	bool makeMoveIn(short i, short j, short player) {
		if (i < 1 || i > size - 2 || j < 1 || j > size - 2 || board[i][j] || isTooFar(i, j, player)) {
			cerr << "Invalid move!" << endl;
			return false;
		}
		board[i][j] = player;
		addCell(i, i, player);
		cout << "The move was made successfully!" << endl;
		return true;
	}

	// Add a cell - add paths (edges) to all 6 neighbors
	void addCell(short i, short j, short player) {
		short jDec = max(0, j - 1);
		short jInc = min(this->size-1, j + 1);
		short iDec = max(0, i - 1);
		short iInc = min(this->size - 1, i + 1);
		addEdgeToGame(i, j, i, jDec, player);
		addEdgeToGame(i, j, iDec, j, player);
		addEdgeToGame(i, j, iDec, jInc, player);
		addEdgeToGame(i, j, i, jInc, player);
		addEdgeToGame(i, j, iInc, j, player);
		addEdgeToGame(i, j, iInc, jDec, player);
	}

	// Check if this move is too far from your cells
	bool isTooFar(short i, short j, short player) {
		if (board[i][j - 1] == player ||
			board[i - 1][j] == player ||
			board[i - 1][j + 1] == player ||
			board[i][j + 1] == player ||
			board[i + 1][j] == player ||
			board[i + 1][j - 1] == player)
			return false;
		return true;
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