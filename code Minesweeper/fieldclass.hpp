#include<SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include <random>
#include "cellCode.cpp"
using namespace std;

class minesweeperField
{
    char gameStart;

    char gameEnd;

    vector<Cell> cells;
    vector<Cell> cellsAIcpy;

public:
    minesweeperField();

    char endGame();

    unsigned short getFlags();

    bool solutionOverAI();
    bool AISolve();

    void draw(sf::RenderWindow &targetWindow);
    void drawAIBoard(sf::RenderWindow &targetWindow);
    void AIsolutionPRINTER(sf::RenderWindow &targetWindow);
    void generateAIBoard();
    void flagCell(unsigned char xInput, unsigned char yInput);
    void openCell(unsigned char xInput, unsigned char yInput);
    void restart();
    void setMouseState(unsigned char currMouseState, unsigned char xInput, unsigned char yInput);
    void revealBoard(sf::RenderWindow &targetWindow);
    

    pair<bool, pair<int, int>> checkForUnvisited();
};