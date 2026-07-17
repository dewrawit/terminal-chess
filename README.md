# terminal-chess
A playable chess game in terminal using modern C++.

How to use: 
In each turn, type the square of the piece you want to move (Ex. e2, a7, h1 etc.). (You can also offer draw or resign)
Then type the destination you want that piece to go to. It will work as long as the move is legal
Note: To castle, input the king's square, then input the square containing the rook that you want to castle into as the destination.

This program follows all the normal chess rule but some rules are not included:
-50 move rule
-3 fold repetition
-draw by insufficient material

## Features
- Legal move validation
- Castling
- En passant
- Pawn promotion
- Check / Checkmate
- Stalemate
- Draw offer
- Resignation

## Requirements
- A C++20 (or newer) compiler
- g++

## Build (Linux)
```bash
g++ -std=c++20 *.cpp Pieces/*.cpp -o chess
```

## Run
```bash
./chess
```

## Windows
Compile with MinGW or Visual Studio:
```bash
g++ -std=c++20 *.cpp Pieces/*.cpp -o chess.exe
```

Then run:
```bash
chess.exe
```
