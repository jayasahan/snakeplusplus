#pragma once  // Prevent multiple inclusions

// Snake UI configuration and rendering

#include "raylib.h"      // Raylib
#include <string>        // std::string for text handling
#include <queue>         // std::queue for snake body representation
#include <cmath>         // std::sinf for animations
#include <cstdio>        // std::snprintf for formatted text  

// Data structures
struct Position { int row; int col; };

// Game screens
enum class Screen { TITLE, PLAYING, GAME_OVER, NAME_ENTRY };

// Layout configuration (cell size and board origin)
static const int CELL   = 28;
static const int GRID_X = 50;
static const int GRID_Y = 90;

// Visual theme colors
static const Color C_BG         = {  15,  20,  30, 255 };
static const Color C_GRID_LINE  = {  30,  40,  55, 255 };
static const Color C_BORDER     = {  80, 220, 120, 200 };
static const Color C_FOOD_OUT   = { 255,  80,  80, 255 };
static const Color C_FOOD_IN    = { 255, 180, 180, 200 };
static const Color C_SCORE_COL  = {  80, 220, 120, 255 };
static const Color C_BEST_COL   = { 200, 200, 100, 255 };
static const Color C_LEVEL_COL  = { 100, 180, 255, 255 };
static const Color C_LEN_COL    = { 200, 140, 255, 255 };
static const Color C_DS_LL      = { 100, 200, 100, 180 };
static const Color C_DS_Q       = { 100, 180, 255, 180 };
static const Color C_DS_STACK   = { 255, 180, 100, 180 };
static const Color C_TITLE_GRN  = {  80, 220, 120, 255 };
static const Color C_GAMEOVER_R = { 255,  80,  80, 255 };
static const Color C_OVERLAY_GO = {   0,   0,   0, 180 };
static const Color C_OVERLAY_PS = {   0,   0,   0, 120 };

// Visual particles for effects
struct UIParticle {
    Vector2 pos, vel;
    float   life, maxLife;
    Color   color;
    bool    active;
};
// for small burst effects
static const int MAX_P = 64;

// UI Renderer 
class UIRenderer {
public:
    int ROWS, COLS;
    int GRID_W, GRID_H;

    UIParticle particles[MAX_P];

    // Buffer for player name entry
    std::string nameBuffer;

    UIRenderer(int rows, int cols)
        : ROWS(rows), COLS(cols),
          GRID_W(cols * CELL), GRID_H(rows * CELL)
    {
        for (auto& p : particles) p.active = false;
    }

    // Initialize layout (recomputed on resize if needed)
    void Init() {}

    // Update animations and particles
    void Update() {
        float dt = GetFrameTime();
        for (auto& p : particles) {
            if (!p.active) continue;
            // Simple physics and lifetime decay
            p.pos.x += p.vel.x * dt;
            p.pos.y += p.vel.y * dt;
            p.vel.y += 120.f * dt;   // gravity
            p.life  -= dt;
            if (p.life <= 0.f) p.active = false;
        }
    }

    // Emit a small burst of particles at a position
    void SpawnParticles(float x, float y) {
        for (int i = 0; i < 8; i++) {
            for (auto& p : particles) {
                if (!p.active) {
                    float angle = (float)(rand() % 360) * DEG2RAD;
                    float spd   = 60.f + rand() % 120;
                    p.pos     = {x, y};
                    p.vel     = {cosf(angle)*spd, sinf(angle)*spd};
                    p.maxLife = p.life = 0.5f + (rand() % 50) / 100.f;
                    p.color   = Color{
                        (unsigned char)(100 + rand() % 155),
                        (unsigned char)(200 + rand() %  55),
                        (unsigned char)(100 + rand() % 100), 255};
                    p.active  = true;
                    break;
                }
            }
        }
    }

    // Trigger an explosion effect at the given grid cell
    void SpawnExplosion(int headRow, int headCol) {
        float x = (float)(GRID_X + headCol * CELL + CELL / 2);
        float y = (float)(GRID_Y + headRow * CELL + CELL / 2);
        for (int i = 0; i < 20; i++) SpawnParticles(x, y);
    }

    // Trigger a smaller burst when food is eaten
    void OnEat(int foodRow, int foodCol) {
        float x = (float)(GRID_X + foodCol * CELL + CELL / 2);
        float y = (float)(GRID_Y + foodRow * CELL + CELL / 2);
        SpawnParticles(x, y);
    }

