#pragma once
#include "Graph.h"
const int generations = 1000;
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
public:
	short size;
	player** board;
	Graph<short, short> game;
public:
	HexBoard(short size = 11) : size(size + 2), board(new player* [this->size]), game(this->size* this->size, 0) {
		for (int i = 0; i < this->size; i++) {
			board[i] = new player[this->size];
			for (int j = 0; j < this->size; j++) board[i][j] = player::NONE;
		}
		// Bounds
		for (int k = 0; k < this->size - 1; k++) {
			addCell(k, 0, player::BLUE);
			addCell(k + 1, this->size - 1, player::BLUE);
			addCell(0, k + 1, player::RED);
			addCell(this->size - 1, k, player::RED);
		}
	}
	~HexBoard() {
		for (int i = 0; i < size; i++) delete[] board[i];
		delete[] board;
	}

	// Checking if someone has won.
	player hasWon() {
		if (game.dijkstra(0, 0, 0, 1).first[size * size - 1] != INF) return player::BLUE;
		if (game.dijkstra(1, 1, 0, 0).first[size * size - 2] != INF) return player::RED;
		return player::NONE;
	}

	// Takes a move to a given position.
	bool makeMoveIn(short i, short j, player p, bool debug = false) {
		if (i < 1 || i > size - 2 || j < 1 || j > size - 2 || at(i, j) != player::NONE) {
			if (debug) cerr << "Invalid move!" << endl;
			return false;
		}
		addCell(i, j, p);
		if (debug) cout << "The move in i=" << i << ", j=" << j << " was made successfully!" << endl;
		return true;
	}

	// Calculates the best move based on the Monte Carlo method.
	pair<short, short> calcBestMove(player p) {
		HexBoard testboard(size), b(size); // Copy of hex board for initialization.
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				if (at(i, j) != player::NONE) testboard.addCell(i, j, at(i, j));

		vector<short> emptySpots, emptySpotsCounter(size * size); //vectors for shuffling and counting the empty spots.

		for (int i = 0; i < size; ++i)
			for (int j = 0; j < size; ++j)
				if (testboard.at(i, j) == player::NONE) {
					emptySpots.push_back(i * size + j);
					++emptySpotsCounter[i * size + j];
				}

		vector<short> emptySpotsCopy = emptySpots, bestMoves(size * size); //copy vector for initialization, and best moves.
		int wins = 0, lastElement;
		bool turn = false;

		for (int trials = 0; trials < generations;
			trials++, emptySpots = emptySpotsCopy, random_shuffle(emptySpots.begin(), emptySpots.end())) {

			for (int i = 0; i < size; i++)
				for (int j = 0; j < size; j++)
					if (at(i, j) != player::NONE) board[i][j] = testboard.board[i][j];

			for (lastElement = emptySpots[emptySpots.size() - 1]; !emptySpots.empty(); emptySpots.pop_back())
				testboard.makeMoveIn(lastElement / size, lastElement % size, (turn = !turn) ? player::BLUE : player::RED);

			for (int i = 0; i < size; ++i)
				for (int j = 0; j < size; ++j)
					if (testboard.at(i, j) == player::BLUE && testboard.hasWon() == player::BLUE && emptySpotsCounter[i * size + j])
						bestMoves[i * size + j]++, wins++; //store all the moves that led to a win.
		}
		//iterator to find the move with the best chances to win.
		auto maxElement = max_element(bestMoves.begin(), bestMoves.end()); //max value means best move.
		short dist = distance(bestMoves.begin(), maxElement), row = dist / size, col = dist % size;
		if (*maxElement == 0) row = emptySpotsCopy[0] / size, col = emptySpotsCopy[0] % size; // in case there is no winning choice.
		return make_pair(row, col);
	}

	// Add edge to a game graph.
	inline void addEdgeToGame(short i1, short j1, short i2, short j2, player player) {
		game.addEdge({ 1, size * i1 + j1, size * i2 + j2, static_cast<color>(player) });
	}

	// Add a cell - add paths (edges) to all 6 neighbors.
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

	// Displays the game board.
	void print() const {
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

	inline player at(short i, short j) { return board[i][j]; }
};