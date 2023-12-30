#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "cellHeader.hpp"
#include "fieldclass.hpp"
#include "cellCode.cpp"
#include "graphicalcode.cpp"
using namespace std;
using namespace sf;

minesweeperField::minesweeperField() : gameEnd(1), random_engine(random_device())
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
    RectangleShape cellBox(sf::Vector2f(cellSize - 1, cellSize - 1));

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
            if (get_cell(a, b, cells)->getOpen())
            {
                // We get the number of mines surrounding it
                unsigned char mineCount = get_cell(a, b, cells)->getMineCount();

                cellBox.setFillColor(Color(232, 232, 232));

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
                if (!gameEnd)
                {
                    if (get_cell(a, b, cells)->getMouseHover() == 1)
                    {
                        cellBox.setFillColor(Color(255, 173, 100));
                    }
                    else if ( get_cell(a, b, cells)->getMouseHover() == 2)
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
        // We declare coordinate distributions
        uniform_int_distribution<unsigned short> xDistribution(0, columns - 1);
        uniform_int_distribution<unsigned short> yDistribution(0, rows - 1);

        gameStart = 1;

        // Then we generate mines
        for (unsigned short a = 0; a < numMines; a++)
        {
            unsigned char xCoordMine = static_cast<unsigned char>(xDistribution(random_engine));
            unsigned char yCoordMine = static_cast<unsigned char>(yDistribution(random_engine));

            // If the chosed cell already has a mine in it or it's a cell that the player wants to open
            if (get_cell(xCoordMine, yCoordMine, cells)->checkMine() || (xInput == xCoordMine && yInput == yCoordMine))
            {
                a--;
            }
            else
            {
                // Otherwise, we set the mine
                get_cell(mine_x, mine_y, cells)->set_mine();
            }
        }

        // After we generate mines, each cell counts how many mines are surrounding it
        for (Cell &cell : cells)
        {
            cell.count_mines_around(cells);
        }
    }

    // We don't open the cell when the game is over or when the cell is flagged
    if (0 == gameEnd && 0 == get_cell(i_x, i_y, cells)->get_is_flagged())
    {
        if (1 == get_cell(i_x, i_y, cells)->open(cells))
        {
            // When the player opens a cell with a mine, we set the game over to -1
            gameEnd = -1;
        }
        else
        {
            unsigned short total_closed_cells = 0;

            // We count how many cells are closed
            for (Cell &cell : cells)
            {
                total_closed_cells += 1 - cell.get_is_open();
            }

            // If the number of closed cells equals the total number of mines, we'll consider that the game is won
            if (MINES == total_closed_cells)
            {
                // We set the game over to 1
                gameEnd = 1;

                // Then we start the effect
                get_cell(i_x, i_y, cells)->set_effect_timer(EFFECT_DURATION - 1);
            }
        }
    }
}

void minesweeperField::restart()
{
    // We only restart the game when it's over
    if (0 != gameEnd)
    {
        first_click = 0;

        gameEnd = 0;

        for (Cell &cell : cells)
        {
            cell.reset();
        }
    }
}

// Since we can't call the cell's function directly, we must use this function
void minesweeperField::set_mouse_state(unsigned char i_mouse_state, unsigned char i_x, unsigned char i_y)
{
    get_cell(i_x, i_y, cells)->set_mouse_state(i_mouse_state);
}