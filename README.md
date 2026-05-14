# 🐍 SNAKE++: 8-bit Edition

A classic Snake game built with **C++** and **Raylib**, featuring clean architecture, efficient data structures, and modern graphics rendering.

---

## 📋 Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Tech Stack](#tech-stack)
- [Project Structure](#project-structure)
- [Data Structures](#data-structures)
- [How to Build & Run](#how-to-build--run)
- [How to Play](#how-to-play)
- [Academic Poster](#academic-poster)

---

## Overview

SNAKE++ is an educational implementation of the classic Snake game that demonstrates:
- Clean separation of concerns (game logic vs. rendering)
- Efficient use of data structures (queues and 2D arrays)
- Real-time game loop and collision detection
- Persistent high score tracking
- Cross-platform graphics rendering with Raylib

---

## ✨ Features

- **Classic Gameplay**: Grow your snake by eating food while avoiding walls and yourself
- **High Score System**: Automatic persistence of the best score with player names
- **Responsive Controls**: Real-time keyboard input handling (Arrow keys or WASD)
- **Smooth Graphics**: 8-bit styled visuals with Raylib rendering
- **Multiple Game States**: Title screen, gameplay, game over, and name entry screens
- **Dynamic Board**: 24x50 grid with configurable game speed
- **Animation Effects**: Visual feedback and smooth animations

---

## 🛠️ Tech Stack

| Component | Technology |
|-----------|-----------|
| **Language** | C++ |
| **Graphics** | Raylib |
| **Build System** | MSVC (cl.exe) / CMake compatible |
| **Platform** | Windows, Linux, macOS |

---

## 📁 Project Structure

```
SNAKE/
├── main.cpp                 # Core game logic and main loop
├── ui.h                     # UI rendering and graphics
├── ui.cpp                   # UI implementation
├── highscore.txt           # High score persistence
├── guide.md                # Presentation guide
├── include/                # Raylib headers
│   ├── raylib.h
│   ├── raymath.h
│   └── rlgl.h
├── lib/                    # Raylib libraries
└── README.md              # This file
```

---

## 🧠 Data Structures

### 1. **2D Array (Game Board)**
- Stores the state of each cell (empty, snake, food)
- Grid dimensions: **24 rows × 50 columns**
- Values: `0` = empty, `1` = snake body, `2` = food
- Enables O(1) collision detection

### 2. **Queue (Snake Body)**
- Stores snake positions as it moves
- **Head**: Back of queue (new segment added here)
- **Tail**: Front of queue (removed when snake moves without eating)
- Efficient growth mechanism when food is consumed
- Prevents immediate self-collision with tail

---

## 🏗️ How to Build & Run

### Prerequisites
- MSVC compiler (Visual Studio)
- Raylib library (included in project)
- C++11 or later

### Build Command
```bash
cl.exe /Zi /EHsc /nologo /Fe:main.exe /I include main.cpp
```

### Run
```bash
./main.exe
```

---

## 🎮 How to Play

| Action | Keys |
|--------|------|
| **Move Up** | `W` or `↑` |
| **Move Down** | `S` or `↓` |
| **Move Left** | `A` or `←` |
| **Move Right** | `D` or `→` |
| **Start Game** | `ENTER` |

### Game Rules
- Eat the white food tiles to grow and increase your score
- Avoid hitting walls or your own body
- Each food eaten = +1 point
- High score is automatically saved
- Game speed is consistent at 220ms per move

---

## 📊 Academic Poster

![Academic Poster](Academic%20Poster.png)

*Detailed project overview and technical breakdown*

---

## 🎯 Algorithm Highlights

### Movement & Collision Detection
```
1. Calculate new head position based on current direction
2. Check wall collision (O(1))
3. Check food collision (O(1) board lookup)
4. Check self-collision (O(1) with movingIntoTail optimization)
5. Update queue and board state
```

### High Score Persistence
```cpp
// Load: Read from highscore.txt on game start
// Save: Write to highscore.txt after game over if new high score
```

---

## 📝 Code Organization

### **main.cpp** - Game Logic
- Board initialization and state management
- Snake movement and collision detection
- Food generation and scoring
- Input handling and direction validation
- High score persistence
- Main game loop with state transitions

### **ui.h/ui.cpp** - Rendering & Visuals
- Board and grid rendering
- Snake and food drawing
- Menu screens (Title, Game Over, Name Entry)
- Score and high score display
- Animation effects and visual feedback

---

## 🚀 Key Features Explained

### Smart Collision Handling
- Allows snake to move into its own tail if tail moves away simultaneously
- Prevents impossible death states
- Validates direction changes (can't reverse into self)

### Frame-Based Game Loop
- 60 FPS rendering
- Configurable game speed (220ms per move)
- Smooth visual animations

### State Management
- Title Screen → Playing → Game Over → Name Entry → Back to Title
- Clean state transitions
- Persistent high score across sessions

---

## 💡 Learning Outcomes

This project demonstrates:
- ✅ Queue data structure implementation and benefits
- ✅ 2D array usage for spatial problems
- ✅ Collision detection algorithms
- ✅ Game loop architecture
- ✅ File I/O for persistence
- ✅ Separation of concerns (logic vs. rendering)
- ✅ Real-time input handling
- ✅ State machine design pattern

---

## 📧 Notes

For a detailed presentation guide, see [guide.md](guide.md).

Enjoy the game! 🎮
