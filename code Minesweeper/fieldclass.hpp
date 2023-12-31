#include<SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include <random>
#include "cellCode.cpp"
using namespace std;

class minesweeperField
{
    bool gameStart;

    char gameEnd;

    vector<Cell> cells;

public:
    minesweeperField();

    char endGame();

    unsigned short getFlags();

    void draw(sf::RenderWindow &targetWindow);
    void flagCell(unsigned char xInput, unsigned char yInput);
    void openCell(unsigned char xInput, unsigned char yInput);
    void restart();
    void setMouseState(unsigned char currMouseState, unsigned char xInput, unsigned char yInput);
};