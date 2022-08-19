// Peer-graded Assignment: Implement Monte Carlo Hex move evaluation,
// by Nickita Kyselyov, NTUU KPI nb. Igor Sikorsky, 2nd grade, FICT, IS-13,
// 19.08.2022

#include "HexBoardASCII.h"

int main(int argc, char const* argv[]) {
    //srand(static_cast<unsigned int>(time(nullptr)));
    short size; cout << "Input size of a game: "; cin >> size;
    bool isAI; cout << "What game mode do you want? (0-player vs player, 1-player vs AI) "; cin >> isAI;
    bool first; cout << "Who will go first? " << (isAI ? "(0-AI, 1-player) " : "(0-player #1, 1-player #2) "); cin >> first;


    HexBoard board(size);
    board.print();
    player win;
    for (short k = 0; (win = board.hasWon()) == player::NONE || k < size * size; k++) {
        cout << "Player #" << k % 2 + 1 << " make move! (Input \'i\' and \'j\')" << endl;
        short i, j;  cout << "i: "; cin >> i; cout << "j: "; cin >> j;
        if (!board.makeMoveIn(i, j, static_cast<player>(k % 2 + 1))) k--;
        board.print();
    }
    cout << "WON PLAYER: " << win << endl;


    return EXIT_SUCCESS;
}