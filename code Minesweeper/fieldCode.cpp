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

    pictureTexture.loadFromFile("Icons16.png"); //used in junction with the screenresize variable, halfing the screen resize would require using icons8.png

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
    flagCount = 0;
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

void minesweeperField::revealBoard(RenderWindow &targetWindow){
    
    RectangleShape cellBox(Vector2f(cellSize - 1, cellSize - 1));

    Sprite pictureSprite;
    Texture pictureTexture;

    pictureTexture.loadFromFile("Icons16.png");

    pictureSprite.setTexture(pictureTexture);
    for (char a = 0; a < columns; a++){
        for (char b = 0; b < rows; b++){
            Cell *currCell = get_cell(a, b, cells);
            cellBox.setPosition(static_cast<float>(cellSize * a), static_cast<float>(cellSize * b));
            if(!currCell->getOpen() &&!currCell->getFlag()){
                currCell->reveal();
                if(!currCell->checkMine()){
                    cellBox.setFillColor(Color(131, 250, 248)); //color after revealing a safe cell
                    unsigned char mineCount = currCell->getMineCount();
                    targetWindow.draw(cellBox);

                    // If the cell has at least one mine around it
                    if (mineCount > 0){
                        // We take the number from the icons' texture and draw it
                        pictureSprite.setPosition(static_cast<float>(cellSize * a), static_cast<float>(cellSize * b));
                        pictureSprite.setTextureRect(IntRect(cellSize * mineCount, 0, cellSize, cellSize));

                        targetWindow.draw(pictureSprite);
                    }
                }
                else{
                    cellBox.setFillColor(Color(100, 100, 100));
                    targetWindow.draw(cellBox);
                }                
            }
            else if(!currCell->getFlag()){
                unsigned char mineCount = get_cell(a, b, cells)->getMineCount();
                if (!get_cell(a, b, cells)->checkMine()){
                    cellBox.setFillColor(Color(200, 200, 200)); 
                }
                else{
                    cellBox.setFillColor(Color(0, 0, 0));
                }

                targetWindow.draw(cellBox);

                if (mineCount > 0){
                    pictureSprite.setPosition(static_cast<float>(cellSize * a), static_cast<float>(cellSize * b));
                    pictureSprite.setTextureRect(IntRect(cellSize * mineCount, 0, cellSize, cellSize));

                    targetWindow.draw(pictureSprite);
                }
                

            }
            else{
                cellBox.setFillColor(Color(246, 124, 18));
                targetWindow.draw(cellBox);
                pictureSprite.setPosition(static_cast<float>(cellSize * a), static_cast<float>(cellSize * b));
                pictureSprite.setTextureRect(IntRect(0, 0, cellSize, cellSize));

                targetWindow.draw(pictureSprite);
            }
        }
    }
}

void minesweeperField::drawAIBoard(RenderWindow &targetWindow){  //same jigg as the normal board drawing, each cell has an index (the start of the rectangular box)
//cell color this time is all the same since all cells are revealed
// numbers are now from 0 to 9
    RectangleShape cellBox(Vector2f(cellSize - 1, cellSize - 1));
    Sprite pictureSprite;
    Texture pictureTexture;
    pictureTexture.loadFromFile("Icons16.png");
    pictureSprite.setTexture(pictureTexture);
    for (char a = 0; a < columns; a++){
        for (char b = 0; b < rows; b++){
            Cell *currCell = get_cell(a, b, cells);
            cellBox.setPosition(static_cast<float>(cellSize * a), static_cast<float>(cellSize * b));
            char mineCount = currCell->getMineCountAI();
            cellBox.setFillColor(Color(200, 200, 200));
            targetWindow.draw(cellBox);
            if(mineCount > 0){
                pictureSprite.setPosition(static_cast<float>(cellSize * a), static_cast<float>(cellSize * b));
                pictureSprite.setTextureRect(IntRect(cellSize * mineCount, 0, cellSize, cellSize));
                targetWindow.draw(pictureSprite);
            }
        }
    }
}

pair<bool, pair<int,int>> minesweeperField:: checkForUnvisited(){ //find the first unvisited node by looping over all indices until an unvisited is found
    for (char a = 0; a < columns; a++){
        for (char b = 0; b < rows; b++){
            Cell *currCell = get_cell(a, b, cells);
            if(!currCell->checkVisited()){
                return (make_pair(true, make_pair(a, b)));
            }
        }
    }
    return (make_pair(false, make_pair(0, 0)));
}

