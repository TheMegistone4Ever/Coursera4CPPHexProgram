// Peer-graded Assignment: Homework 1: Implement Hex Board,
// by Nickita Kyselyov, NTUU KPI nb. Igor Sikorsky, 2nd grade, FICT, IS-13,
// 16.08.2022

#include "HexBoardASCII.h"

int main(int argc, char const* argv[]) {
    //srand(static_cast<unsigned int>(time(nullptr)));
    short size; cout << "Input size of a game: "; cin >> size;

    HexBoard board(size);
    board.print();

    for (short k = 0; k < size * size; k++) {
        cout << "Player #" << k % 2 + 1 << " make move! (Input \'i\' and \'j\')" << endl;
        try {
        short i;  cout << "i: "; cin >> i;
        short j;  cout << "j: "; cin >> j;
            if (!board.makeMoveIn(i, j, k % 2 + 1)) k--;
        } catch (const exception& e) { break; }
        board.print();
        short win = board.hasWon();
        if (win) {
            cout << "WON PLAYER #" << win << endl;
            break;
        }
        if (k == size * size - 1) cout << "DRAW!" << endl;
    }

    return EXIT_SUCCESS;
}