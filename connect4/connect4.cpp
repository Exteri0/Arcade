#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
        
enum Tile {NONE, RED, YELLOW};

// Functions

bool victoryCheck(Tile *grid, int newTile)
{
    int maxLeft = newTile % 7,
        maxRight = 6 - (newTile % 7),
        maxUp = newTile / 7,
        maxDown = 6 - newTile / 7,
        counter = 1;
    Tile color = static_cast<Tile>(grid[newTile]);

    // Horizontal
    for (int i = 1; i <= maxLeft; i++)
        if (grid[newTile - i] == color)
            counter++;
        else
            break; // Out of the for loop.

    for (int i = 1; i <= maxRight; i++)
        if (grid[newTile + i] == color)
            counter++;
        else
            break;

    if (counter == 4)
        return true;

    // Vertical
    counter = 1;
    for (int i = 1; i <= maxUp; i++)
        if (grid[newTile - 7*i] == color)
            counter++;
        else
            break; // Out of the for loop.

    for (int i = 1; i <= maxDown; i++)
        if (grid[newTile + 7*i] == color)
            counter++;
        else
            break;

    if (counter == 4)
        return true;

    // Diagonal: Top-left
    counter = 1;
    for (int i = 1; i <= maxLeft && i <= maxUp; i++)
        if (grid[newTile - 8*i] == color)
            counter++;
        else
            break; // Out of the for loop.

    for (int i = 1; i <= maxRight && i <= maxDown; i++)
        if (grid[newTile + 8*i] == color)
            counter++;
        else
            break;

    if (counter == 4)
        return true;

    // Diagonal: Bottom-left
    counter = 1;
    for (int i = 1; i <= maxLeft && i <= maxDown; i++)
        if (grid[newTile + 6*i] == color)
            counter++;
        else
            break; // Out of the for loop.

    for (int i = 1; i <= maxRight && i <= maxUp; i++)
        if (grid[newTile - 6*i] == color)
            counter++;
        else
            break;

    if (counter == 4)
        return true;

    // No win
    return false;
}

int dropLocation(Tile *grid, int column)
{
    int i = 6;
    do
    {
        if (grid[column + 7*i] == NONE)
            return column + 7*i;
        i--;
    } while (i >= 0);

    return -1;
}

int dropRandomLocation(Tile *grid)
{
    int i = 6,column = rand()%8;
    do
    {
        if (grid[column + 7*i] == NONE)
            return column + 7*i;
        i--;
    } while (i >= 0);

    return -1;
}

int scorepos(Tile *grid, int newTile){
    int maxLeft = newTile % 7, maxRight = 6 - (newTile % 7),maxUp = newTile / 7,maxDown = 6 - newTile / 7,counter = 0,score = 0,bestcounter = -1;
    Tile color = static_cast<Tile>(grid[newTile]);

    // Horizontal scoring
    counter = 1;
    for (int i = 1; i <= maxLeft; i++)
        if (grid[newTile - i] == color)
            counter++;
        else
            break; // Out of the for loop.

    for (int i = 1; i <= maxRight; i++)
        if (grid[newTile + i] == color)
            counter++;
        else
            break;
    bestcounter = std::max(bestcounter,counter);

    // Vertical
    counter = 1;
    for (int i = 1; i <= maxUp; i++)
        if (grid[newTile - 7*i] == color)
            counter++;
        else
            break; // Out of the for loop.

    for (int i = 1; i <= maxDown; i++)
        if (grid[newTile + 7*i] == color)
            counter++;
        else
            break;
    bestcounter = std::max(bestcounter,counter);

    // Diagonal: Top-left
    counter = 1;
    for (int i = 1; i <= maxLeft && i <= maxUp; i++)
        if (grid[newTile - 8*i] == color)
            counter++;
        else
            break; // Out of the for loop.

    for (int i = 1; i <= maxRight && i <= maxDown; i++)
        if (grid[newTile + 8*i] == color)
            counter++;
        else
            break;
    bestcounter = std::max(bestcounter,counter);

    // Diagonal: Bottom-left
    counter = 1;
    for (int i = 1; i <= maxLeft && i <= maxDown; i++)
        if (grid[newTile + 6*i] == color)
            counter++;
        else
            break; // Out of the for loop.

    for (int i = 1; i <= maxRight && i <= maxUp; i++)
        if (grid[newTile - 6*i] == color)
            counter++;
        else
            break;
    bestcounter = std::max(bestcounter,counter);

    if (bestcounter == 4)
        score += 100;
    else if (bestcounter == 3)
        score += 50;
    else if (bestcounter == 2)
        score += 10;
    else if (bestcounter == 1)
        score += 5;
    
    return score;
}

