#include <bits/stdc++.h>
#include "Board.h"

using namespace std;
void printSolution(Board* targetBoard, bool printPath = false){
    cout << "Total moves needed : " << targetBoard->cost() << endl;
    vector<Board*> path;
    Board* currentBoard = targetBoard;
    do {
        path.push_back(currentBoard);
        currentBoard = currentBoard->getPreviousBoard();
    } while (currentBoard != nullptr);

    if (!printPath)
        return;
    
    cout << "Solution : " << endl;
    for (int i = path.size()-1; i>=0; i--){
        cout << *(path[i]);
    }
    return;
}
void solvePuzzle(Board* initialBoard){
    long closedCount = 0;
    map<vector<int>, bool> visited;
    priority_queue<Board *, vector<Board*>, compareCost > pq;

    pq.push(initialBoard);
    visited[initialBoard->getBlocks()] = true;

    while (!pq.empty())
    {
        Board *promisingBoard = pq.top();
        pq.pop();
        closedCount++;
        // cout << promisingBoard;
        if (promisingBoard->isTarget())
        {
            cout << "total moves : " << promisingBoard->cost() << endl;
            printSolution(promisingBoard);
            cout << "total nodes explored with " << promisingBoard->getHeuristicName() + " heuristic : " << closedCount << endl;
            cout << "total nodes discovered with " << promisingBoard->getHeuristicName() + " heuristic : " << closedCount + pq.size() << endl;
            break;
        }
        for (auto neighbour : promisingBoard->getNeighBours())
        {
            if (!visited[neighbour->getBlocks()])
            {
                visited[neighbour->getBlocks()] = true;
                pq.push(neighbour);
            }
        }
    }
}


int main()
{
    int k;
    ifstream file("in.txt");
    file >> k;
    string inputBoard[k];
    for (int i = 0; i < k; i++)
        file >> inputBoard[i];

    Board* initialBoardMan = new Board(k, inputBoard, MANHATTAN);
    Board* initialBoardHam = new Board(k, inputBoard, HAMMING);
    
    if(!initialBoardMan->isSolvable()){
        cout << "unsolvable";
        return 0;
    }

    cout << "solvable" << endl;
    solvePuzzle(initialBoardMan);
    solvePuzzle(initialBoardHam);
    return 0;
}