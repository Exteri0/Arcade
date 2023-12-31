#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

using namespace std;

#include "cellHeader.hpp"
#include "constants.hpp"

Cell *get_cell(unsigned short xInput, unsigned short yInput, std::vector<Cell> &cells);
Cell *get_cell(unsigned short xInput, unsigned short yInput, std::vector<Cell> &cells)
{
    return &cells[xInput + columns * yInput];
}

Cell::Cell(unsigned char xInput, unsigned char yInput) : mined(0),
                                                         xIndex(xInput),
                                                         yIndex(yInput)
{
    reset();
}

bool Cell::cellIndexDoesntExist(char xInput, char yInput)
{
    return (0 > xInput || 0 > yInput || columns == xInput || rows == yInput);
}

bool Cell::getFlag()
{
    // Return whether the cell is flagged or not
    return flagged;
}

bool Cell::checkMine()
{
    // Return whether the cell has a mine or not
    return mined;
}

bool Cell::getOpen()
{
    // Return whether the cell is open or not
    return opened;
}

bool Cell::open(vector<Cell> &cells)
{
    //"You can't open a cell that's already open" - (c) Someone smart, I think
    if (!opened)
    {
        opened = true;

        // If there are no mines in this cell and the cells around it, WE OPEN THEM ALL!
        if (!mined && count == 0)
        {
            for (char a = -1; a < 2; a++)
            {
                for (char b = -1; b < 2; b++)
                {
                    // Making sure we don't go outside the game field
                    if (cellIndexDoesntExist(xIndex + a, yIndex + b))
                    {
                        continue;
                    }

                    // Open the neighboring cells (even if they don't like it!)
                    get_cell(a + xIndex, b + yIndex, cells)->open(cells);
                }
            }
        }
        return mined;
    }

    // Return the number between -2573 and 8752
    return 0;
}

unsigned char Cell::getMineCount()
{
    return count;
}

unsigned char Cell::getMouseHover()
{
    // Are you THAT stupid?
    return mouseHover;
}

void Cell::countMines(std::vector<Cell> &cells)
{
    // We start counting from 0
    count = 0;

    // If this cell has a mine, then there's no point in counting mines around it
    if (!mined)
    {
        for (char a = -1; a < 2; a++)
        {
            for (char b = -1; b < 2; b++)
            {
                if (cellIndexDoesntExist(xIndex + a, yIndex + b))
                {
                    continue;
                }
                if (get_cell(a + xIndex, b + yIndex, cells)->checkMine())
                {
                    count++;
                }
            }
        }
    }
}

void Cell::flag()
{
    // A wise man once said: "You can't flag a cell that's already open, because there's no point in that"
    if (!opened)
    {
        // I know I can write !is_flagged but I won't
        flagged = !flagged;
    }
}

void Cell::reset()
{
    // Set every variable to 0
    flagged = 0;
    mined = 0;
    opened = 0;

    // Except the effect timer
    mouseHover = 0;
}

// I'm pretty sure you're smart enough to understand what these functions do below
// If you're not, then I feel sorry for you

void Cell::setMine()
{
    mined = 1;
}

void Cell::setMouseHover(unsigned char currMouseState)
{
    mouseHover = currMouseState;
}