    // Draw title screen
    void DrawTitle(int hiScore, const std::string& hiName) {
        ClearBackground(C_BG);

        // Centered title layout
        int cx = GetScreenWidth() / 2;

        DrawText("SNAKE",
                 cx - MeasureText("SNAKE", 80) / 2,
                 150, 80, C_TITLE_GRN);

        DrawText("Data Structures Edition",
                 cx - MeasureText("Data Structures Edition", 24) / 2,
                 240, 24, Color{180, 180, 180, 200});

        // Best score on title
        char hbuf[128];
        snprintf(hbuf, sizeof(hbuf), "BEST: %d   by   %s", hiScore, hiName.c_str());
        DrawText(hbuf,
                 cx - MeasureText(hbuf, 20) / 2,
                 290, 20, C_BEST_COL);

        // Controls
        DrawText("WASD / Arrow Keys  -  Move",  cx - 180, 340, 20, LIGHTGRAY);
        DrawText("ESC                -  Exit", cx - 180, 368, 20, LIGHTGRAY);
        DrawText("Eat food to grow the snake",  cx - 180, 396, 20, LIGHTGRAY);

        // DS labels
        DrawText("DS USED:",
                 cx - 180, 444, 18, Color{255, 220, 100, 255});
        DrawText("  2D Array for Game board",
                 cx - 180, 468, 16, C_DS_LL);
        DrawText("  Queue for Snake body segments",
                 cx - 180, 490, 16, C_DS_Q);

        // Blinking start prompt
        float blink = sinf((float)GetTime() * 3.f);
        if (blink > 0)
            DrawText("PRESS ENTER TO START",
                     cx - MeasureText("PRESS ENTER TO START", 26) / 2,
                     558, 26, C_TITLE_GRN);

        _DrawParticles();
    }

    // Draw playing screen
    void DrawGame(
        const int board[][50],
        const std::queue<Position>& snakeQ,
        int score, int hiScore, const std::string& hiName,
        char direction,
        int level = 1,
        int inputQueueSize = 0
    ) {
        ClearBackground(C_BG);
        // Render all playfield layers
        _DrawGrid();
        _DrawFood(board);
        _DrawSnake(board, snakeQ, direction);
        _DrawParticles();
        _DrawHUD(score, hiScore, level, (int)snakeQ.size(), inputQueueSize);
    }

    // Draw pause screen mask
    void DrawPause() {
        int sw = GetScreenWidth(), sh = GetScreenHeight();
        DrawRectangle(0, 0, sw, sh, C_OVERLAY_PS);
        DrawText("PAUSED",
                 sw / 2 - MeasureText("PAUSED", 60) / 2,
                 sh / 2 - 60, 60, C_TITLE_GRN);
        DrawText("Press P or ESC to resume",
                 sw / 2 - MeasureText("Press P or ESC to resume", 22) / 2,
                 sh / 2 + 20, 22, LIGHTGRAY);
    }

    // Draw game over screen
    void DrawGameOver(int score, int hiScore, const std::string& hiName,
                      bool isNewHi,
                      // last 5 score history entries (newest first)
                      const char history[][64] = nullptr, int historyCount = 0)
    {
        int sw = GetScreenWidth(), sh = GetScreenHeight();
        DrawRectangle(0, 0, sw, sh, C_OVERLAY_GO);

        DrawText("GAME OVER",
                 sw / 2 - MeasureText("GAME OVER", 70) / 2,
                 120, 70, C_GAMEOVER_R);

        DrawText(TextFormat("Score: %d", score),
                 sw / 2 - MeasureText(TextFormat("Score: %d", score), 30) / 2,
                 210, 30, WHITE);

        DrawText(TextFormat("Best:  %d", hiScore),
                 sw / 2 - MeasureText(TextFormat("Best:  %d", hiScore), 24) / 2,
                 250, 24, C_BEST_COL);

        if (isNewHi)
            DrawText("NEW HIGH SCORE!",
                     sw / 2 - MeasureText("NEW HIGH SCORE!", 22) / 2,
                     282, 22, C_TITLE_GRN);

        // Score history (stack concept)
        DrawText("--- Score History (Queue/Stack) ---",
                 sw / 2 - MeasureText("--- Score History (Queue/Stack) ---", 20) / 2,
                 318, 20, C_DS_STACK);

        int show = (historyCount < 5) ? historyCount : 5;
        for (int i = 0; i < show; i++) {
            DrawText(TextFormat("#%d  %s", i + 1, history[i]),
                     sw / 2 - 140, 348 + i * 28, 18, LIGHTGRAY);
        }

        // Blinking prompts
        float blink = sinf((float)GetTime() * 3.f);
        if (blink > 0) {
            const char* pr = isNewHi ? "ENTER - Save & Play Again" : "ENTER - Play Again";
            DrawText(pr,
                     sw / 2 - MeasureText(pr, 24) / 2,
                     510, 24, C_TITLE_GRN);
            DrawText("ESC - Quit",
                     sw / 2 - MeasureText("ESC - Quit", 20) / 2,
                     544, 20, LIGHTGRAY);
        }

        _DrawParticles();
    }

