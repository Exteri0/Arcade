#include "towerofhanoi.h"

towerOfHanoi::towerOfHanoi()
{
    numDisks = 0;
    numRods = 3;
    startRod = 0;
    endRod = 0;
    rod = nullptr; //stack of rods
    auxRod = nullptr;
    state = PRE_GAME;
    focus = 0;
    moveInProgress = needConfirmation = doLargeValue = false;
    moveTime = milliseconds(100);

    // Window
    window.create(VideoMode::getDesktopMode(), "Towers of Hanoi");
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(true);

    // Pointers to game state run functions
    runState[PRE_GAME] = &towerOfHanoi::preGame;
    runState[IN_GAME] = &towerOfHanoi::inGame;
    runState[POST_GAME] = &towerOfHanoi::postGame;

    // Pointers to game state event functions
    processEvent[PRE_GAME] = &towerOfHanoi::preGameEvent;
    processEvent[IN_GAME] = &towerOfHanoi::inGameEvent;
    processEvent[POST_GAME] = &towerOfHanoi::postGameEvent;

    // Disk
    diskSprite = nullptr;
    for (int i = 0; i < 4; i++)
        diskTexture[i].loadFromFile("disk" + to_string(i + 1) + ".png");
    // Background
    backgroundTexture.loadFromFile("background.png");
    backgroundSprite.setTexture(backgroundTexture);

    // Foreground
    foregroundTexture.loadFromFile("foreground.png");
    foregroundSprite.setTexture(foregroundTexture);
    foregroundSprite.setPosition(0, window.getSize().y - 350);


    // Title
    font.loadFromFile("hanoi.ttf");
    title.setFont(font);
    title.setCharacterSize(190);
    title.setString("Tower of Hanoi");
    title.setFillColor(Color::White);
    title.setPosition(window.getSize().x / 2 - title.getGlobalBounds().width / 2, window.getSize().y / 10);

    // Prompt for enter
    askEnter.setFont(font);
    askEnter.setCharacterSize(40);
    askEnter.setString("Press ENTER to start");
    askEnter.setFillColor(Color::White);
    askEnter.setPosition(window.getSize().x / 2 - askEnter.getGlobalBounds().width / 2, window.getSize().y / 1.2);

    // Prompt for input text
   
        askInput[0].setFont(font);
        askInput[0].setCharacterSize(50);
        askInput[0].setFillColor(Color::White);
        askInput[0].setPosition(window.getSize().x / 2 - title.getGlobalBounds().width / 4.9, window.getSize().y / 2.5 );
   
    askInput[0].setString("Number of disks:");
   

    // Prompt for confirmation
    askConfirm.setFont(font);
    askConfirm.setCharacterSize(70);
    askConfirm.setFillColor(Color::White);
    askConfirm.setString("Please confirm: y / n");
    askConfirm.setPosition(window.getSize().x / 2 - askConfirm.getGlobalBounds().width / 2, window.getSize().y / 1.25);

    // Default input strings (number of disks, starting rod, ending rod)
    inputStr[0] = "3";
   

    // Input text (number of disks, starting rod, ending rod)
   
        inputText[0].setFont(font);
        inputText[0].setCharacterSize(50);
        inputText[0].setString(inputStr[0]);
        inputText[0].setFillColor(Color::White);
        inputText[0].setPosition(window.getSize().x / 2 + 200, window.getSize().y / 2.5 );
    

    // Move count
    userMoveCount.setFont(font);
    userMoveCount.setCharacterSize(100);
    userMoveCount.setString("0");
    userMoveCount.setFillColor(Color::White);
    userMoveCount.setPosition(30, 1);

    // Ending message
    for (int i = 0; i < 3; i++)
    {
        endMsg[i].setFont(font);
        endMsg[i].setCharacterSize(70);
        endMsg[i].setFillColor(Color::White);
    }
    endMsg[0].setString("Congratulations!");

    // Focus box
    focusBox.setFillColor(Color(255, 255, 255, 50));

    // Focus box arrows
    arrowTexture.loadFromFile("arrow.png");
    for (int i = 0; i < 2; i++)
    {
        arrowSprite[i].setTexture(arrowTexture);
        arrowSprite[i].setColor(focusBox.getFillColor());
        arrowSprite[i].setScale(1, 0.93);
    }
    arrowSprite[0].setPosition(inputText[0].getPosition().x - 57.2, inputText[0].getPosition().y + 1);
    arrowSprite[1].setScale(-1, 0.93);
}