std::vector<int> getValidLocations(Tile *grid){
    std::vector<int> getValidLocations;
    for (int column =0;column < 7;column++){
        int loc = dropLocation(grid,column);
        if (loc != -1) getValidLocations.push_back(loc);
    }
    return getValidLocations;
}

int getBestMove(Tile *grid,bool currentRed){
    std::vector<int> validLocations = getValidLocations(grid);
    int bestscore = 0,bestloc = 0;
    for (auto &loc : validLocations){
        Tile temp_grid[49];
        std::copy(grid, grid+49, std::begin(temp_grid));
        
        temp_grid[loc] = static_cast<Tile>(2-currentRed);
        int score = scorepos(temp_grid, loc);
        if (score > bestscore){
            bestscore = score;
            bestloc = loc;
        }
    }
    return bestloc;
}


int main()
{
    // This sets firstly the settings which creats the tab, and the tab is called Connect 4

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 630), "Connect 4", sf::Style::Close, settings);

    // This is importing the font  and checking if it is there or not

    sf::Font font;
    if (!font.loadFromFile("andlso.ttf"))
    {
        std::cout << "Failed to load resources.\n\n";
        return 0;
    }
    sf::Text ggText;
    ggText.setFont(font);
    ggText.setCharacterSize(80);
    ggText.setColor(sf::Color::Black);
    
    // Creates the background with the same size of the window and colors it Gray

    sf::RectangleShape background(sf::Vector2f(800, 630));
    background.setFillColor(sf::Color(138,138,138));
    
    // We create a board that is 630x630 leaving 

    sf::RectangleShape board(sf::Vector2f(630, 630));
    board.setFillColor(sf::Color(50, 50, 255));
    board.setOutlineColor(sf::Color::Black);
    board.setOutlineThickness(5);

    // Restart yellow box positioned in the middle of the remaining area (This box will also act as a button) 

    sf::RectangleShape restart(sf::Vector2f(150, 50));
    restart.setFillColor(sf::Color(255, 150, 0));
    restart.setPosition(640, 300);
    restart.setOutlineColor(sf::Color::Black);
    restart.setOutlineThickness(1);

    // Restart text positioned inside the box

    sf::Text restartText;
    restartText.setFont(font);
    restartText.setString("Restart");
    restartText.setCharacterSize(35);
    restartText.setColor(sf::Color::Black);
    restartText.setPosition(664, 304);

    sf::CircleShape circle;
    circle.setRadius(38);
    circle.setPointCount(300);
    circle.setOutlineThickness(-3);
    circle.setOutlineColor(sf::Color::Black);

    Tile grid[49];

    for (int i = 0; i < 49; i++)
        grid[i] = NONE;

    bool gameOver = false, tileDropped = rand() % 2, currentRed = rand() % 2; // If currentRed is changed then the color changes
    int column = -1, newTile = 0, hoverTile = 0, moveCounter = 0;
    Tile winner = NONE;


    while (window.isOpen())
    {

        // This clears the window and redraws the new board 

        window.clear();

        window.draw(background);
        window.draw(board);

        for (int i = 0; i < 49; i++)
        {
            circle.setPosition(7 + 90 * (i % 7), 7 + 90 * (i / 7));
            if (grid[i] == RED)
                circle.setFillColor(sf::Color::Red);
            else if (grid[i] == YELLOW)
                circle.setFillColor(sf::Color::Yellow);
            else
                circle.setFillColor(sf::Color(138,138,138));
            window.draw(circle);
        }

        // Colors the tile on the boards

        if (sf::Mouse::getPosition(window).x < 630)
        {
            hoverTile = dropLocation(grid, sf::Mouse::getPosition(window).x / 90);
            if (hoverTile != -1)
            {
                circle.setPosition(7 + 90 * (hoverTile % 7), 7 + 90 * (hoverTile / 7));
                if (currentRed)
                    circle.setFillColor(sf::Color::Red + sf::Color(0, 75, 75));
                else
                    circle.setFillColor(sf::Color::Yellow + sf::Color(0, 0, 75));
                window.draw(circle);
            }
        }

        // Gameover messages and restarting

        if (gameOver)
        {
            if (winner == RED)
            {
                background.setFillColor(sf::Color(255, 0, 0, 150));
                ggText.setString("Red Wins!");
                ggText.setPosition(150, 260);
            }
            else if (winner == YELLOW)
            {
                background.setFillColor(sf::Color(255, 255, 0, 150));
                ggText.setString("Yellow Wins!");
                ggText.setPosition(90, 260);
            }
            else
            {
                background.setFillColor(sf::Color(150, 150, 150, 150));
                ggText.setString("Draw!");
                ggText.setPosition(180, 260);
            }
            window.draw(background);
            window.draw(ggText);
        }

        if (restart.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
            restart.setFillColor(sf::Color(255, 200, 0));
        else
            restart.setFillColor(sf::Color(255, 150, 0));
        window.draw(restart);
        window.draw(restartText);

        window.display();

        // This part is not important to understand
        // All it does is that it tracks whenever the mouse is button is pressed as an event, and it has 3 cases. 
        // First is that if the window is closed (exited)
        // Second is that if a tile is dropped
        // Third is that when the player clicks restart it resets the grid and starts the game over

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                if (event.mouseButton.x < 630 && !gameOver)
                {
                    tileDropped = true;
                    column = event.mouseButton.x / 90;
                }
                else if (restart.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                {
                    for (int i = 0; i < 49; i++)
                        grid[i] = NONE;
                    gameOver = false;
                    tileDropped = rand() % 2;
                    currentRed = rand() % 2;
                    moveCounter = 0;
                    winner = NONE;
                    column = -1;
                }
            }
        }

        ////////////////////////////////////

        if (tileDropped)
        {
            tileDropped = false;
            if (column == -1){
                newTile = dropRandomLocation(grid);
                grid[newTile] = static_cast<Tile>(2-currentRed);
                currentRed = !currentRed;
                moveCounter++;
            }
            else {
                newTile = dropLocation(grid, column);
                if (newTile != -1)
                {
                    grid[newTile] = static_cast<Tile>(2 - currentRed);
                    currentRed = !currentRed;
                    moveCounter++;
                    std::cout << victoryCheck(grid,newTile) << ' ';
                    if (moveCounter >= 49)
                    {
                        gameOver = true;
                        winner = NONE;
                    }
                    else if (victoryCheck(grid, newTile))
                    {
                        gameOver = true;
                        winner = grid[newTile];
                    }
                    else {
                        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(300));

                        newTile = getBestMove(grid,currentRed);
                        std::cout << newTile << '\n';
                        grid[newTile] = static_cast<Tile>(2-currentRed);
                        currentRed = !currentRed;
                        moveCounter++;

                        if (moveCounter >= 49)
                        {
                            gameOver = true;
                            winner = NONE;
                        }
                        else if (victoryCheck(grid, newTile))
                        {
                            gameOver = true;
                            winner = grid[newTile];
                        }
                    }
                }
            }
        }

        

    }

    return 0;
}