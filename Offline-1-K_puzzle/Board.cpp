#pragma once

#include<vector>
#include<string>
#include<cmath>

using namespace std;

class Board {
    private:
        int size;
        int width;
        Board* previousBoard;
        vector<int> blocks;
        vector<Board> neighbours;
        int blankIndex;

        inline int getX(int index) const {
            return index % width;
        }
        inline int getY(int index) const {
            return index / width;
        }
        inline int getIndex(int y, int x) const {
            return y * width + x;
        }
        inline void setPreviousBoard(Board* parent){
            this->previousBoard = parent;
        }

        int hammingDistance() const {
            int distance = 0;
            for(int i = 0; i<size; i++){
                if (blocks[i] == 0) continue;
                if( i+1 != blocks[i] )
                    distance++;
            }
            return distance;
        }

        int manhattanDistance() const {
            int distance = 0;
            for(int i = 0; i < size; i++){
                if(blocks[i]==0) continue;

                int targetX = getX(blocks[i]-1);
                int targetY = getY(blocks[i]-1);

                int x = getX(i);
                int y = getY(i);

                distance += abs(targetX-x) + abs(targetY-y);
            }
            return distance;
        }
        //! test
        void swapBlocks(int i, int j) {
            int t = blocks[i];
            blocks[i] = blocks[j];
            blocks[j] = t;

            if (blocks[j] == 0)
                blankIndex = j;
        }

        //! test
        void generateSingleNeighbour(int swapIndex) {
            Board neighbour = *(this);
            neighbour.setPreviousBoard(this);
            neighbour.swapBlocks(this->blankIndex, swapIndex);
            this->neighbours.push_back(neighbour);
        }

    public:
        Board(int size, string numbers[]){
            this->size = size;
            this->width = (int)sqrt(size);
            for(int i = 0, number; i < size; i++){
                if (numbers[i][0] == '*') {
                    number = 0;
                    blankIndex = i;
                }
                else number = stoi(numbers[i]);
                blocks.push_back(number);
            }
        }
        Board( const Board& otherBoard ){
            size = otherBoard.size;
            width = otherBoard.width;
            blocks = otherBoard.blocks;
            blankIndex = otherBoard.blankIndex;
        }
        void generateAllPossibleNeighbours() {
            //move blank up
            int upIndex = blankIndex - width;
            if (upIndex != previousBoard->blankIndex && upIndex >= 0) 
                generateSingleNeighbour(upIndex);
            
            int downIndex = blankIndex + width;
            if (downIndex != previousBoard->blankIndex && downIndex < size)
                generateSingleNeighbour(downIndex);

            int leftIndex = blankIndex - 1;
            if (leftIndex != previousBoard->blankIndex && getY(leftIndex) == getY(blankIndex))
                generateSingleNeighbour(leftIndex);

            int rightIndex = blankIndex + 1;
            if (rightIndex != previousBoard->blankIndex && getY(rightIndex) == getY(blankIndex))
                generateSingleNeighbour(rightIndex);
        }

        bool isSolvable(){
            
        }

        void printNeighbours(){
            for(auto x: neighbours) 
                cout << x ;
            return;
        }

        friend ostream& operator<<(ostream& stream, const Board& board);
};

ostream& operator<<(ostream& stream, const Board& board){
    int n = board.width;
    for (int y = 0; y < n; y++)
    {
        for (int x = 0; x < n; x++)
        {
            board.blocks[y * n + x] == 0 ? stream << '*' : stream << board.blocks[y * n + x];
            stream << " ";
        }
        stream << endl;
    }
    stream << "hamming dist : " << board.hammingDistance() << endl;
    stream << "manhattan dist : " << board.manhattanDistance() << endl;
    stream << "blank is at [" << board.blankIndex << "]" << endl;

    return stream;
}