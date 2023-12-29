#include <SFML/Graphics.hpp>
#include <iostream>

//////////////////////////////////////////////////
//////////////////////////////////////////////////

enum Tile {NONE, RED, YELLOW};

//////////////////////////////////////////////////
//////////////////////////////////////////////////

// Classes

//////////////////////////////////////////////////
//////////////////////////////////////////////////

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

    if (counter >= 4)
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

    if (counter >= 4)
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

    if (counter >= 4)
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

    if (counter >= 4)
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

//////////////////////////////////////////////////
//////////////////////////////////////////////////

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 630), "Connect 4", sf::Style::Close, settings);

    sf::Font font;
    if (!font.loadFromFile("andlso.ttf"))
    {
        std::cout << "Failed to load resources.\n\n";
        return 0;
    }

    sf::RectangleShape background(sf::Vector2f(800, 630));
    background.setFillColor(sf::Color(138,138,138));

    sf::RectangleShape overlay(sf::Vector2f(630, 630));
    overlay.setFillColor(sf::Color(150, 150, 150, 150));
    sf::Text ggText;
    ggText.setFont(font);
    ggText.setCharacterSize(80);
    ggText.setColor(sf::Color::Black);

    sf::RectangleShape board(sf::Vector2f(630, 630));
    board.setFillColor(sf::Color(50, 50, 255));
    board.setOutlineColor(sf::Color::Black);
    board.setOutlineThickness(5);

    sf::RectangleShape skipButton(sf::Vector2f(150, 50));
    skipButton.setFillColor(sf::Color(255, 150, 0));
    skipButton.setPosition(649, 579);
    skipButton.setOutlineColor(sf::Color::Black);
    skipButton.setOutlineThickness(1);
    sf::Text skipText;
    skipText.setFont(font);
    skipText.setString("Skip Turn");
    skipText.setCharacterSize(35);
    skipText.setColor(sf::Color::Black);
    skipText.setPosition(655, 582);

    sf::RectangleShape restart(sf::Vector2f(150, 50));
    restart.setFillColor(sf::Color(255, 150, 0));
    restart.setPosition(649, 1);
    restart.setOutlineColor(sf::Color::Black);
    restart.setOutlineThickness(1);
    sf::Text restartText;
    restartText.setFont(font);
    restartText.setString("Restart");
    restartText.setCharacterSize(35);
    restartText.setColor(sf::Color::Black);
    restartText.setPosition(675, 2);

    sf::CircleShape circle;
    circle.setRadius(38);
    circle.setPointCount(300);
    circle.setOutlineThickness(-3);
    circle.setOutlineColor(sf::Color::Black);

    Tile grid[49];

    for (int i = 0; i < 49; i++)
        grid[i] = NONE;

    bool gameOver = false, tileDropped = false, currentRed = true;
    int column = 0, newTile = 0, hoverTile = 0, moveCounter = 0;
    Tile winner = NONE;

    //////////////////////////////////////////////////
    //////////////////////////////////////////////////

    while (window.isOpen())
    {
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
                else if (skipButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && !gameOver)
                {
                    currentRed = !currentRed;
                }
                else if (restart.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                {
                    for (int i = 0; i < 49; i++)
                        grid[i] = NONE;
                    gameOver = false;
                    tileDropped = false;
                    currentRed = true;
                    moveCounter = 0;
                    winner = NONE;
                }
            }
        }

        //////////////////////////////////////////////////
        //////////////////////////////////////////////////

        if (tileDropped)
        {
            tileDropped = false;
            newTile = dropLocation(grid, column);
            if (newTile != -1)
            {
                grid[newTile] = static_cast<Tile>(2 - currentRed);
                currentRed = !currentRed;
                tileDropped = false;
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

        //////////////////////////////////////////////////
        //////////////////////////////////////////////////

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

        if (gameOver)
        {
            if (winner == RED)
            {
                overlay.setFillColor(sf::Color(255, 0, 0, 150));
                ggText.setString("Red Wins!");
                ggText.setPosition(150, 260);
            }
            else if (winner == YELLOW)
            {
                overlay.setFillColor(sf::Color(255, 255, 0, 150));
                ggText.setString("Yellow Wins!");
                ggText.setPosition(90, 260);
            }
            else
            {
                overlay.setFillColor(sf::Color(150, 150, 150, 150));
                ggText.setString("Draw!");
                ggText.setPosition(180, 260);
            }
            window.draw(overlay);
            window.draw(ggText);
        }
        else
        {
            if (skipButton.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
                skipButton.setFillColor(sf::Color(255, 200, 0));
            else
                skipButton.setFillColor(sf::Color(255, 150, 0));
            window.draw(skipButton);
            window.draw(skipText);
        }

        if (restart.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
            restart.setFillColor(sf::Color(255, 200, 0));
        else
            restart.setFillColor(sf::Color(255, 150, 0));
        window.draw(restart);
        window.draw(restartText);

        window.display();
    }

    //////////////////////////////////////////////////
    //////////////////////////////////////////////////

    return 0;
}