#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

using namespace std;

#include "cellHeader.hpp"
#include "constants.hpp"

Cell *get_cell(unsigned short xInput, unsigned short yInput, std::vector<Cell> &cells);
Cell *get_cell(unsigned short xInput, unsigned short yInput, std::vector<Cell> &cells)
{
    return &cells[xInput + columns * yInput]; //mathemtical formula to get a 2d array index from a 1d array
}

Cell::Cell(unsigned char xInput, unsigned char yInput) : mined(0),
                                                         xIndex(xInput),
                                                         yIndex(yInput),
                                                         revealed(0)
{
    reset(); //constructor of cell
}

bool Cell::cellIndexDoesntExist(char xInput, char yInput)
{
    return (0 > xInput || 0 > yInput || columns == xInput || rows == yInput); //check if a cell index is outside the board (0-based)
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
    // Return whether the cell is open or not.
    return opened;
}

bool Cell::checkVisited(){
    //Returns whether the A.I.
    return visitedAI;
}

bool Cell::checkSafe(){
    //Returns whether the cell could have a mine.
    return safeAI;
}

bool Cell::open(vector<Cell> &cells)
{
    //"You can't open a cell that's already open or flagged"
    if (!opened && !flagged)
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
    return 0;
}

unsigned char Cell::getMineCount() //obvious getter
{
    return count;
}

unsigned char Cell:: getMineCountAI(){ //obvious getter mk. II
    return countAI;
}

unsigned char Cell::getMouseHover() //returns the state of the mouse related to cell (hovering over it or not)
{

    return mouseHover;
}
bool Cell::getReveal(){ //checks if the cell is revealed or not (used after losing or in A.I Mode)
    return revealed;
}

void Cell::countMines(std::vector<Cell> &cells) //normal mode minecount
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

void Cell::countMinesAI(std::vector<Cell> &cells){ //this is different from normal mode as it considers the cell itself in the count
    countAI = 0;
    for (char a  = -1; a < 2; a++){
        for (char b = -1; b < 2; b++){
            if (cellIndexDoesntExist(xIndex + a, yIndex + b)){
                continue;
            }
            if(get_cell(a+xIndex, b+yIndex, cells)-> checkMine()){
                countAI++;
            }
        }
    }
}

void Cell:: increaseMineCountAI(){ //used in the solve function to reset the cell's minecount
    countAI++;
}

void Cell::flag()
{
    // A wise man once said: "You can't flag a cell that's already open, because there's no point in that"
    if (!opened && !flagged && flagCount < numMines) //u cant put more flags than mines
    {
        flagged = !flagged;
        flagCount++;
    }
    else if (!opened && flagged)
    {
        flagged = !flagged;
        flagCount--;
    }
}

void Cell::reveal(){//reveals cell (obvious)
    if(!revealed && !opened){
        revealed = true;
    }
}

void Cell::visit(){ //visits cell in A.I solution (still obvious)
    visitedAI = true;
}

void Cell::reset()
{ //resets all data of cell (really obvious)
    flagged = 0;
    mined = 0;
    opened = 0;
    revealed = 0;
    visitedAI = 0;
    safeAI = 0;
    countAI = 0;
    count = 0;
    mouseHover = 1;
}

void Cell::setSafetyOfCell(){ //makes the cell safe to have a mine during A.I solution
    safeAI = true;
}

void Cell::setUnsafetyOfCell(){ //makes it unsafe to have a mine
    safeAI = false;
}

void Cell::setMine() //place a mine in the cell
{
    mined = 1;
}

void Cell::setMouseHover(unsigned char currMouseState)
{
    mouseHover = currMouseState;
}

void Cell::decreaseMineCountAI(){ //lower the expected minecount (used in A.I solution)
    countAI--;
}

bool Cell:: checkSafetyOfCell(vector<Cell> &cells){ //check if the cell can have a mine or not
    
    if(cellIndexDoesntExist(xIndex, yIndex)){
        return false;
    }
    
    for (char a = -1; a < 2; a++){
        for (char b = -1; b < 2; b++){
            if(!cellIndexDoesntExist(a+xIndex, b+yIndex)){
                if (get_cell(a + xIndex, b + yIndex, cells)->getMineCountAI() - 1 < 0)
                    return false;
            }
        }
    }

    for (char a = -1; a < 2; a++){
        for (char b = -1; b < 2; b++){
            if(cellIndexDoesntExist(a+xIndex, b+yIndex)){
                continue;
            }
            get_cell(a + xIndex, b + yIndex, cells)->decreaseMineCountAI(); //if it can, we assume it does and decrease the expected minecount of surrounding cells by one
        }
    }
    return true;
}

void Cell:: unvisit(){
    visitedAI = false; //unvisit the cell (many obvious comments here)
}