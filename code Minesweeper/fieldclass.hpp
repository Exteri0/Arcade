#include<SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include <cellHeader.hpp>
class minesweeperField
{
    bool gameStart;

    char gameEnd;

    std::default_random_engine random_engine;

    std::random_device random_device;

    std::vector<Cell> cells;

public:
    minesweeperField();

    char endGame();

    unsigned short getFlags();

    void draw(sf::RenderWindow &i_window);
    void flagCell(unsigned char i_x, unsigned char i_y);
    void openCell(unsigned char i_x, unsigned char i_y);
    void restart();
    void setMouseState(unsigned char i_mouse_state, unsigned char i_x, unsigned char i_y);
};