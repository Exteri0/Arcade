#ifndef TOWEROFHANOI_H
#define TOWEROFHANOI_H

#include <bits/stdc++.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;
enum gameState { PRE_GAME, IN_GAME, POST_GAME };

class towerOfHanoi
{
public:
    towerOfHanoi();
    ~towerOfHanoi();
    void run();

private:
    // Data structure to represent the towers
    stack<int>* rod;
    void createRods();

    // Puzzle settings
    int numDisks, numRods, startRod, endRod, * auxRod;

    // Solve the puzzle
    void solve();
    void startRodToEndRod();
    bool isInvalidRod(int rodNum, int prevDisk);
    bool isAuxiliaryRod(int rodNum);
    void placeDisk(int beginRod, int endRod, bool solving = true);

    // Solve instructions
    queue<pair<int, int>> solveData;
    pair<int, int> instruction;

    // Game state and events
    gameState state;
    typedef void (towerOfHanoi::* runStatePtr)();
    runStatePtr runState[3];
    void preGame();
    void inGame();
    void postGame();
    typedef void (towerOfHanoi::* eventPtr)(Event event);
    eventPtr processEvent[3];
    void preGameEvent(Event event);
    void inGameEvent(Event event);
    void postGameEvent(Event event);

    // Graphics
    RenderWindow window;

    // Disk
    Texture diskTexture[4];
    Sprite* diskSprite;
    void loadDiskSprites();

    // Background
    Texture backgroundTexture;
    Sprite backgroundSprite;

    // Foreground
    Texture foregroundTexture;
    Sprite foregroundSprite;

  


    // Text/Menu
    Font font ;
    Text title, askEnter, askInput[2], inputText[2], userMoveCount, endMsg[3], askConfirm;
    String inputStr[2];

    int focus;                          // 0 = num disks, 1 = num rods, 2 = confirmation
    bool needConfirmation, doLargeValue;

    RectangleShape focusBox;
    Texture arrowTexture;
    Sprite arrowSprite[2];

    void incrementInputStr(int focus);
    void decrementInputStr(int focus);
    inline void incrementMoveCount()
    {
        userMoveCount.setString(to_string(stoul(userMoveCount.getString().toAnsiString()) + 1));
    }
    void setEndMessage();

 

    // Disk animation
    bool moveInProgress;
    Clock moveClock;
    Time moveTime;
    void moveDisk();
    void getNextInstruction();
    int posX(int rodNum);
    int posY(int rodNum);

    // Etc
    void clearSettings();
};

#endif // TOWEROFHANOI_H