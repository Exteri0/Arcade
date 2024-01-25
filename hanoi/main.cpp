#include "towerofhanoi.h"

towerOfHanoi::towerOfHanoi()
{
    numDisks = 0;
    numRods = 3;
    startRod = 0;
    endRod = 0;
    rod = nullptr; // stack of rods
    auxRod = nullptr;
    state = PRE_GAME;
    focus = 0;
    moveInProgress = false;
    moveTime = milliseconds(100);

    // Creating Window
    window.create(VideoMode::getDesktopMode(), "Towers of Hanoi");
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(true);

    // Pointers to game state run
    runState[PRE_GAME] = &towerOfHanoi::preGame;
    runState[IN_GAME] = &towerOfHanoi::inGame;
    runState[POST_GAME] = &towerOfHanoi::postGame;

    // Pointers to game state events
    processEvent[PRE_GAME] = &towerOfHanoi::preGameEvent;
    processEvent[IN_GAME] = &towerOfHanoi::inGameEvent;
    processEvent[POST_GAME] = &towerOfHanoi::postGameEvent;

    // loading disks pictures
    diskSprite = nullptr;
    for (int i = 0; i < 4; i++)
        diskTexture[i].loadFromFile("disk" + to_string(i + 1) + ".png");
    // Loading background picture
    backgroundTexture.loadFromFile("background.png");
    backgroundSprite.setTexture(backgroundTexture);

    // loading foreground pictures
    foregroundTexture.loadFromFile("foreground.png");
    foregroundSprite.setTexture(foregroundTexture);
    foregroundSprite.setPosition(0, window.getSize().y - 350);

    // creatnig the title text using font (hanoi.ttf)
    font.loadFromFile("hanoi.ttf");
    title.setFont(font);
    title.setCharacterSize(190);
    title.setString("Tower of Hanoi");
    title.setFillColor(Color::White);
    title.setPosition(window.getSize().x / 2 - title.getGlobalBounds().width / 2, window.getSize().y / 10);

    // Text prompt for "enter"
    askEnter.setFont(font);
    askEnter.setCharacterSize(40);
    askEnter.setString("Press ENTER to start");
    askEnter.setFillColor(Color::White);
    askEnter.setPosition(window.getSize().x / 2 - askEnter.getGlobalBounds().width / 2, window.getSize().y / 1.2);

    // Prompt for input text

    askInput[0].setFont(font);
    askInput[0].setCharacterSize(50);
    askInput[0].setFillColor(Color::White);
    askInput[0].setPosition(window.getSize().x / 2 - title.getGlobalBounds().width / 4.9, window.getSize().y / 2.5);
    // text to ask for number of disks
    askInput[0].setString("Number of disks:");

    // Prompt for confirmation
    askConfirm.setFont(font);
    askConfirm.setCharacterSize(70);
    askConfirm.setFillColor(Color::White);
    askConfirm.setPosition(window.getSize().x / 2 - askConfirm.getGlobalBounds().width / 2, window.getSize().y / 1.25);

    // Default number of disks
    inputStr[0] = "3";

    // Input text (number of disks)

    inputText[0].setFont(font);
    inputText[0].setCharacterSize(50);
    inputText[0].setString(inputStr[0]);
    inputText[0].setFillColor(Color::White);
    inputText[0].setPosition(window.getSize().x / 2 + 200, window.getSize().y / 2.5);

    // counter for Moves
    userMoveCount.setFont(font);
    userMoveCount.setCharacterSize(100);
    userMoveCount.setString("0");
    userMoveCount.setFillColor(Color::White);
    userMoveCount.setPosition(30, 1);

    // Ending message
    // endmsg[0] is congrats , endmsg[1] is your moves, endsmg[2] is enter to resart
    for (int i = 0; i < 3; i++)
    {
        endMsg[i].setFont(font);
        endMsg[i].setCharacterSize(70);
        endMsg[i].setFillColor(Color::White);
    }
    endMsg[0].setString("Congratulations!");

    // Focusbox is where the focus of change is
    focusBox.setFillColor(Color(255, 255, 255, 50));

    // focus box arrows pictures
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
// destructor for the class
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
    moveInProgress = NULL;
}

void towerOfHanoi::run()
{
    while (window.isOpen())
    {
        // Handle keyboard events
        Event event;
        while (window.pollEvent(event))
            (this->*processEvent[state])(event);

        // Window rendering
        window.clear();
        (this->*runState[state])();
        window.display();
    }
}

void towerOfHanoi::createRods()
{
    // initialize rods and fill with disks
    rod = new stack<int>[numRods];
    for (int i = 0; i < numDisks; i++)
    {
        rod[startRod].push(i + 1);
    }
}

void towerOfHanoi::solve()
{
    // initial loop
    startRodToEndRod();
    // reset the rods for next user input
    while (!rod[endRod].empty())
    {
        rod[endRod].pop();
    }
    for (int i = 0; i < numDisks; i++)
    {
        rod[startRod].push(i + 1);
    }
}

