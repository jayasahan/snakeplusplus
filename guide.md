# Snake++ Presentation Guide

## How to Present This Project
1. **Introduction**: Start by introducing the game as a classic version of Snake with upgraded graphics using the Raylib library.
2. **Gameplay Demo**: Play the game for a few minutes. Show how the snake grows, the score increases, and what happens when the game ends.
3. **Data Structures Used**: Highlight the core data structures that power the game:
   - **2D Array**: Used for the game board to track the snake, food, and empty spaces.
   - **Queue**: Used for the snake's body. As the snake moves, the new head is pushed to the queue, and the tail is popped off.
4. **Code Structure**: Explaining the separation of game logic (`main.cpp`) and graphics/UI (`ui.h`) is a good way to show good software design.

## Codebase Overview
- **`main.cpp`**: Contains the core game logic (movement, collision, score tracking) and the main game loop. It also handles input, manages game state transitions, and stores the high score.
- **`ui.h`**: Handles all visual elements. It draws the board, the snake, particles, menus, overlays, and HUD text using Raylib. It also includes simple animation effects.
- **`include/` & `lib/`**: Contain the Raylib headers and libraries required for graphics, input, and window management.

## Full Codebase Description (Simple)
- **Core loop**: The program starts a Raylib window and runs a loop that updates input, game state, and rendering every frame.
- **Board model**: A 2D array stores the board cells. Values indicate empty space, snake body, or food.
- **Snake model**: A queue stores the snake body. The head is added each move, and the tail is removed unless food is eaten.
- **Game states**: The game switches between Title, Playing, Game Over, and Name Entry screens.
- **Persistence**: High scores are saved to and loaded from a text file.
- **UI rendering**: The UI class draws the grid, snake, food, effects, and menus with consistent styling.

## Presentation Script (2–4 minutes)
Hello everyone. Today I am presenting my Snake game built in C++ with the Raylib library.

This is a classic Snake game with upgraded visuals. The goal is simple: eat food, grow longer, and avoid hitting walls or yourself.

The game uses two main data structures. First, a 2D array represents the board and stores what is in each cell. Second, a queue stores the snake body. Each move, the new head is pushed to the queue, and the tail is popped unless the snake eats food.

The code is split into two main parts. The logic is in the main file, and all rendering is in the UI header. This separation makes the project easier to manage and explains clearly how the game works.

Let me show the gameplay. You will see the score increase, the snake grow, and the game end when a collision happens. When the game ends, the high score is saved and can be displayed on the title screen.

In summary, this project shows how basic data structures can be used to build a real-time game with a clean structure and simple logic. Thank you.