towerOfHanoi::~towerOfHanoi()
{
    if (rod)
        delete[] rod;
    if (auxRod)
        delete[] auxRod;
    if (diskSprite)
        delete[] diskSprite;

    rod = nullptr;
    auxRod = nullptr;
    diskSprite = nullptr;
    numDisks = numRods = startRod = endRod = focus = NULL;
    moveInProgress = needConfirmation = doLargeValue = NULL;
}

void towerOfHanoi::run()
{
    while (window.isOpen())
    {
        // Handle keyboard events
        Event event;
        while (window.pollEvent(event))
            (this->*processEvent[state])(event);

        // Render window
        window.clear();
        (this->*runState[state])();
        window.display();
    }
}

void towerOfHanoi::createRods()
{
    // Create rods
    rod = new stack<int>[numRods];

    // Fill starting rod with disks
    for (int i = 0; i < numDisks; i++)
        rod[startRod].push(i + 1);
}

void towerOfHanoi::solve()
{
        startRodToEndRod();
    // Reset rods for user input

    while (!rod[endRod].empty())
        rod[endRod].pop();
    for (int i = 0; i < numDisks; i++)
        rod[startRod].push(i + 1);

}


void towerOfHanoi::startRodToEndRod()
{
    int prevDisk{}, nextRod, originalStartSize = rod[startRod].size();

    // Loop through the rods from left to right until the puzzle is solved
    for (int currRod = 0; rod[endRod].size() != originalStartSize; currRod == numRods - 1 ? currRod = 0 : currRod++)
    {
        // Cannot perform actions
        if (isInvalidRod(currRod, prevDisk) || isAuxiliaryRod(currRod))
            continue;

        // Even number disk -> attempt move left
        if ((rod[currRod].top() % 2))
        {
            nextRod = !currRod ? numRods - 1 : currRod - 1;
            while (isAuxiliaryRod(nextRod))
                nextRod--;
        }

        // Odd numbered disk -> attempt move right
        else
        {
            nextRod = (currRod + 1) % numRods;
            while (isAuxiliaryRod(nextRod))
                nextRod++;
        }

        // Next rod is empty OR is disk to move is a greater disk number
        if (rod[nextRod].empty() || rod[currRod].size() && rod[currRod].top() > rod[nextRod].top())
        {
            // Identify the disk as the prev, and move it.
            prevDisk = rod[currRod].top();
            placeDisk(currRod, nextRod);
        }
    }
}
bool towerOfHanoi::isInvalidRod(int rodNum, int prevDisk)
{
    // Current rod is empty OR on the previous disk moved
    if (rod[rodNum].empty() || (!rod[rodNum].empty() && rod[rodNum].top() == prevDisk))
        return true;
    return false;
}

bool towerOfHanoi::isAuxiliaryRod(int rodNum)
{
    for (int i = 0; (i < numRods - 3) && auxRod; i++)
        if (rodNum == auxRod[i])
            return true;
    return false;
}

void towerOfHanoi::placeDisk(int beginRod, int endRod, bool solving)
{
    // Place disk from top of beginning rod to end rod
    int disk = rod[beginRod].top();
    rod[beginRod].pop();
    rod[endRod].push(disk);

    // If solving, record instruction into solveData
    if (solving)
        solveData.push(pair<int, int>(disk, endRod));
}

void towerOfHanoi::preGame()
{
    // Draw background, foreground
    window.draw(backgroundSprite);
    window.draw(foregroundSprite);


    // Set focus box position
    if (focus < 2)
    {
        // Set position relative to focus
        focusBox.setPosition(inputText[0].getPosition().x / 1.597 + askInput[0].getGlobalBounds().width / 0.84,
            askInput[0].getGlobalBounds().top - askInput[0].getGlobalBounds().height / 2.5 + (focus * 100));

        // Set size relative to width of input text being focused
        focusBox.setSize(Vector2f(47 + inputText[focus].getGlobalBounds().width, 70));

        // Set position of arrows relative to focus
        arrowSprite[0].setPosition(arrowSprite[0].getPosition().x,
            inputText[focus].getPosition().y + 1);

        arrowSprite[1].setPosition(inputText[0].getPosition().x + inputText[focus].getGlobalBounds().width + 55.8,
            inputText[focus].getPosition().y + 1);
    }
    else
    {
        focusBox.setSize(Vector2f(60, 70));
        focusBox.setPosition(askConfirm.getGlobalBounds().width / 0.57 + (!doLargeValue * 110.5),
            askConfirm.getGlobalBounds().top - askConfirm.getGlobalBounds().height / 5.5);
    }

    // Draw focus box and arrows
    window.draw(focusBox);
    for (int i = 0; i < 2 && focus < 2; i++)
        window.draw(arrowSprite[i]);

    // Draw title
    window.draw(title);

    // Draw input text
    for (int i = 0; i < 2; i++)
    {
        window.draw(askInput[i]);
        window.draw(inputText[i]);
    }

    // Draw confirmation if necessary
    if (needConfirmation)
        window.draw(askConfirm);

    // Draw prompt enter message
    else
    {
        window.draw(askEnter);
    }
}

