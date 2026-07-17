#ifndef BOARD_H
#define BOARD_H

#include <array>
#include "Constants.h"
#include <utility>

// Internal representation: file is lowercase a-h, rank is 1-8
class Piece;
struct Position;
class Player;
struct Movement;

//Points to the piece that player owned
using ChessBoard = 
std::array<std::array<Piece*,ChessConstants::BoardDimension>,ChessConstants::BoardDimension>;


class Board
{
    private:
    ChessBoard chessboard{};

    public:
    Board(const Player& p1, const Player& p2);

    void makeEveryTileNull();
    std::size_t getFileBoardIndex(const Position& pos) const;
    std::size_t getRankBoardIndex(const Position& pos) const;
    void drawBoard() const;
    bool isOccupied(const Position& pos) const;
    

    //Now getPieceAtPosition works for const and non-const
    //const -> Ex. just Checking which side it's on
    //non-const -> Ex. move piece

    Piece& getPieceAtPosition(const Position& pos); //For modifiable version
    const Piece& getPieceAtPosition(const Position& pos) const;

    bool differentTeams(const Position& pos1, const Position& pos2) const;
    bool sameTeams(const Position& pos1, const Position& pos2) const;
    
    void movePiece(Player& player, const Movement& moveData);
    bool isControlled(const Position& pos, const Board& board, const Player& opponent) const;
    bool kingInCheck(const Player& player, const Player& opponent) const;
    void overwriteTileData(const Position& source, const Position& dest);
    void setToNull(const Position& pos);
    void handleCapture(const Position& source, const Position& dest);
};


#endif