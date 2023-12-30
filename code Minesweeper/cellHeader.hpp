#include <bits/stdc++.h>
using namespace std;
class Cell
{
    bool flagged, mined, opened;
    unsigned char count;
    unsigned char mouseHover;
    unsigned char xIndex;
    unsigned char yIndex;

public:
    Cell(unsigned char xInput, unsigned char yInput);

    bool getFlag();
    bool checkMine();
    bool getOpen();
    bool open(vector<Cell> &cells);
    bool cellIndexDoesntExist(unsigned char xInput, unsigned char yInput);

    unsigned char getMineCount();
    unsigned char getMouseHover();

    void countMines(std::vector<Cell> &cells);
    void flag();
    void reset();
    void setMine();
    void setMouseHover(unsigned char currMouseState);
};