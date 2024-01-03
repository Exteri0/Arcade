#include <bits/stdc++.h>
using namespace std;
class Cell
{
    bool flagged, mined, opened, revealed, visitedAI, safeAI;
    unsigned char count;
    unsigned char countAI;
    unsigned char mouseHover;
    unsigned char xIndex;
    unsigned char yIndex;

public:
    Cell(unsigned char xInput, unsigned char yInput);

    bool getFlag();
    bool checkMine();
    bool getOpen();
    bool getReveal();
    bool open(vector<Cell> &cells);
    bool cellIndexDoesntExist(char xInput, char yInput);
    bool checkVisited();
    bool checkSafe();

    unsigned char getMineCount();
    unsigned char getMouseHover();
    unsigned char getMineCountAI();

    void countMines(vector<Cell> &cells);
    void countMinesAI(vector<Cell> &cells);
    void flag();
    void reset();
    void setMine();
    void visit();
    void decreaseMineCountAI();
    void increaseMineCountAI();
    bool checkSafetyOfCell(vector<Cell> &cells);
    void setSafetyOfCell();
    void setUnsafetyOfCell();
    void setMouseHover(unsigned char currMouseState);
    void reveal();
    void unvisit();
};