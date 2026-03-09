# Scopae Minarum

**Alpha Version** | A Minesweeper-inspired Roguelike Mining Adventure

---

## About

**Scopae Minarum** is a unique twist on the classic Minesweeper formula, combining strategic mine-sweeping gameplay with roguelike progression, score-based challenges, and a multi-round campaign system.

Navigate through increasingly difficult mining operations, beat target scores, manage your lives, and push your luck to reach **Calicata Level 10** and face the final challenge!

---

## Features

### Core Gameplay
- **Classic Minesweeper Mechanics** – Reveal tiles, flag mines, and use logic to survive
- **Score-Based Progression** – Reach target scores to advance, not just clear the board
- **Par Time System** – Beat the clock for bonus points (faster = more points)
- **4 Round Types** – Each with unique difficulty and rewards

### Progression System
- **Calicata Levels (1-10)** – Climb through 10 tiers of increasing difficulty
- **Persistent Best Progress** – Your highest Calicata level is saved even after Game Over
- **Economy System** – Earn money from completed rounds to spend in the future Shop (Beta+)
- **Statistics Tracking** – Track mines found, tiles revealed, runs completed, and best scores

### Visual Features
- **Round-Specific UI Colors** – Each round type has its own color scheme
- **End of Round Summary** – Detailed breakdown of scores, bonuses, and earnings
- **Dynamic Sidebar** – Real-time display of lives, money, progress, and more
- **Custom Mine Graphics** – Simple black circle design for clean visibility

---

## Rounds

| Round | Grid Size | Mines | Target Score | Par Time | Reward |
|-------|-----------|-------|--------------|----------|--------|
| **Catito** | 8×8 | 10 | 80 | 45s | $0 (Tutorial) |
| **Sondeo** | 12×12 | 20 | 420 | 90s | $3 |
| **Cata** | 16×16 | 35 | 700 | 180s | $4 |
| **Calicata** | 16×16 | 40 | 1300+ | 300s | $5 |

> **Note:** Calicata Target Score increases with each level (1/10 to 10/10)

---

## Controls

| Action | Input |
|--------|-------|
| **Reveal Tile** | Left Click |
| **Flag/Unflag Tile** | Right Click |
| **Pause Game** | `ESC` |
| **Continue/Confirm** | `C` or Click Button |
| **Return to Menu** | `M` |
| **Skip Round (Sondeo/Cata)** | `S` (when available) |
| **Retry (Game Over)** | `R` |

---

## Scoring System

### Base Score
- **Numbered Tiles:** Value × 2 points (e.g., revealing a "3" = 6 points)
- **Empty Tiles (0):** 1 point each

### Par Time Bonus
- Finish **under** Par Time → Earn bonus points
- Finish **over** Par Time → No bonus (no penalty)
- Bonus rounded to nearest integer

### Total Score
### Round Completion
- **COMPLETED:** Total Score ≥ Target Score → Advance + Earn Money
- **FAILED:** Total Score < Target Score → Game Over

---

## Game Over Conditions

| Condition | Result |
|-----------|--------|
| **Lives Reach 0** | Game Over – All progress reset except Best Calicata |
| **Failed Target Score** | Game Over after End of Round summary |
| **Quit to Menu** | Progress saved, can resume later |

### On Game Over:
-  `calicataLevel` resets to 1
-  `money` resets to $0
-  `sondeoBeaten`, `cataBeaten`, `calicataBeaten` reset
-  `bestCalicata` is **saved** (permanent progress)
-  Statistics are **saved** (mines found, tiles revealed, etc.)

---

## File Structure
Scopae Minarum/
├── build/
│   └── scopae_minarum.exe      # Compiled game
├── include/
│   ├── board.h                 # Board logic declarations
│   ├── config.h                # Game configuration & constants
│   ├── game.h                  # Game state & function declarations
│   ├── menu.h                  # Menu system declarations
│   ├── save.h                  # Save/Load system declarations
│   └── ui.h                    # UI rendering declarations
├── src/
│   ├── board.c                 # Board logic (mines, reveal, flag)
│   ├── game.c                  # Game state management
│   ├── main.c                  # Main game loop
│   ├── menu.c                  # Menu & stats screens
│   ├── save.c                  # Save/Load functionality
│   └── ui.c                    # UI rendering (sidebar, boards, etc.)
├── build.bat                   # Build script (Windows + GCC)
├── save.dat                    # Save file (auto-generated)
└── README.md                   # This file

## 🛠️ Building

### Requirements
- **Compiler:** GCC (MinGW-w64) or compatible C compiler
- **Library:** Raylib (w64devkit recommended)
- **OS:** Windows (for build.bat), Linux/Mac (modify build script)

### Instructions

1. **Install Dependencies:**
   ```bash
   # Download w64devkit with raylib
   # https://github.com/raysan5/raylib


### Game Flow

MAIN MENU
    │
    ├──► PLAY ──────► CATITO (Tutorial)
    │                    │
    │                    ▼
    │               SONDEO/CATA/CALICATA SELECTOR
    │                    │
    │         ┌──────────┼──────────┐
    │         │          │          │
    │         ▼          ▼          ▼
    │     SONDEO      CATA     CALICATA
    │         │          │          │
    │         └──────────┼──────────┘
    │                    │
    │                    ▼
    │              END OF ROUND
    │                    │
    │         ┌──────────┴──────────┐
    │         │                     │
    │         ▼                     ▼
    │    COMPLETED              FAILED
    │         │                     │
    │         ▼                     ▼
    │    + Money               Game Over
    │         │                     │
    │         └──────────┬──────────
    │                    │
    │                    ▼
    │              Game Over / Menu
    │
    └──► STATS ──────► View Statistics