void towerOfHanoi::startRodToEndRod()
{
    // initial variables
    int prevDisk{}, nextRod, originalStartSize = rod[startRod].size();
    // loop from left to rigt
    for (int currRod = 0; rod[endRod].size() != originalStartSize; currRod == numRods - 1 ? currRod = 0 : currRod++)
    {
        // if it is the remaining rod or invalid then skip it
        if (isInvalidRod(currRod, prevDisk) || isAuxiliaryRod(currRod))
            continue;
        // if number of disks is even then go left
        if ((rod[currRod].top() % 2))
        {
            nextRod = !currRod ? numRods - 1 : currRod - 1;
            while (isAuxiliaryRod(nextRod))
                nextRod--;
        }
        // else go right
        else
        {
            nextRod = (currRod + 1) % numRods;
            while (isAuxiliaryRod(nextRod))
                nextRod++;
        }
        // if next rod is empty or disk to move is greater then put the disk as the prev, and move it.
        if (rod[nextRod].empty() || rod[currRod].size() && rod[currRod].top() > rod[nextRod].top())
        {
            prevDisk = rod[currRod].top();
            placeDisk(currRod, nextRod);
        }
    }
}
bool towerOfHanoi::isInvalidRod(int rodNum, int prevDisk)
{
    // current rod is empty or on the previous disk moved
    if (rod[rodNum].empty() || (!rod[rodNum].empty() && rod[rodNum].top() == prevDisk))
        return true;
    return false;
}

bool towerOfHanoi::isAuxiliaryRod(int rodNum)
{ // check if rod is auxiliary or not
    for (int i = 0; (i < numRods - 3) && auxRod; i++)
        if (rodNum == auxRod[i])
            return true;
    return false;
}

void towerOfHanoi::placeDisk(int beginRod, int endRod, bool solving)
{
    // place disk from top of beginning rod to end rod
    int disk = rod[beginRod].top();
    rod[beginRod].pop();
    rod[endRod].push(disk);
    // if solving then push the steps in the solveData
    if (solving)
        solveData.push(pair<int, int>(disk, endRod));
}

void towerOfHanoi::preGame()
{
    // Draw the background and foreground
    window.draw(backgroundSprite);
    window.draw(foregroundSprite);

    // Set the position of the focus box relative to the focused input text
    focusBox.setPosition(
        inputText[0].getPosition().x / 1.597 + askInput[0].getGlobalBounds().width / 0.84,
        askInput[0].getGlobalBounds().top - askInput[0].getGlobalBounds().height / 2.5 + (focus * 100));

    // Set the size of the focus box relative to the width of the focused input text
    focusBox.setSize(Vector2f(47 + inputText[focus].getGlobalBounds().width, 70));

    // Set the position of arrows relative to the focused input text
    arrowSprite[0].setPosition(arrowSprite[0].getPosition().x,
                               inputText[focus].getPosition().y + 1);

    arrowSprite[1].setPosition(
        inputText[0].getPosition().x + inputText[focus].getGlobalBounds().width + 55.8,
        inputText[focus].getPosition().y + 1);

    // Draw focus box and arrows
    window.draw(focusBox);
    for (int i = 0; i < 2 && focus < 2; i++)
        window.draw(arrowSprite[i]);

    // Draw title
    window.draw(title);

    // Draw input and prompt
    window.draw(askInput[0]);
    window.draw(inputText[0]);
    window.draw(askEnter);
}

void towerOfHanoi::inGame()
{
    // Check win condition
    if (rod[endRod].size() == numDisks && !moveInProgress)
    {
        // Set end message and enter post-game state
        setEndMessage();
        state = POST_GAME;
    }

    // Handle disk animation if a move is in progress
    if (moveInProgress)
        moveDisk();

    // Draw background
    window.draw(backgroundSprite);

    // draw disks
    for (int i = 0; i < numDisks; i++)
        window.draw(diskSprite[i]);

    // draw foreground
    window.draw(foregroundSprite);

    // draw move counter
    window.draw(userMoveCount);
}

