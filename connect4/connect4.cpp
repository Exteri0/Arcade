#include <SFML/Graphics.hpp>
#include <iostream>
#include <utility>
#include <tuple>
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

int dropLocation(Tile *grid, int column) // Edits the grid and returns the first vertically free location.
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

int dropRandomLocation(Tile *grid) // Used to pick a random column to insert, using dropLocation.
{
    int i = 6,column = rand()%8; // Random number from 0-7
    return dropLocation(grid, column);
}


int evaluation(int counter,bool isAI){ // Evaluation of the board.
    int score = 0;
    // If we are evaluating AI then the score will be positive.
    if (isAI){ 
        if (counter == 4) // If there are 4 in a row the score is large as that is an immediate win
            score += 1000000;
        else if (counter == 3)
            score += 5;
        else if (counter == 2)
            score += 2;
    }
    // If we are evaluating Player then the score will be in negative.
    else {
        if (counter == 4)
            score -= 1000000;
        else if (counter == 3)
            score -= 50;
    }
    return score; // Returns final score
}

int evalPart(std::vector<Tile> arr,Tile color){
    Tile prevcolor = NONE;
    int counter = 0, score = 0;
    if (arr.size() <= 3) return 0;
    for (int i =0;i <= arr.size()-3;i++){
        int empty = 0,AIPiece = 0,PlayerPiece = 0;
        for (int j = i;j < i+4; j++){
            if (arr[j] == NONE) empty++;
            else if (arr[j] == color) AIPiece++;
            else PlayerPiece++;
        }
        if ((AIPiece == 4) || (AIPiece == 3 && empty == 1) || (AIPiece == 2 && empty == 2)) score += evaluation(AIPiece,1);
        else if ((PlayerPiece == 4)||(PlayerPiece == 3 && empty == 1)) score += evaluation(PlayerPiece,0);
    }
    return score;
}


