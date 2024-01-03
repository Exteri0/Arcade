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
        // Here we're calculating the lag
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
            while (gameWindow.pollEvent(event))
            {
                if (event.type == Event::Closed)
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
                    else if (event.key.code == Keyboard::A)
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
                else if (event.type == Event::MouseButtonReleased)
                {
                    switch (event.mouseButton.button)
                    {
                    case Mouse::Left:
                    {
                        gameObj.openCell(xIndexMouse, yIndexMouse);
                    }
                    case Mouse::Right:
                    {
                        gameObj.flagCell(xIndexMouse, yIndexMouse);
                    }
                    }
                }
            }
            if (Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right))
            {
                gameObj.setMouseState(2, xIndexMouse, yIndexMouse);
            }
            else
            {
                gameObj.setMouseState(1, xIndexMouse, yIndexMouse);
            }
            if (frameTime > lag)
            {
                gameWindow.clear();

                if (!gameAlreadyDone)
                    gameObj.draw(gameWindow);

                if (gameObj.endGame() == -1)
                {
                    textDraw(0, 0, cellSize * rows, "You Lose!\t Mines Left: " + to_string(numMines - flagCount), gameWindow);
                    gameObj.revealBoard(gameWindow);
                    gameAlreadyDone = true;
                }
                else if (gameObj.endGame() == 1 && !gameAlreadyDone)
                {
                    textDraw(0, 0, cellSize * rows, "You Win!\t Mines Left:" + to_string(numMines - flagCount), gameWindow);
                    gameAlreadyDone = true;
                }
                // How many mines are left?
                else
                {
                    textDraw(0, 0, cellSize * rows, "Mines:" + to_string(numMines - gameObj.getFlags()), gameWindow);
                }
                if (!AIMODE)
                {
                    gameWindow.display();
                }
            }
        }
    }
}