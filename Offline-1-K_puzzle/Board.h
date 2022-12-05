#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<iomanip>

using namespace std;

#define MANHATTAN 0
#define HAMMING 1
#define debug cout << "Here at line " << __LINE__ << endl

class Board {
    private:
        int size;
        int width;
        Board* previousBoard;
        vector<int> blocks;
        vector<Board*> neighbours;
        int blankIndex;
        int stepsTakenFromInitialBoard;
        bool neighboursGenerated;
        int heuristic;

        inline int getX(int index) const {
            return index % width;
        }
        inline int getY(int index) const {
            return index / width;
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

        int h() const {
            if(heuristic == MANHATTAN)
                return manhattanDistance();
            else if(heuristic == HAMMING)
                return hammingDistance();
            return 100000;
        }

        void swapBlocks(int i, int j) {
            int t = blocks[i];
            blocks[i] = blocks[j];
            blocks[j] = t;

            if (blocks[j] == 0) blankIndex = j;
        }

        void generateSingleNeighbour(int swapIndex) {
            Board* neighbour = new Board(*this);
            neighbour->previousBoard = this;
            neighbour->stepsTakenFromInitialBoard++;
            neighbour->swapBlocks(this->blankIndex, swapIndex);
            this->neighbours.push_back(neighbour);
            // cout << *neighbour;
            // cout << "address " << neighbour << endl;
            // cout << "^^etar parent" << neighbours[neighbours.size() - 1]->getPreviousBoard() << endl;
        }
        

        int getInversions2(vector<int> blocks){
            int inversions = 0;
            for(int i=0; i<size; i++)
                for(int j=i+1; j<size; j++)
                    if (blocks[j] < blocks[i])
                        inversions++;
            return inversions;
        }

    public:
        Board(int size, string numbers[], int heuristic = 0){
            this->size = size;
            this->width = (int)sqrt(size);
            this->stepsTakenFromInitialBoard = 0;
            this->neighboursGenerated = false;
            this->previousBoard = nullptr;
            this->heuristic = heuristic;

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
            stepsTakenFromInitialBoard = otherBoard.stepsTakenFromInitialBoard;
            previousBoard = nullptr;
            neighboursGenerated = false;
            heuristic = otherBoard.heuristic;
        }
        
        bool isSolvable(){
            vector<int> blocksCopy = blocks; 
            vector<int> temp(size);
            blocksCopy.erase(blocksCopy.begin() + blankIndex);
            int inversions = getInversions2(blocksCopy);
            cout << "inversions " << inversions << endl;
            if(size%2==1 && inversions%2==0){
                return true;
            } else if (size%2==0) {
                if (inversions%2==1 && getY(blankIndex)%2==0)
                    return true;
                if (inversions%2==0 && getY(blankIndex)%2==1)
                    return true;
            }
            return false;
        }
        void generateAllPossibleNeighbours()
        {
            int upIndex = blankIndex - width;
            int downIndex = blankIndex + width;
            int leftIndex = blankIndex - 1;
            int rightIndex = blankIndex + 1;
            
            bool prevExists = (previousBoard != nullptr);
            bool upAvailable = upIndex >= 0;
            bool downAvailable = downIndex < size;
            bool leftAvailable = leftIndex >= 0 && (getY(leftIndex) == getY(blankIndex));
            bool rightAvailable = (getY(rightIndex) == getY(blankIndex)) ;

            if ((!prevExists || (prevExists && upIndex != previousBoard->blankIndex)) && upAvailable){
                generateSingleNeighbour(upIndex);
            }

            if ((!prevExists || (prevExists && downIndex != previousBoard->blankIndex)) && downAvailable){
                generateSingleNeighbour(downIndex);
            }

            if ((!prevExists || (prevExists && leftIndex != previousBoard->blankIndex)) && leftAvailable){
                generateSingleNeighbour(leftIndex);
            }

            if ((!prevExists || (prevExists && rightIndex != previousBoard->blankIndex)) && rightAvailable){
                generateSingleNeighbour(rightIndex);
            }

            this->neighboursGenerated = true;
            return;
        }
        vector<Board*> getNeighBours() {
            if(!neighboursGenerated){
                generateAllPossibleNeighbours();
            }
            return this->neighbours;
        }
        vector<int> getBlocks() {
            return this->blocks;
        }
        Board* getPreviousBoard() {
            return this->previousBoard;
        }

        bool isTarget() {
            for(int i=0; i<size-1; i++)
                if(i != blocks[i]-1)
                    return false;
            return true;
        }

        int cost() const {
            return stepsTakenFromInitialBoard+h();
        } 

        string getHeuristicName(){
            if(heuristic==0)
                return "Manhatton";
            return "Hamming";
        }

        friend ostream& operator<<(ostream &stream, const Board &board);
        friend bool operator==(const Board &me,const Board &other);
};

ostream& operator<<(ostream& stream, const Board& board){
    cout << "=================" << endl;
    int n = board.width;
    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            board.blocks[y * n + x] == 0 ? stream << " *" : stream << setw(2) << board.blocks[y * n + x];
            stream << " ";
        }
        stream << endl;
    }
    // stream << "hamming dist : " << board.hammingDistance() << endl;
    // stream << "manhattan dist : " << board.manhattanDistance() << endl;
    // stream << "steps taken " << board.stepsTakenFromInitialBoard << endl;
    // stream << "cost : " << board.stepsTakenFromInitialBoard + board.h() << endl;
    cout << "=================" << endl;
    return stream;
}

struct compareCost {
    bool operator()(const Board *me, const Board *other){
        return me->cost() > other->cost();
    }
};

bool operator==(const Board &me, const Board &other){
    if (me.size != other.size)
        return false;

    for (int i = 0; i < me.size; i++)
        if (me.blocks[i] != other.blocks[i])
            return false;
    return true;
}