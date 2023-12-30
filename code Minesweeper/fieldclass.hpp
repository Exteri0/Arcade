#include<SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include <cellHeader.hpp>
using namespace std;

class minesweeperField
{
    bool gameStart;

    char gameEnd;

    default_random_engine random_engine;

    random_device random_device;

    vector<Cell> cells;

public:
    minesweeperField();

    char endGame();

    unsigned short getFlags();

    void draw(sf::RenderWindow &targetWindow);
    void flagCell(unsigned char xInput, unsigned char yInput);
    void openCell(unsigned char xInput, unsigned char yInput);
    void restart();
    void setMouseState(unsigned char i_mouse_state, unsigned char i_x, unsigned char i_y);
};