int scorepos(Tile *grid, Tile color){
    int counter = 0,score = 0;


    // Middle Column scoring
    
    std::vector<Tile> arr;
    for (int i = 0;i < 7;i++){
        arr.push_back(grid[7*i+3]);
    }
    score += evalPart(arr,color) * 3;
   

    // Horizontal scoring
    for (int i = 6;i >= 0;i--){
        std::vector<Tile> arr;
        std::copy(grid+i*7,grid+(i+1)*7,std::back_inserter(arr));
        score += evalPart(arr,color);
    }
    
    // Vertical scoring
    Tile prevcolor = NONE;
    counter = 0;
    bool won = false;
    for (int i = 0;i <= 6;i++){
        std::vector<Tile> arr;
        for (int j =6;j >= 0;j--){
            arr.push_back(grid[7*j+i]);
        }
        score += evalPart(arr,color);
    }
    // Diagonal: Top-right
    for (int i =1;i <= 6;i++){
        std::vector <Tile> arr;
        for (int j = 0;j <= i;j++){
            arr.push_back(grid[7*(i-j)+j]);
        }
        score += evalPart(arr,color);

        arr.clear();
        for (int j = 0;j < (7-i);j++){
            arr.push_back(grid[7*(6-j)+j+i]);
        }
        score += evalPart(arr,color);
    }
    
    // Diagonal: Top-left
    for (int i =1;i <= 6;i++){
        std::vector <Tile> arr;
        for (int j = 0;j <= i;j++){
            arr.push_back(grid[7*(i-j)+6-j]);
        }
        score += evalPart(arr,color);

        arr.clear();
        for (int j = 0;j < (7-i);j++){
            arr.push_back(grid[7*(6-j)+(6-i-j)]);
        }
        score += evalPart(arr,color);
    }
    
    return score;
}
// Returns an array of all valid locations
std::vector<int> getValidLocations(Tile *grid){
    std::vector<int> getValidLocations;
    for (int column =0;column < 7;column++){
        int loc = dropLocation(grid,column);
        if (loc != -1) getValidLocations.push_back(loc);
    }
    return getValidLocations;
}
// Returns best move location for the current board 
int getBestMove(Tile *grid,Tile color){
    std::vector<int> validLocations = getValidLocations(grid);
    int bestscore = -10000,bestloc = dropRandomLocation(grid);
    for (auto &loc : validLocations){
        Tile temp_grid[49];
        std::copy(grid, grid+49, std::begin(temp_grid));
        
        temp_grid[loc] = color;
        int score = scorepos(temp_grid, color);
        if (score > bestscore){
            bestscore = score;
            bestloc = loc;
        }
    }
    return bestloc;
}
// This is the minimax algorithm which changes the board and then tries all valid plays for both player and AI to find the best option based on the score 
std::pair<int,int> minimax(Tile *grid, int depth, Tile AI, bool maximizingPlayer,int moveCounter, int newTile){
    std::vector<int> validLocations = getValidLocations(grid); // Get all valid locations
    
    if (victoryCheck(grid,newTile)){ // If won, drawn, or the depth is finished the function is stopped.
        if (grid[newTile] == AI) return {-1,1000000};
        else return {-1,-1000000};
    }    
    else if (moveCounter >= 49) return {-1,0};
    else if (depth == 0) return {-1,scorepos(grid,AI)};

    // If you are maximizing this player - The AI's turn
    if (maximizingPlayer){
        int value = -1000000,bestloc = dropRandomLocation(grid);

        for (auto &loc : validLocations){
            Tile temp_grid[49];
            std::copy(grid, grid+49, std::begin(temp_grid));

            temp_grid[loc] = AI;
            int newscore = minimax(temp_grid, depth-1, AI, 0,moveCounter++,loc).second;
            if (newscore > value){
                value = newscore;
                bestloc = loc;
            }
        }
        return {bestloc,value};
    }
    else {
        // You are trying to minimize this player - This is the player
        int value = 1000000,bestloc = dropRandomLocation(grid);

        for (auto &loc : validLocations){
            Tile temp_grid[49];
            std::copy(grid, grid+49, std::begin(temp_grid));
            
            temp_grid[loc] = (AI == RED) ? YELLOW : RED; 
            int newscore = minimax(temp_grid, depth-1, AI, 1,moveCounter++,loc).second;
            if (newscore < value){
                value = newscore;
                bestloc = loc;
            }
        }
        return {bestloc,value};
    }
}


int main()
{
    // This sets firstly the settings which creats the tab, and the tab is called Connect 4

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 630), "Connect 4", sf::Style::Close, settings);

    // This is importing the font and checking if it is there or not

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
    int column = -1, newTile = 0, hoverTile = 0,moveCounter = 0,prevtile = -1;
    Tile winner = NONE;
    Tile AI = NONE;
    if ((tileDropped == 1 && currentRed == 1) || (tileDropped == 0 && currentRed == 0)) AI = RED;
    else if ((tileDropped == 1 && currentRed == 0) || (tileDropped == 0 && currentRed == 1)) AI = YELLOW;

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

        ////////////////////////////////////////////////////////////////////////////////////////
        // This is the part that explains everything to do with the user and the computer playing
        if (tileDropped)
        {
            tileDropped = false;
            // If the start is the computer then the computer enters here.
            if (column == -1){
                // As it is the first move it is inimportant to look into the future, therefore we use getBestMove which only analysis the current board.
                newTile = getBestMove(grid,static_cast<Tile>(2-currentRed));
                grid[newTile] = static_cast<Tile>(2-currentRed);
                prevtile = newTile;
                currentRed = !currentRed;
                moveCounter++;
            }
            // This is the part where the program functions
            else {
                //  We store the droplocation of player in newTile
                newTile = dropLocation(grid, column);
                if (newTile != -1)
                {
                    // Drops the tile and increments the move counter then checks if player won or not
                    grid[newTile] = static_cast<Tile>(2 - currentRed);
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
                    else {
                        // Delay for user experience
                        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(300));

                        // Right after the user plays the computer's minimax function is entered 
                        newTile = minimax(grid,4,static_cast<Tile>(2-currentRed),1,moveCounter,prevtile).first;
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