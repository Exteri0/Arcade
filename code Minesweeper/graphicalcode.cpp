#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
using namespace std;
using namespace sf;

void textDraw(bool color, unsigned short xInput, unsigned short yInput, const string &usedText, RenderWindow &targetWindow)
{
    // We're gonna align the text ot the left top
    short xChar = xInput;
    short yChar = yInput;

    unsigned char width;

    Sprite textSprite;

    Texture usedFont;
    usedFont.loadFromFile("Font.png");

  // We're gonna calculate the width of the character based on the font image size
    // 96 because there are 96 character in the image  
    width = usedFont.getSize().x / 96;

    textSprite.setTexture(usedFont);

    // We can use this to change the color of the text
    if (color)
    {
        textSprite.setColor(Color(0, 0, 0));
    }

    for (const char a : usedText)
    {
        if ('\n' == a)
        {
            // After every newline we put increase the y-coordinate and reset the x-coordinate
            xChar = xInput;
            yChar += usedFont.getSize().y;

            continue;
        }

        // Change the position of the next character
        textSprite.setPosition(xChar, yChar);
        // Pick the character from the font image
        textSprite.setTextureRect(IntRect(width * (a - 32), 0, width, usedFont.getSize().y));

        // Increase the x-coordinate
        xChar += width;

        // Draw the character
        targetWindow.draw(textSprite);
    }
}