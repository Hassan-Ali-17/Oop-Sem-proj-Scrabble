```
 ██████╗  ██████╗ ██████╗       ███████╗███████╗███╗   ███╗
██╔═══██╗██╔═══██╗██╔══██╗      ██╔════╝██╔════╝████╗ ████║
██║   ██║██║   ██║██████╔╝█████╗███████╗█████╗  ██╔████╔██║
██║   ██║██║   ██║██╔═══╝ ╚════╝╚════██║██╔══╝  ██║╚██╔╝██║
╚██████╔╝╚██████╔╝██║           ███████║███████╗██║ ╚═╝ ██║
 ╚═════╝  ╚═════╝ ╚═╝           ╚══════╝╚══════╝╚═╝     ╚═╝

██████╗ ██████╗  ██████╗      ██╗
██╔══██╗██╔══██╗██╔═══██╗     ██║
██████╔╝██████╔╝██║   ██║     ██║
██╔═══╝ ██╔══██╗██║   ██║██   ██║
██║     ██║  ██║╚██████╔╝╚█████╔╝
╚═╝     ╚═╝  ╚═╝ ╚═════╝  ╚════╝

███████╗ ██████╗██████╗  █████╗ ██████╗ ██████╗ ██╗     ███████╗
██╔════╝██╔════╝██╔══██╗██╔══██╗██╔══██╗██╔══██╗██║     ██╔════╝
███████╗██║     ██████╔╝███████║██████╔╝██████╔╝██║     █████╗
╚════██║██║     ██╔══██╗██╔══██║██╔══██╗██╔══██╗██║     ██╔══╝
███████║╚██████╗██║  ██║██║  ██║██████╔╝██████╔╝███████╗███████╗
╚══════╝ ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝ ╚═════╝ ╚══════╝╚══════╝
```

