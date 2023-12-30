#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include "constants.hpp"

using namespace sf;

int main()
{
    unsigned lag = 0; //In order to allow the game regardless of frames per second
    std::chrono::time_point<std::chrono::steady_clock> previous_time; //another lag reduction method.
    Event event;
    RenderWindow gameWindow(VideoMode(cellSize * columns * screenResize, screenResize * (fontHeight + cellSize * columns)), "MinesweeperGame", Style::Close);
    gameWindow.setView(View(FloatRect(0, 0, cellSize * columns, fontHeight + cellSize * rows)));
     Fieldobj;

}