void towerOfHanoi::inGame()
{
    // Check for win condition
    if (rod[endRod].size() == numDisks && !moveInProgress)
    {
        setEndMessage();
        state = POST_GAME;
    }

    // Disk animation
    if (moveInProgress)
        moveDisk();

    // Draw background 
    window.draw(backgroundSprite);

    // Draw disks
    for (int i = 0; i < numDisks; i++)
        window.draw(diskSprite[i]);

    // Draw foreground 
    window.draw(foregroundSprite);


    // Draw move count
    window.draw(userMoveCount);
}

void towerOfHanoi::postGame()
{

    // Draw background 
    window.draw(backgroundSprite);

    // Draw disks
    for (int i = 0; i < numDisks; i++)
        window.draw(diskSprite[i]);

    // Draw foreground 
    window.draw(foregroundSprite);


    // Draw end message
    for (int i = 0; i < 3; i++)
        window.draw(endMsg[i]);

    // Prompt for enter
    window.draw(askEnter);
}

void towerOfHanoi::preGameEvent(Event event)
{
    // Keyboard
    if (event.type == Event::KeyPressed)
    {
        switch (event.key.code)
        {
            // Quit
        case Keyboard::Escape:
            window.close();
            cout.flush();
            break;

            // Decrement focused value
        case Keyboard::Left:

            // Set to confirm large value
            if (focus == 2)
                doLargeValue = true;
            else
                decrementInputStr(focus);
           
            break;

            // Increment focused value or set to confirm large value
        case Keyboard::Right:

            // Set to unconfirm large value or set to unconfirm large value
            if (focus == 2)
                doLargeValue = false;
            else
                incrementInputStr(focus);
          
            break;

            // Set to confirm large value
        case Keyboard::Y:
            if (focus == 2)
                doLargeValue = true;
            break;

            // Set to unconfirm large value
        case Keyboard::N:
            if (focus == 2)
                doLargeValue = false;
            break;

            // Set the board and proceed to ingame
        case Keyboard::Return:

            // Reset error status
            needConfirmation = false;

            // Unconfirm large value and clear number of disks
            if (focus == 2 && !doLargeValue)
            {
                focus = 0;
                return;
            }

            // Set the board based off of input
            numDisks = stoi(inputStr[0].toAnsiString());
            endRod = numRods - 1;

            // Large processing warning
            if ((numDisks >= 8  && !doLargeValue))
            {
                needConfirmation = true;
                focus = 2;
                return;
            }
            // No errors! Proceed to game
            createRods();
            solve();
            loadDiskSprites();
            state = IN_GAME;
            break;

     
        default:
            break;
        }
    }

}

void towerOfHanoi::inGameEvent(Event event)
{
    // Keyboard
    if (event.type == Event::KeyPressed)
    {
        switch (event.key.code)
        {
            // Quit
        case Keyboard::Escape:
            window.close();
            cout.flush();
            break;

            // Trigger a move
        case Keyboard::Space:
            if (!moveInProgress && rod[endRod].size() != numDisks)
            {
                getNextInstruction();
                moveInProgress = true;
                moveClock.restart();
            }
            break;

            default:
            break;
        }
    }
}

void towerOfHanoi::postGameEvent(Event event)
{
    // Keyboard
    if (event.type == Event::KeyPressed)
    {
        switch (event.key.code)
        {
            // Quit
        case Keyboard::Escape:
            window.close();
            cout.flush();
            break;

            // Returns to title screen
        case Keyboard::Return:
            clearSettings();
            break;
        default:
            break;
        }
    }
}

void towerOfHanoi::loadDiskSprites()
{
    diskSprite = new Sprite[numDisks];

    // Each disk is (100 - diskNum*(0.5/totalDisks)) % smaller than the next, capping at a 50 % scale of the original
    float multiplier = 0.5 / (float)numDisks;
    for (int i = 0; i < numDisks; i++)
    {
        // Set the texture, iterating from 1-4 repeatedly
        diskSprite[i].setTexture(diskTexture[i % 4]);

        // Set the origin as the middle of the first disk's dimensions
        diskSprite[i].setOrigin(300, 88);

        // Make the disks smaller in size for each disk made
        diskSprite[i].setScale(1 - (i * multiplier), 1 - (i * multiplier));
        diskSprite[i].setScale(diskSprite[i].getScale().x * (3 / ((float)numRods)), diskSprite[i].getScale().y);

        // Set the position of the disk onto the start rod
        diskSprite[i].setPosition(posX(startRod), posY(startRod));
    }
}


