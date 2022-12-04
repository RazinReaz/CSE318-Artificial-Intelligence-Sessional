#include <bits/stdc++.h>
#include "Board.h"

using namespace std;


int main()
{
    int k, x;
    ifstream file("in.txt");
    file >> k;
    string inputBoard[k];
    for (int i = 0; i < k; i++)
        file >> inputBoard[i];

    Board initialBoard(k, inputBoard, MANHATTAN);

    if(!initialBoard.isSolvable()){
        cout << "unsolvable";
        return 0;
    }

    cout << "solvable" << endl;


    map<vector<int>,bool> visited;
    priority_queue<Board> pq;
    pq.push(initialBoard);
    visited[initialBoard.getBlocks()] = true;

    while(!pq.empty()){
        Board b = pq.top();
        // cout << b;
        if(b.isTarget()){
            cout << "paisi" << endl;
            cout << "prev board at " << b.getPreviousBoard() << endl;
            break;
        }
        pq.pop();
        for(auto board: b.getNeighBours()){
            if (!visited[board.getBlocks()]){
                visited[board.getBlocks()] = true;
                pq.push(board);
            }
        }
    }

    return 0;
}