void minesweeperField::generateAIBoard(){ //similiar to main board generation, without first click stuff
    for (char a = 0; a < columns; a++){
        for (char b = 0; b < rows; b++){
            get_cell(a, b, cells)->reset();
        }
    }

    for (char i = 0; i < numMines; i++){
        char xIndexMine, yIndexMine;
        xIndexMine = rand() % columns;
        yIndexMine = rand() % rows;
        if (get_cell(xIndexMine, yIndexMine ,cells)->checkMine()){
            i--;
        }
        else{
            get_cell(xIndexMine, yIndexMine, cells)->setMine();
        }
    }
    for (char a = 0; a < columns; a++){
        for (char b = 0; b < rows; b++){
            get_cell(a, b, cells)->countMinesAI(cells);
        }
    }
    copy(cells.begin(),cells.end(),back_inserter(cellsAIcpy));
}

bool minesweeperField::solutionOverAI(){ //check if the ai solution is done
    bool returnVal = true;
    for (char a = 0; a < columns; a++){
        for (char b = 0; b < rows; b++){
            Cell *currCell = get_cell(a, b, cells);
            returnVal = returnVal && (currCell->getMineCountAI() == 0) && (currCell->checkVisited());
        }
    }
    return returnVal;
}

bool minesweeperField::AISolve(){ //solution algorithm
    
    if(solutionOverAI()) //if the solution over condition, return true for the specific state of cells
        return true;
    
    pair<bool, pair<int, int>> unvisitedValue = checkForUnvisited(); //get the first unvisited node
    
    if(!unvisitedValue.first) //all nodes are visited (this happened after we know the game is not finished) --> this state of cells doesnt satisfy
        return false;
    
    char xbegin = unvisitedValue.second.first;
    char ybegin = unvisitedValue.second.second;
    Cell *currCell = get_cell(xbegin, ybegin, cells);
    currCell->visit(); //visit the current unvisited cell

    if(currCell->checkSafetyOfCell(cells)){
        currCell->setSafetyOfCell(); //cell can have a mine
        if(AISolve()) //keep solving for the next unvisited cell
            return true; // the assumption is correct, recruse back
    
        currCell->setUnsafetyOfCell(); //now assume the cell didnt have the mine
        for (char a = -1; a < 2; a++){
            for (char b = -1; b < 2; b++){
                if(currCell->cellIndexDoesntExist(xbegin+a, ybegin+b))
                    continue;
                
                Cell *tempCell = get_cell(xbegin + a, ybegin + b, cells);
                tempCell->increaseMineCountAI();//we reset the cell's neightbors numbers (they were changed in the assumption)
            }
        }
    }
        if(AISolve())
            return true;// start solving on the new assumption that it doesnt have a mine

        currCell->unvisit();//unvisit the cell in case later recursion calls need it

        return false; // both scenarios didnt work, return false.
}

void minesweeperField::AIsolutionPRINTER(sf::RenderWindow &targetWindow){ //the function just recolors the boxes depending on whether they are mines or not
    if(AISolve()){
        RectangleShape cellBox(Vector2f(cellSize - 1, cellSize - 1));
        Sprite pictureSprite;
        Texture pictureTexture;
        pictureTexture.loadFromFile("Icons16.png");
        pictureSprite.setTexture(pictureTexture);
        for (char a = 0; a < columns; a++)
        {
            for (char b = 0; b < rows; b++)
            {
                Cell *currCell = get_cell(a, b, cells);
                Cell *counterCell = get_cell(a, b, cellsAIcpy);
                cellBox.setPosition(static_cast<float>(cellSize * a), static_cast<float>(cellSize * b));
                char mineCount = counterCell->getMineCountAI();
                if(currCell->checkSafe()){
                    cellBox.setFillColor(Color(255,255,255));
                    targetWindow.draw(cellBox);
                }
                else{
                    cellBox.setFillColor(Color(131, 250, 248));
                    targetWindow.draw(cellBox);
                }
                if(mineCount > 0){
                    pictureSprite.setPosition(static_cast<float>(cellSize * a), static_cast<float>(cellSize * b));
                    pictureSprite.setTextureRect(IntRect(cellSize * mineCount, 0, cellSize, cellSize));
                    targetWindow.draw(pictureSprite);
                }
            }
        }
    }
}