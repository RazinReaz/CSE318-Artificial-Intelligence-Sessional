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
    priority_queue<Board> pq;
    map<Board,bool> visited;


    // while(!pq.empty()){
    //     auto e = pq.top();
    //     cout << e;
    //     pq.pop();
    //     cout << "size of pq " << pq.size() << endl;
    //     for(auto b: e.getNeighBours()){
    //         cout << b;
    //     }
    //     cout << "size of pq " << pq.size() << endl;
    // }
    pq.push(initialBoard);
    visited[initialBoard] = true;

    while(!pq.empty()){
        Board b = pq.top();
        cout << b;
        if(b.isTarget()){
            cout << "paisi" << endl;
            break;
        } else {
            cout <<"****this not it****" << endl;
        }
        pq.pop();
        for(auto board: b.getNeighBours()){
            if (!visited[board]){
                cout << board;
                visited[board] = true;
                pq.push(board);
            }
        }
    }

    return 0;
}