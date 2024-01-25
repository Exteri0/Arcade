#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include "fieldCode.cpp"

using namespace sf;
using namespace std;

int main()
{
    unsigned lag = 0;                                       // In order to allow the game regardless of frames per second
    chrono::time_point<chrono::steady_clock> previous_time; // another lag reduction method requires this variable.
    Event event;
    RenderWindow gameWindow(VideoMode(cellSize * columns * screenResize, screenResize * (fontHeight + cellSize * columns)), "MinesweeperGame", Style::Close);
    gameWindow.setView(View(FloatRect(0, 0, cellSize * columns, fontHeight + cellSize * rows)));
    minesweeperField gameObj;
    previous_time = chrono::steady_clock::now();
    bool gameAlreadyDone = false;
    bool AIMODE = false;
    bool wait = false;

    while (gameWindow.isOpen())
    {
        // Lag Reduction.
        unsigned delta_time = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - previous_time).count();

        lag += delta_time;

        previous_time += chrono::microseconds(delta_time);

        // While the lag exceeds the maximum allowed frame duration
        while (frameTime <= lag)
        {
            unsigned char xIndexMouse, yIndexMouse;
            xIndexMouse = clamp(static_cast<int>(floor(Mouse::getPosition(gameWindow).x / static_cast<float>(cellSize * screenResize))), 0, columns - 1);
            yIndexMouse = clamp(static_cast<int>(floor(Mouse::getPosition(gameWindow).y / static_cast<float>(cellSize * screenResize))), 0, rows - 1);
            lag -= frameTime;
            while (gameWindow.pollEvent(event)) // start taking actions from the user
            {
                if (event.type == Event::Closed) // user wants to close the window
                {
                    gameWindow.close();
                    break;
                }
                else if (event.type == Event::KeyReleased)
                {
                    if (event.key.code == Keyboard::Enter)
                    {
                        gameObj.restart();
                        gameAlreadyDone = false;
                    }
                    else if (event.key.code == Keyboard::A) // user presses A, starts A.I mode, user presses B, A.I solution begins, then C to close
                    {
                        AIMODE = true;
                        gameObj.restart();
                        gameObj.generateAIBoard();
                        gameObj.drawAIBoard(gameWindow);
                        gameWindow.display();
                    }
                    else if (event.key.code == Keyboard::B)
                    {
                        gameObj.AIsolutionPRINTER(gameWindow);
                        gameWindow.display();
                    }
                    else if (event.key.code == Keyboard::C)
                    {
                        AIMODE = false;
                        gameObj.restart();
                        gameWindow.close();
                    }
                }
                else if (event.type == Event::MouseButtonReleased) // this is user clicking on a square on normal mode
                {
                    switch (event.mouseButton.button)
                    {
                    case Mouse::Left:
                    {
                        gameObj.openCell(xIndexMouse, yIndexMouse); // the cell is opened on left click
                    }
                    case Mouse::Right:
                    {
                        gameObj.flagCell(xIndexMouse, yIndexMouse); // the cell is flagged on right click
                    }
                    }
                }
            }
            if (Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right)) // slightly change the color tone of the cell
            {
                gameObj.setMouseState(2, xIndexMouse, yIndexMouse);
            }
            else
            {
                gameObj.setMouseState(1, xIndexMouse, yIndexMouse);
            }
            if (frameTime > lag) // the window is cleared to be redrawn on each frame
            {
                gameWindow.clear();

                if (!gameAlreadyDone)
                    gameObj.draw(gameWindow); // the normal state, game isnt over

                if (gameObj.endGame() == -1)
                {
                    textDraw(0, 0, cellSize * rows, "You Lose!\t Mines Left: " + to_string(numMines - flagCount), gameWindow);
                    gameObj.revealBoard(gameWindow);
                    gameAlreadyDone = true; // player lost state, game is over
                }
                else if (gameObj.endGame() == 1 && !gameAlreadyDone)
                {
                    textDraw(0, 0, cellSize * rows, "You Win!\t Mines Left:" + to_string(numMines - flagCount), gameWindow);
                    gameAlreadyDone = true; // player won state, game is over
                }
                // How many mines are left?
                else
                {
                    textDraw(0, 0, cellSize * rows, "Mines:" + to_string(numMines - gameObj.getFlags()), gameWindow); // always show remaining number of mines
                }
                if (!AIMODE)
                {
                    gameWindow.display(); // draw the window
                }
            }
        }
    }
}