    // Draw high score name input component
    bool DrawNameEntry(int score) {
        int sw = GetScreenWidth(), sh = GetScreenHeight();
        DrawRectangle(0, 0, sw, sh, C_OVERLAY_GO);

        DrawText("NEW HIGH SCORE!",
                 sw / 2 - MeasureText("NEW HIGH SCORE!", 50) / 2,
                 160, 50, C_TITLE_GRN);

        DrawText(TextFormat("Score: %d", score),
                 sw / 2 - MeasureText(TextFormat("Score: %d", score), 28) / 2,
                 230, 28, WHITE);

        DrawText("Enter your name:",
                 sw / 2 - MeasureText("Enter your name:", 24) / 2,
                 300, 24, LIGHTGRAY);

        // Input box
        DrawRectangle(sw / 2 - 180, 338, 360, 40,
                      Color{20, 30, 45, 255});
        DrawRectangleLines(sw / 2 - 180, 338, 360, 40, C_BORDER);

        std::string disp = nameBuffer;
        float blink = sinf((float)GetTime() * 4.f);
        if (blink > 0) disp += "|";

        DrawText(disp.c_str(),
                 sw / 2 - MeasureText(disp.c_str(), 22) / 2,
                 348, 22, C_TITLE_GRN);

        DrawText("ENTER to confirm",
                 sw / 2 - MeasureText("ENTER to confirm", 18) / 2,
                 400, 18, Color{150, 150, 150, 200});

        // Keyboard input (printable ASCII only)
        int key = GetCharPressed();
        while (key > 0) {
            if (key >= 32 && key <= 125 && (int)nameBuffer.size() < 18)
                nameBuffer += (char)key;
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !nameBuffer.empty())
            nameBuffer.pop_back();

        return IsKeyPressed(KEY_ENTER);
    }

    // Retrieve entered name (fallback if empty)
    std::string GetEnteredName() const {
        return nameBuffer.empty() ? "Player" : nameBuffer;
    }
    // Clear name input buffer
    void ResetName() { nameBuffer.clear(); }

private:

    // Render grid background
    void _DrawGrid() {
        DrawRectangle(GRID_X, GRID_Y, GRID_W, GRID_H, C_BG);

        for (int c = 0; c <= COLS; c++)
            DrawLine(GRID_X + c * CELL, GRID_Y,
                     GRID_X + c * CELL, GRID_Y + GRID_H,
                     C_GRID_LINE);
        for (int r = 0; r <= ROWS; r++)
            DrawLine(GRID_X,           GRID_Y + r * CELL,
                     GRID_X + GRID_W,  GRID_Y + r * CELL,
                     C_GRID_LINE);

        DrawRectangleLinesEx(
            {(float)GRID_X - 2, (float)GRID_Y - 2,
             (float)GRID_W + 4, (float)GRID_H + 4},
            2, C_BORDER);
    }