> **A fully-featured digital Scrabble game built with C++ & SFML — showcasing core OOP principles through clean, modular design.**

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [OOP Design Principles](#-oop-design-principles)
- [Architecture](#-architecture)
- [Tech Stack](#-tech-stack)
- [Getting Started](#-getting-started)
- [Gameplay](#-gameplay)
- [Project Structure](#-project-structure)
- [Team](#-team)

---

## 🎯 Overview

This project is a **semester OOP project** that brings the classic board game **Scrabble** to life as a fully digital experience. Players form words on a 15×15 grid to score points, with automated rule enforcement, real-time word validation against a dictionary, and a clean SFML-powered graphical interface.

The codebase is architected around solid OOP principles — serving as both a playable game and a demonstration of how **inheritance**, **polymorphism**, **composition**, and **aggregation** can produce maintainable, extensible software.

---

## ✨ Features

| Feature | Description |
|---|---|
| 🎲 **15×15 Game Board** | Full-size Scrabble board with all standard bonus tiles |
| 🔠 **Player Racks** | Each player manages a rack of up to 7 letter tiles |
| 🎒 **Tile Bag** | Randomized draw system with correct letter distribution |
| 📖 **Dictionary Validation** | Real-time word checking against a built-in dictionary |
| 🏆 **Automated Scoring** | Handles DL, TL, DW, TW bonus squares automatically |
| ↩️ **Move Cancellation** | Undo tile placements before confirming a move |
| 📚 **Dictionary Updates** | Add new words to the dictionary at runtime |
| 🖥️ **SFML GUI** | Smooth, interactive graphical interface |

---

## 🧠 OOP Design Principles

This project demonstrates four core OOP pillars in action:

### 🔷 Inheritance
```
GameEntity  ←──────────────────┐
    └── position management    │
                               │
Drawable    ←──────────────────┤
    └── virtual draw()         │
                               │
          Tile ───────────────►┘
      (inherits both)
```
The `Tile` class inherits from both `GameEntity` (position tracking) and `Drawable` (rendering), enabling code reuse and a clean class hierarchy.

---

### 🔶 Polymorphism
```cpp
// Drawable's virtual method enables uniform rendering
virtual void draw(sf::RenderWindow& window) = 0;

// All components (Tile, Board, UI elements) rendered the same way
for (auto& drawable : gameComponents) {
    drawable->draw(window);  // polymorphic dispatch
}
```
The `Drawable` interface's virtual `draw()` method allows all renderable components to be handled uniformly, regardless of their concrete type.

---

### 🔹 Composition
```
Board
 └── Tile[15][15]       ← owns, manages lifecycle

Bag
 └── Piece[]            ← owns all undrawn pieces

YesNoDialog
 └── Button[]           ← owns UI buttons
```
`Board`, `Bag`, and `YesNoDialog` fully own their child objects — when a `Board` is destroyed, its tiles go with it.

---

### 🔸 Aggregation
```
Rack ──────► Piece[]    ← loosely holds, does NOT own
               │
               └──► can be reassigned back to Bag
```
`Rack` loosely references `Piece` objects. When a tile is played or swapped, the piece moves back to the `Bag` — demonstrating a "has-a" relationship without ownership.

---

## 🏗️ Architecture

```
┌─────────────────────────────────────────────────────────┐
│                        Game Loop                        │
│                  (Event → Update → Draw)                │
└────────────────────────┬────────────────────────────────┘
                         │
        ┌────────────────┼────────────────┐
        ▼                ▼                ▼
   ┌─────────┐     ┌──────────┐    ┌──────────────┐
   │  Board  │     │  Player  │    │  Dictionary  │
   │ 15 × 15 │     │  + Rack  │    │  Validator   │
   └────┬────┘     └────┬─────┘    └──────────────┘
        │               │
        ▼               ▼
   ┌─────────┐     ┌──────────┐
   │  Tile   │     │   Bag    │
   │(Drawable│     │ (Pieces) │
   │  + Pos) │     └──────────┘
   └─────────┘
```

---

## 🛠️ Tech Stack

```
Language  ──►  C++17
Graphics  ──►  SFML 2.5+
Build     ──►  g++ / Makefile
OS        ──►  Windows / Linux / macOS
```

---

## 🚀 Getting Started

### Prerequisites

- C++17 compatible compiler (`g++` or `clang++`)
- [SFML 2.5+](https://www.sfml-dev.org/download.php) installed

### Build & Run

```bash
# Clone the repository
git clone https://github.com/your-username/Oop-Sem-proj-Scrabble.git
cd Oop-Sem-proj-Scrabble

# Compile
g++ -std=c++17 src/*.cpp -o scrabble -lsfml-graphics -lsfml-window -lsfml-system

# Run
./scrabble
```

### Windows (with MinGW)
```bash
g++ -std=c++17 src/*.cpp -o scrabble.exe ^
  -lsfml-graphics -lsfml-window -lsfml-system ^
  -DSFML_STATIC
```

---

## 🎮 Gameplay

```
┌──────────────────────────────────────────────┐
│  HOW TO PLAY                                 │
├──────────────────────────────────────────────┤
│  1. Draw 7 tiles from the Bag at game start  │
│  2. Place tiles on the board to form words   │
│  3. Words must connect to existing tiles     │
│  4. All words are validated against dict     │
│  5. Score = letter values + bonus squares    │
│  6. Draw new tiles to refill your rack       │
│  7. Game ends when Bag is empty              │
└──────────────────────────────────────────────┘

  Bonus Squares:
  [DL] Double Letter    [TL] Triple Letter
  [DW] Double Word      [TW] Triple Word  ← corners!
```

---

## 📁 Project Structure

```
Oop-Sem-proj-Scrabble/
│
├── src/
│   ├── main.cpp            # Entry point & game loop
│   ├── Board.cpp/h         # 15x15 grid logic
│   ├── Tile.cpp/h          # Tile (inherits GameEntity + Drawable)
│   ├── Rack.cpp/h          # Player tile rack (aggregation)
│   ├── Bag.cpp/h           # Tile bag (composition)
│   ├── Player.cpp/h        # Player state & scoring
│   ├── Dictionary.cpp/h    # Word validation
│   ├── GameEntity.h        # Base class: position
│   ├── Drawable.h          # Interface: virtual draw()
│   └── YesNoDialog.cpp/h   # UI dialog (composes Buttons)
│
├── assets/
│   ├── fonts/
│   └── textures/
│
├── dictionary.txt          # Word list for validation
├── Makefile
└── README.md
```

---

## 👥 Team

> OOP Semester Project — built with ☕ and way too many late nights.

---

*Built with C++ · Powered by SFML · Driven by OOP*
