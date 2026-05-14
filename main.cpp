
// Wires game logic to the UI layer.

// Game logic includes
#include <iostream>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>

// Raylib for graphics
// Raylib header path
#include "raylib.h"

// UI header
#include "ui.h"

using namespace std;

// Board size and pacing
const int ROWS = 24;
const int COLS = 50;

// Snake step interval 
const float GAME_SPEED_SEC = 0.220f;

// Game state variables 
// Board cells, Snake queue 
int   board[ROWS][COLS];
queue<Position> snake;
Position food;

// Current movement direction
char  direction  = 'R';
bool  gameOver   = false;
int   score      = 0;
int   highScore  = 0;
string highScoreName = "No player yet";

// Game logic functions
// Load high score from disk if available
void loadHighScore() {
    ifstream file("highscore.txt");
    if (file) {
        file >> highScore;
        file.ignore();
        getline(file, highScoreName);
        if (highScoreName.empty()) highScoreName = "Unknown";
    }
}

// high score to disk
void saveHighScore() {
    ofstream file("highscore.txt");
    file << highScore << "\n" << highScoreName << "\n";
}

// food
void generateFood() {
    int r, c;
    do {
        r = rand() % ROWS;
        c = rand() % COLS;
    } while (board[r][c] != 0);

    food = { r, c };   // UI rendering
    board[r][c] = 2;   
}

// Reset for a new run
void initializeGame() {
    srand((unsigned)time(0));
    loadHighScore();

    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            board[i][j] = 0;

    // Drain any leftover snake positions from a previous game
    while (!snake.empty()) snake.pop();

    direction = 'R';
    gameOver  = false;
    score     = 0;

    // Spawn snake 
    Position start = { ROWS / 2, COLS / 2 };
    snake.push(start);
    board[start.row][start.col] = 1;

    generateFood();
}

// Wall collision 
bool hitWall(Position p) {
    return p.row < 0 || p.row >= ROWS || p.col < 0 || p.col >= COLS;
}

// move snake
void moveSnake() {
    Position head    = snake.back();
    Position newHead = head;

    if      (direction == 'U') newHead.row--;
    else if (direction == 'D') newHead.row++;
    else if (direction == 'L') newHead.col--;
    else if (direction == 'R') newHead.col++;

    // End game 
    if (hitWall(newHead)) { gameOver = true; return; }

    // check food eaten
    bool ateFood = board[newHead.row][newHead.col] == 2;

    // tail movement
    Position tail = snake.front();
    bool movingIntoTail =
        newHead.row == tail.row &&
        newHead.col == tail.col &&
        !ateFood;

    // Self-collision e
    if (board[newHead.row][newHead.col] == 1 && !movingIntoTail) {
        gameOver = true;
        return;
    }

    // if no food is eaten
    if (!ateFood) {
        snake.pop();
        board[tail.row][tail.col] = 0;
    }

    snake.push(newHead);
    board[newHead.row][newHead.col] = 1;

    // Eating food 
    if (ateFood) {
        score++;
        generateFood();
    }
}

// Keyboard input 
void inputDirection() {
    if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    && direction != 'D') direction = 'U';
    if ((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  && direction != 'U') direction = 'D';
    if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  && direction != 'R') direction = 'L';
    if ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && direction != 'L') direction = 'R';
}

// Window dimensions derived from board + UI padding
static int WindowW() { return (COLS + 4) * CELL; }   // board + 2-cell border each side
static int WindowH() { return (ROWS + 6) * CELL; }   // board + top panel + borders

// M
int main() {
  
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WindowW(), WindowH(), "Snake++ : a demonstration of data structures in a game ");
    SetTargetFPS(60);

    // Initialize UI renderer and layout
    UIRenderer ui(ROWS, COLS);
    ui.Init();

    // Set initial screen state
    Screen currentScreen = Screen::TITLE;

    
    loadHighScore();

    // Snake movement timer (frame time)
    float moveTimer = 0.f;

    // High score 
    bool isNewHighScore = false;

    // Main game loop
    while (!WindowShouldClose()) {

        // UI animations and timing
        float dt = GetFrameTime();
        ui.Update();

        // Handle window resizing
        if (IsWindowResized()) ui.Init();

        // Render current screen
        switch (currentScreen) {

        case Screen::TITLE:
            // press enter
            if (IsKeyPressed(KEY_ENTER)) {
                initializeGame();
                moveTimer     = 0.f;
                isNewHighScore = false;
                currentScreen = Screen::PLAYING;
            }
            BeginDrawing();
            ui.DrawTitle(highScore, highScoreName);
            EndDrawing();
            break;

        case Screen::PLAYING:
            inputDirection();

            // Move snake on timer
            moveTimer += dt;
            if (moveTimer >= GAME_SPEED_SEC) {
                moveSnake();
                moveTimer = 0.f;
            }

            // Draw
            BeginDrawing();
            ui.DrawGame(board, snake, score, highScore, highScoreName, direction);
            EndDrawing();

            // Check game over
            if (gameOver) {
                isNewHighScore = score > highScore;
                currentScreen  = Screen::GAME_OVER;
            }
            break;

        case Screen::GAME_OVER:
            BeginDrawing();
            ui.DrawGame(board, snake, score, highScore, highScoreName, direction);
            ui.DrawGameOver(score, highScore, highScoreName, isNewHighScore);
            EndDrawing();

            // Restart and go to name entry 
            if (IsKeyPressed(KEY_ENTER)) {
                if (isNewHighScore) {
                    // name entry
                    ui.ResetName();
                    currentScreen = Screen::NAME_ENTRY;
                } else {
                    // Restart
                    initializeGame();
                    moveTimer     = 0.f;
                    currentScreen = Screen::PLAYING;
                }
            }
            // Esc quits the game 
            if (IsKeyPressed(KEY_ESCAPE)) {
                CloseWindow();
                return 0;
            }
            break;

        case Screen::NAME_ENTRY:
        {
            // Player name 
            BeginDrawing();
            ui.DrawGame(board, snake, score, highScore, highScoreName, direction);
            bool done = ui.DrawNameEntry(score);
            EndDrawing();

            if (done) {
                highScore     = score;
                highScoreName = ui.GetEnteredName();
                saveHighScore();

                // Restart
                initializeGame();
                moveTimer     = 0.f;
                currentScreen = Screen::PLAYING;
            }
            break;
        }
    }
    }

    CloseWindow();
    return 0;
    
}