void towerOfHanoi::postGame()
{
    // draw background
    window.draw(backgroundSprite);

    // draw disks
    for (int i = 0; i < numDisks; i++)
        window.draw(diskSprite[i]);

    // draw foreground
    window.draw(foregroundSprite);

    // draw end messages
    for (int i = 0; i < 2; i++)
        window.draw(endMsg[i]);

    // Prompt for enter to restart
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

            // Decrement focused value (number of disks)
        case Keyboard::Left:
            decrementInput(focus);
            break;

            // Increment focused value (number of disks)
        case Keyboard::Right:
            incrementInput(focus);
            break;

            // Set the board and proceed to ingame
        case Keyboard::Return:
            if (focus == 2)
            {
                focus = 0;
                return;
            }

            // Set the board based off of input
            numDisks = stoi(inputStr[0].toAnsiString());
            endRod = numRods - 1;

            // if no errors -> proceed
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
{ // Handle keyboard events
    if (event.type == Event::KeyPressed)
    { // Check the specific key that was pressed
        switch (event.key.code)
        {
        // Quit
        case Keyboard::Escape:
            window.close(); // Close the game window
            cout.flush();   // clear any pending output
            break;

        // Trigger a move if the Space key is pressed
        case Keyboard::Space:
            // Check if a move is not already in progress and if there are still disks to move
            if (!moveInProgress && rod[endRod].size() != numDisks)
            {
                getNextInstruction();  // get next move instruction
                moveInProgress = true; // Set the flag indicating a move is in progress
                moveClock.restart();   // Restart clock to measure elapsed time for animation
            }
            break;
        }
    }
}

void towerOfHanoi::postGameEvent(Event event)
{ // Handle keyboard events
    if (event.type == Event::KeyPressed)
    {
        switch (event.key.code)
        {
        // Quit
        case Keyboard::Escape:
            window.close();
            cout.flush();
            break;

        // Return to the title screen
        case Keyboard::Return:
            clearSettings(); // Clear game settings
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
        // Set the texture, iterating from textures 1-4 repeatedly
        diskSprite[i].setTexture(diskTexture[i % 4]);

        // Set the origin as the middle of the first disk's dimensions
        diskSprite[i].setOrigin(300, 88);

        // make disks smaller in size for each disk made to stack them
        diskSprite[i].setScale(1 - (i * multiplier), 1 - (i * multiplier));
        diskSprite[i].setScale(diskSprite[i].getScale().x * (3 / ((float)numRods)), diskSprite[i].getScale().y);

        // position disks onto the start rod
        diskSprite[i].setPosition(posX(startRod), posY(startRod));
    }
}

void towerOfHanoi::incrementInput(int focus)
{ // this handles what happens when you press the right arrow which increase the number of disks here
    if (inputStr[focus].getSize())
    {
        int num = stoi(inputStr[0].toAnsiString());
        if (num < 8) // max num allowed of disks
        {
            num++;
            inputStr[focus] = to_string(num);
            inputText[focus].setString(inputStr[focus]);
            arrowSprite[1].setColor(Color(255, 255, 255, 200));
        }
    }
}

void towerOfHanoi::decrementInput(int focus)
{ // this handles what happens when you press the left arrow which decreases the number of disks here
    if (inputStr[focus].getSize())
    {
        int num = stoi(inputStr[0].toAnsiString());
        if (num > 1) // minimum allowed of disks
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
{ // animation for moving a disk
    // Pick up
    if (moveClock.getElapsedTime() < milliseconds(30))
    {
        // Move the disk upwards
        diskSprite[instruction.first - 1].move(0, -5);
    }

    // Done moving
    else if (moveClock.getElapsedTime() >= moveTime)
    {
        // Set the disk to end position
        diskSprite[instruction.first - 1].setPosition(posX(instruction.second), posY(instruction.second));

        // Move is finished
        moveInProgress = false;

        // Increment the user move counter
        incrementMoveCount();
        return;
    }

    // Put down
    else if (moveClock.getElapsedTime() > milliseconds(30))
    {
        // Move the disk downwards
        diskSprite[instruction.first - 1].move(0, 5);
    }
}

void towerOfHanoi::getNextInstruction()
{ // check if there are more instructions and no move is already in progress
    if (!solveData.empty() && !moveInProgress)
    {
        // get next instruction from the solveData queue
        instruction = solveData.front();
        solveData.pop();

        // Apply instruction to the game state
        for (int beginRod = 0; beginRod < numRods; beginRod++)
        { // Check if the top of the current rod matches the disk
            if (rod[beginRod].size() && rod[beginRod].top() == instruction.first)
                placeDisk(beginRod, instruction.second); // Move the disk to the desired rod
        }
    }
}

int towerOfHanoi::posX(int rodNum)
{ // Calculate the X position of a given rod
    float xPos = diskSprite[0].getGlobalBounds().width / 2. + rodNum * (diskSprite[0].getGlobalBounds().width + (3 / (numRods * 0.05)) - pow(0.5, numRods));

    // Calculate the farthest X position to determine the total width of all rods
    float xPosFarthest = ((numRods - 1) * (diskSprite[0].getGlobalBounds().width + (3 / (numRods * 0.05)) - pow(0.5, numRods)));
    xPosFarthest += diskSprite[0].getGlobalBounds().width;

    float xDif = (window.getSize().x - xPosFarthest) / 2.; // Calculate the difference between the window width and the total width of all rods

    xPos += xDif; // Adjust the X position by adding the calculated difference

    return static_cast<int>(xPos); // Convert and return as an integer
}

int towerOfHanoi::posY(int rodNum)
{
    // Default Y position (when there are no disks on the rod)
    int yPosition = 950;

    // Find the heights of all disks in the rod and subtract from the default Y position
    stack<int> temp = rod[rodNum];
    for (int i = 0; i < rod[rodNum].size(); i++)
    {
        int disk = temp.top();
        temp.pop();
        yPosition -= static_cast<int>(diskSprite[disk - 1].getGlobalBounds().height / 2.25);
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
    userMoveCount.setString("0");
    state = PRE_GAME;
}
int main()
{
    towerOfHanoi b;
    b.run();
    return EXIT_SUCCESS;
}