void towerOfHanoi::incrementInputStr(int focus)
{
    if (inputStr[focus].getSize())
    {
        int num = stoi(inputStr[focus].toAnsiString());
        if (num < 100)
        {
            num++;
            inputStr[focus] = to_string(num);
            inputText[focus].setString(inputStr[focus]);
            arrowSprite[1].setColor(Color(255, 255, 255, 200));
        }
    }
}

void towerOfHanoi::decrementInputStr(int focus)
{
    if (inputStr[focus].getSize())
    {
        int num = stoi(inputStr[focus].toAnsiString());
        if (num > 3)
        {
            num--;
            inputStr[focus] = to_string(num);
            inputText[focus].setString(inputStr[focus]);
            arrowSprite[0].setColor(Color(255, 255, 255, 200));
        }
    }
}

void towerOfHanoi::setEndMessage()
{
    // Set end message strings
    endMsg[1].setString("You did " + userMoveCount.getString() + " moves.");

    // Set prompt enter string
    askEnter.setString("Press ENTER to restart");
    askEnter.setPosition(window.getSize().x / 2 - askEnter.getGlobalBounds().width / 2, window.getSize().y / 1.2);

    // Set position
    for (int i = 0; i < 3; i++)
        endMsg[i].setPosition(window.getSize().x / 2 - endMsg[i].getGlobalBounds().width / 2, window.getSize().y / 4 + (i * 100));

}

void towerOfHanoi::moveDisk()
{
    // Pick up
    if (moveClock.getElapsedTime() < milliseconds(30))
    {
        // Move disk upwards
        diskSprite[instruction.first - 1].move(0, -5);
    }

    // Done
    else if (moveClock.getElapsedTime() >= moveTime)
    {
        // Set disk to designated end position
        diskSprite[instruction.first - 1].setPosition(posX(instruction.second), posY(instruction.second));

        // Move finished
        moveInProgress = false;

        // Increment user move count
        incrementMoveCount();
        return;
    }

    // Put down
    else if (moveClock.getElapsedTime() > milliseconds(30))
    {
        // Move disk downwards
        diskSprite[instruction.first - 1].move(0, 5);
    }
}

void towerOfHanoi::getNextInstruction()
{
    if (!solveData.empty() && !moveInProgress)
    {
        // Obtain next instruction in moveData
        instruction = solveData.front();
        solveData.pop();

        // Apply instruction to game stack
        for (int beginRod = 0; beginRod < numRods; beginRod++)
        {
            if (rod[beginRod].size() && rod[beginRod].top() == instruction.first)
                placeDisk(beginRod, instruction.second);
        }
    }
}

int towerOfHanoi::posX(int rodNum)
{
    float xPos = diskSprite[0].getGlobalBounds().width / 2. + rodNum * (diskSprite[0].getGlobalBounds().width + (3 / (numRods * 0.05)) - pow(0.5, numRods));
    float xPosFarthest = ((numRods - 1) * (diskSprite[0].getGlobalBounds().width + (3 / (numRods * 0.05)) - pow(0.5, numRods)));
    xPosFarthest += diskSprite[0].getGlobalBounds().width;
    float xDif = (window.getSize().x - xPosFarthest) / 2.;
    xPos += xDif;
    return xPos;
}

int towerOfHanoi::posY(int rodNum)
{
    // Default position (no disks)
    int yPosition = 950;

    // Find the heights of all disks in the rod and add to y position
    stack<int> temp = rod[rodNum];
    for (int i = 0; i < rod[rodNum].size(); i++)
    {
        int disk = temp.top();
        temp.pop();
        yPosition -= diskSprite[disk - 1].getGlobalBounds().height / 2.25;

    }
    return yPosition;
}

void towerOfHanoi::clearSettings()
{
    if (auxRod)
        delete[] auxRod;
    if (rod)
        delete[] rod;
    if (diskSprite)
        delete[] diskSprite;
    while (!solveData.empty())
        solveData.pop();
    focus = 0;
    doLargeValue = false;
    userMoveCount.setString("0");
    state = PRE_GAME;
}
int main()
{
    towerOfHanoi b;
    b.run();
    return EXIT_SUCCESS;
}