    // Render food element (pulsing effect)
    void _DrawFood(const int board[][50]) {
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                if (board[r][c] != 2) continue;
                int   px = GRID_X + c * CELL + CELL / 2;
                int   py = GRID_Y + r * CELL + CELL / 2;
                float pulse = 1.f + 0.15f * sinf((float)GetTime() * 5.f);
                int   rad   = (int)(CELL / 2 * 0.75f * pulse);
                DrawCircle(px, py, (float)rad,          C_FOOD_OUT);
                DrawCircle(px, py, (float)rad * 0.55f,  C_FOOD_IN);
            }
        }
    }

    // Render snake segments with a head-to-tail gradient
    void _DrawSnake(const int board[][50],
                    const std::queue<Position>& snakeQ,
                    char dir) {
        if (snakeQ.empty()) return;

        Position headPos = snakeQ.back();
        int      total   = (int)snakeQ.size();
        int      idx     = total - 1;   // head = back, so draw back→front visually

        // Iterate a copy of the queue tail→head to assign gradient index
        std::queue<Position> tmp = snakeQ;
        // We need head-first order for gradient t=0 at head.
        // Build a simple array from the queue (front=tail → back=head).
        // Max snake size is ROWS*COLS which is at most 24*50=1200.
        // Use dynamic approach: front of queue is the tail (idx=total-1 in gradient),
        // back of queue is the head (idx=0 in gradient).

        std::queue<Position> copy = snakeQ;
        int count = (int)copy.size();

        // Draw each segment: assign gradient t by position in queue
        // front = tail (t close to 1), back = head (t=0)
        for (int i = 0; i < count; i++) {
            Position seg = copy.front(); copy.pop();

            // i=0 is tail end, i=count-1 is head
            float t = (count > 1)
                      ? (float)(count - 1 - i) / (float)(count - 1)
                      : 0.f;

            unsigned char gr = (unsigned char)(40  + t * 10);
            unsigned char gg = (unsigned char)(220 - t * 80);
            unsigned char gb = (unsigned char)(80  + t * 20);

            int px = GRID_X + seg.col * CELL;
            int py = GRID_Y + seg.row * CELL;

            DrawRectangle(px + 2, py + 2, CELL - 4, CELL - 4,
                          Color{gr, gg, gb, 255});

            // Head decoration (i = count-1 is the head / back of queue)
            if (i == count - 1) {
                DrawRectangleLinesEx(
                    {(float)px + 1, (float)py + 1,
                     (float)CELL - 2, (float)CELL - 2},
                    2, Color{180, 255, 180, 200});

                // Eyes
                int ex = 6, ey = 6;
                if (dir == 'R') { ex = CELL - 9; ey = 6; }
                if (dir == 'L') { ex = 3;         ey = 6; }
                if (dir == 'U') { ex = 6;         ey = 3; }
                if (dir == 'D') { ex = 6;         ey = CELL - 9; }

                DrawCircle(px + ex,        py + ey, 3, WHITE);
                DrawCircle(px + CELL - ex, py + ey, 3, WHITE);
                DrawCircle(px + ex,        py + ey, 1, BLACK);
                DrawCircle(px + CELL - ex, py + ey, 1, BLACK);
            }
        }
    }

    // Particle update routine
    void _DrawParticles() {
        for (const auto& p : particles) {
            if (!p.active) continue;
            float alpha = p.life / p.maxLife;
            Color c = p.color;
            c.a = (unsigned char)(alpha * 255);
            DrawCircleV(p.pos, 4.f * alpha, c);
        }
    }

    // Draw heads-up display information
    void _DrawHUD(int score, int hiScore, int level, int snakeLen,
                  int queuedInputs) {
        DrawText(TextFormat("SCORE: %d", score),
                 GRID_X,           20, 28, C_SCORE_COL);
        DrawText(TextFormat("BEST:  %d", hiScore),
                 GRID_X + 200,     20, 22, C_BEST_COL);
        DrawText(TextFormat("LEVEL: %d", level),
                 GRID_X + 420,     20, 22, C_LEVEL_COL);
        DrawText(TextFormat("LEN:   %d", snakeLen),
                 GRID_X + 600,     20, 22, C_LEN_COL);

        // DS labels (bottom)
        int by = GRID_Y + GRID_H + 8;
        DrawText("2D ARRAY: Game Board",   GRID_X,       by, 14, C_DS_LL);
        DrawText("QUEUE: Snake Body",       GRID_X + 260, by, 14, C_DS_Q);
        DrawText(TextFormat("[%d queued]", queuedInputs),
                 GRID_X + 420,             by, 13,
                 Color{150, 200, 255, 160});
    }
};
