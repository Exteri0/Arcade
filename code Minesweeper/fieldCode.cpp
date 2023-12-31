#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <time.h>
#include "fieldclass.hpp"
#include "graphicalcode.cpp"
using namespace std;
using namespace sf;
minesweeperField::minesweeperField() : gameEnd(1), gameStart(0)
{
    // Here we're adding cells to the cells vector
    for (unsigned char a = 0; a < rows; a++)
    {
        for (unsigned char b = 0; b < columns; b++)
        {
            // I swapped the coordinates to fix the bug regarding the coordinates
            cells.push_back(Cell(b, a));
        }
    }

    // Like the reset function in the Cell class, we have a restart function that restarts the game, so why not use it?
    restart();
}

char minesweeperField::endGame()
{
    // I'm not even gonna explain this
    return gameEnd;
}

unsigned short minesweeperField::getFlags()
{
    // We just count the total number of flagged cells
    unsigned short flagCount = 0;

    for (Cell cell : cells)
    {
        flagCount += cell.getFlag();
    }

    // And we return the result
    return flagCount;
}

// I'm too lazy to write comments for this function
// IT'S SOOOOOOOO LOOOOOOOOONG!!!!!!!!
void minesweeperField::draw(sf::RenderWindow &targetWindow)
{
    // We'll use this to draw cells
    RectangleShape cellBox(Vector2f(cellSize - 1, cellSize - 1));

    Sprite pictureSprite;
    Texture pictureTexture;

    pictureTexture.loadFromFile("Icons16.png");

    pictureSprite.setTexture(pictureTexture);

    // We loop through each cell
    for (unsigned char a = 0; a < columns; a++)
    {
        for (unsigned char b = 0; b < rows; b++)
        {
            // We change the position of the cell shape
            cellBox.setPosition(static_cast<float>(cellSize * a), static_cast<float>(cellSize * b));

            // If the current cell is open
            if (get_cell(a, b, cells)->getOpen() == 1)
            {
                // We get the number of mines surrounding it
                unsigned char mineCount = get_cell(a, b, cells)->getMineCount();
                if(!get_cell(a,b,cells)->checkMine()){
                cellBox.setFillColor(Color(200, 200, 200));
                }
                else{
                    cellBox.setFillColor(Color(0, 0, 0));
                }

                // We draw the cell (Wow, what a surprise!)
                targetWindow.draw(cellBox);

                // If the cell has at least one mine around it
                if (mineCount > 0)
                {
                    // We take the number from the icons' texture and draw it
                    pictureSprite.setPosition(static_cast<float>(cellSize * a), static_cast<float>(cellSize * b));
                    pictureSprite.setTextureRect(IntRect(cellSize * mineCount, 0, cellSize, cellSize));

                    targetWindow.draw(pictureSprite);
                }
            }
            else // If the cell is closed
            {
                cellBox.setFillColor(Color(246, 124, 18));

                // We set the cell's color based on the mouse state
                // We also don't change it's color if the game is over
                if (gameEnd == 0)
                {
                    if (get_cell(a, b, cells)->getMouseHover() == 1)
                    {
                        cellBox.setFillColor(Color(255, 173, 100));
                    }
                    else if (get_cell(a, b, cells)->getMouseHover() == 2)
                    {
                        cellBox.setFillColor(Color(255, 60, 100));
                    }
                }

                targetWindow.draw(cellBox);

                // If the cell is flagged
                if (get_cell(a, b, cells)->getFlag())
                {
                    // We take the flag image and draw it
                    pictureSprite.setPosition(static_cast<float>(cellSize * a), static_cast<float>(cellSize * b));
                    pictureSprite.setTextureRect(IntRect(0, 0, cellSize, cellSize));

                    targetWindow.draw(pictureSprite);
                }
            }

            // Reset the cell's mouse state
            get_cell(a, b, cells)->setMouseHover(0);
        }
    }
}

void minesweeperField::flagCell(unsigned char xInput, unsigned char yInput)
{
    // We don't let the player to flag cells when the game is over
    if (!gameEnd)
    {
        get_cell(xInput, yInput, cells)->flag();
    }
}

void minesweeperField::openCell(unsigned char xInput, unsigned char yInput)
{
    // If this is the first cell we're opening
    if (!gameStart)
    {

        gameStart = true;

        // Then we generate mines
        for (unsigned short a = 0; a < numMines; a++)
        {
            unsigned short xCoordMine = (rand() % columns);
            unsigned short yCoordMine = (rand() % rows);

            // If the chosed cell already has a mine in it or it's a cell that the player wants to open
            if (get_cell(xCoordMine, yCoordMine, cells)->checkMine() || (xInput == xCoordMine && yInput == yCoordMine))
            {
                a--;
            }
            else
            {
                // Otherwise, we set the mine
                get_cell(xCoordMine, yCoordMine, cells)->setMine();
            }
        }

        // After we generate mines, each cell counts how many mines are surrounding it
        for (Cell &cell : cells)
        {
            cell.countMines(cells);
        }
    }

    // We don't open the cell when the game is over or when the cell is flagged
    if (!gameEnd && !get_cell(xInput, yInput, cells)->getFlag())
    {
        if (get_cell(xInput, yInput, cells)->open(cells))
        {
            // When the player opens a cell with a mine, we set the game over to -1
            gameEnd = -1;
        }
        else
        {
            unsigned short closedCellCount = 0;

            // We count how many cells are closed
            for (Cell &cell : cells)
            {
                if (!cell.getOpen())
                {
                    closedCellCount++;
                }
            }

            // If the number of closed cells equals the total number of mines, we'll consider that the game is won
            if (numMines == closedCellCount)
            {
                // We set the game over to 1
                gameEnd = 1;

                // Then we start the effect
            }
        }
    }
}

void minesweeperField::restart()
{
    // We only restart the game when it's over
    if (0 != gameEnd)
    {
        gameStart = 0;

        gameEnd = 0;

        for (Cell &cell : cells)
        {
            cell.reset();
        }
    }
}

// Since we can't call the cell's function directly, we must use this function
void minesweeperField::setMouseState(unsigned char currMouseState, unsigned char xInput, unsigned char yInput)
{
    get_cell(xInput, yInput, cells)->setMouseHover(currMouseState);
}