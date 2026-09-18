# Bridg-It

A graphical implementation of the connection game Bridg-It in C++, with a scalable board, undo support and a computer opponent.

![C++](https://img.shields.io/badge/C%2B%2B-00599C?logo=cplusplus&logoColor=white)
![WinBGIm](https://img.shields.io/badge/graphics-WinBGIm-informational)

## The game

Two players alternate placing pieces on a grid. A player wins by completing an unbroken path of their own pieces between their two opposite edges — top to bottom for player 1, left to right for player 2.

## Features

- Board size derived from a piece count: `matrix_size = 2 * pieces + 1`, so the grid scales with the chosen difficulty
- Path detection over the grid to decide the winner after every move
- Undo, available at any point before the game ends
- Computer opponent with a simple heuristic: prefer extending two cells up or down when the target is free and in bounds, otherwise fall back to a random empty cell
- Rendered board, move counter and win screen, with audio feedback on move

## Project structure

```
main.cpp         # entry point, game loop
game.cpp/.h      # board state, move handling, win detection, AI
graph.cpp/.h     # rendering helpers on top of WinBGIm
draw_matrix.cbp  # Code::Blocks project
bridg-it-rules   # rules reference and board assets
```

## Build

Open `draw_matrix.cbp` in Code::Blocks with the WinBGIm graphics library available, then build and run. The project targets Windows, since it relies on `graphics.h` and `Beep()`.
