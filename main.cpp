#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <windows.h>

int main() {
    // Create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Arcadia");

    // Create font for text
    sf::Font font1;
    sf::Font font2;
    if (!font1.loadFromFile("AovelSansRounded-rdDL.ttf")) {
        std::cerr << "Failed to load font 1." << std::endl;
        return 1;
    }
    // Creating title string

    sf::Text title;
    title.setString("Arcadia");
    title.setFont(font1);
    title.setCharacterSize(70);
    title.setPosition(300,0);




    if (!font2.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font 2." << std::endl;
        return 1;
    }

    // Create buttons
    sf::RectangleShape buttons[5];
    sf::Text buttonText[5];
    std::string names[] = {"Minesweeper", "Connect 4", "Tower of Hanoi"};

    for (int i = 0; i < 3; ++i) {
        buttons[i].setSize(sf::Vector2f(200, 50));
        buttons[i].setPosition(50, 100 + i * 70);
        buttons[i].setFillColor(sf::Color::Green);

        buttonText[i].setFont(font2);
        buttonText[i].setString(names[i]);
        buttonText[i].setCharacterSize(25);
        buttonText[i].setFillColor(sf::Color::White);
        buttonText[i].setPosition(70, 115 + i * 70);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Check for button clicks
            if (event.type == sf::Event::MouseButtonReleased) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (buttons[0].getGlobalBounds().contains(sf::Vector2f(mousePos))) {
                    // Run Minesweeper game
                    system("cd \"code Minesweeper\" && start game.exe");
                }
                if (buttons[1].getGlobalBounds().contains(sf::Vector2f(mousePos))) {
                    // Run Connect4 game
                    system("cd connect4 && start game.exe");
                }
                if (buttons[2].getGlobalBounds().contains(sf::Vector2f(mousePos))) {
                    // Run Tower of Hanoi game
                    system("cd hanoi && start game.exe");
                }
            }
        }

        sf::Color background = sf::Color(124,223,243,100);

        sf::Texture texture;
        window.clear(background);
        if (!texture.loadFromFile("bg2.gif"))
        {
            std::cout << "couldnt load image" << std::endl;
        }
        else{
            sf::Sprite spr;
            spr.setTexture(texture);
            spr.setScale(sf::Vector2f(1.3f, 1.2f));
            window.draw(spr);
        }

        window.draw(title);

        // Draw buttons and text
        for (int i = 0; i < 5; ++i) {
            window.draw(buttons[i]);
            window.draw(buttonText[i]);
        }

        window.display();
    }

    return 0;
}
