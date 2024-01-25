# Arcadia

A mini arcade developed to make and solve 3 popular games

Written in C++ and SFML

[comment]: <> (add the image for arcadia here)

## Table of Contents

1. [Prerequisites](#prerequisites)

2. [Requirements](#requirements)

3. [Main Software](#main-software)

## Prerequisites

- SFML 2.6.1 www.sfml-dev.org

- g++ v12.1.0

- GNU Make 4.4.1

## Requirements

### SFML

After installing SFML: <br>
![SFML](https://github.com/Exteri0/Arcade/blob/main/READMEAssets/SFML%20include%201.png)

1. Add the include folder inside the SFML in the include section in c_cpp_properties.json
   ![SFML](https://github.com/Exteri0/Arcade/blob/main/READMEAssets/SFML%20include%202.png)

2. Make sure the include & lib folder are accessed properly in the Makefile commands
   ![SFML](https://github.com/Exteri0/Arcade/blob/main/READMEAssets/SFML%20include%203.png)

## Main Software

After building the game.exe file, run it and choose your desired game.

Arcadia includes 3 games:

- [Tower of Hanoi](#tower-of-hanoi)
- [Connect 4](#connect-4)
- [Minesweeper](#minesweeper)

### Tower of Hanoi

- [Description](#description)
- [Features](#features)
- [Controls](#controls)
- [Implemented Algorithm](#implemented-algorithm)
- [Time Complexity](#time-complexity)

#### Description:

The Tower of Hanoi is a mathematical and strategic puzzle that challenges players to solve a problem of spatial reasoning. The game consists of three rods (there could be more) and several disks of increasing sizes. The puzzle starts with the disks in ascending order of size on one rod. The objective is to move the entire stack of disks from one rod to another, keeping in mind the following rules:

Only one disk can be moved at a time.
You can only move the upper disk on the rod.
No bigger disk may be placed on top of a smaller disk.

#### Features

Changeable disk number: disks can be any number from 1-8. The limit can be increased to more but will be demanding on the computer.

#### Controls

Right arrow: Increments the number of Disks. <br>
Left arrow: Decrements the number of Disks. <br>
Enter: Starts the game, and restarts if the game ended. <br>
ESC key: Quits the game. <br>

#### Implemented Algorithm

The algorithm implemented works for only tower of hanoi with 3 rods. It's a greedy algorithm that recognizes the pattern of finding the optimal number of moves for solving the game:

1. If the number of disks on the main rod is even -> go right
2. If the number of disks on the main rod is odd -> go left

While making sure that a disk doesn't move 2 times in a row.

#### Time Complexity

Since the algorithm always finds the optimal number of moves, the time complexity is O((2^n) - 1) where N is the number of disks.

### Connect 4

- [Description](#description-1)
- [Features](#features-1)
- [Controls](#controls-1)
- [Implemented Algorithm](#implemented-algorithm-1)
- [Time Complexity](#time-complexity-1)

#### Description

Connect 4 is a two-player connection game in which the players first choose a color and then take turns dropping one colored disc from the top into a vertically suspended grid. The goal is to connect four of one's own discs of the same color in a row, vertically, horizontally, or diagonally.

#### Features

- SFML Graphics: Using SFML library, there is a visual representation of the connect 4 board, pieces and player interactions.
- AI Opponent: This game includes an AI opponent that makes use of the minimax algorithm to determine the best moves. The evaluation technique will be explained in a later section.
- Randomized Starting player: The starting player is randomized either between Human and AI as well between Red and Yellow.
- Restart Option: The game allows the players to restart by clicking a "Restart" button on the interface.

#### Controls

- Player Move: Players can make moves by clicking the column which they want to insert the disc in.
- Restart: Players can restart the game by clicking the "Restart" button.

#### Implemented Algorithm

The algorithm implemented is a recursive algorithm called Minimax of Time Complexity O(b^d) where b is the branching factor and d is the depth. <br>
The algorithm explores all possible future game states, assigns scores to each state based on the evaluation function, and selects the move with the highest score for the AI.<br>
The score is based on which possible combinations could lead to a larger number of consecutive similar colored discs.<br>
The player we want to maximize, in this case is the computer, is given a positive score and the Human player is given a negative score.

#### Time Complexity

As mentioned, the algorithm implemented is a recursive algorithm called Minimax of Time Complexity O(b^d) where b is the branching factor and d is the depth. <br>
The Minimax algorithm implemented is O(7^4) as that each time there is at max 7 options to pick from and the depth used, and can be changed, is 4. <br>

### Minesweeper

- [Description](#description-2)
- [Features](#features-2)
- [Controls](#controls-2)
- [Implemented Algorithm](#implemented-algorithm-2)
- [Time Complexity](#time-complexity-2)

#### Description

Minesweeper is a game where the player knows there a specific number of mines (k) in an NxM board. <br>
The user thus needs to, using basic mathematics and probability, determine what squares have mines and flag them with his (k) flags available.<br>

#### Features

This project contains two Minesweeper modes, normal & A.Isolve.

- Normal: is just the traditional Minesweeper game where the user first reveals a square (always safe) then numbers appear, the number on a square presents the number of mines in the 8 squares around it. The user wins if all squares that aren't mines are covered and loses if he reveals a mine. the normal game can be restarted.
- AI Solve: this mode is accessed by pressing A on the keyboard, it generates a reduced subproblem of Minesweeper, the revealed board. In it, all boards are visible, all of them are numbered (this time from 0 - 9) representing the number of mines in the 9 square grid (including the square itself to avoid triviality), by pressing B, the solution algorithm (explained below) runs and flags mines with white while safe squares are cyan, then the user presses C to end the session. the A.Isolve game isn't restartable.

#### Controls

- Normal mode: <br>
  left click to reveal cell<br>
  right click to flag cell<br>
  Enter to restart the game. <br>

- AI Solve:<br>
  A to start <br>
  B to solve <br>
  C to close session<br>

#### Implemented Algorithm

The implemented algorithm is a backtracking algorithm which runs over all possible board configurations.
we use a visited array to track the current cell.

1st: check if the solution is over (all board numbers are 0) if not, get the first unvisited cell, if all cells are visited --> the solution is wrong --> return false.

2nd: visit the current cell and check if the current cell could have a mine or not (we do this by checking if all the numbers in its 3x3 grid are bigger than 0 (indices of the 3x3 grid outside the board are ignored obviously))

3rd: if the cell can't have a mine, the function returns false to the recursion stack --> cell isn't a safe mine assumption

4th: if the cell can have a mine, we have to situations:

1. cell does have a mine -> we reduce the number of its 3x3 grid cells by 1 and recursively call the funtion on the cell after it
2. cell doesn't have a mine -> we keep the number of its 3x3 grid cells the same and recursively call the function on the cell after it

5th: the cell is unvisited for future recursive calls that may need it in a different state.

#### Time Complexity

Time Complexity: O((2^(n\*m))\*n\*m)
where n and m are the dimensions of the board.
