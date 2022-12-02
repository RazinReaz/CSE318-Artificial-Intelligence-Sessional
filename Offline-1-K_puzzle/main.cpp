#include <bits/stdc++.h>
#include "Board.cpp"

using namespace std;

int main()
{
    int k, x;
    ifstream file("in.txt");
    file >> k;
    string inputBoard[k];
    for (int i = 0; i < k; i++)
        file >> inputBoard[i];

    Board initialBoard(k, inputBoard);
    initialBoard.generateAllPossibleNeighbours();
    cout << initialBoard;
    initialBoard.printNeighbours();
    return 0;
}