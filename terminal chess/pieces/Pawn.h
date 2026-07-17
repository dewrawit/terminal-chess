#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"
#include <vector>
#include <memory>

struct Position;
class Board;
struct MoveData;

class Pawn : public Piece
{
    private:
    bool justMovedTwoSquares { false };

    public:
    Pawn(Position pos);
    Pawn(Position pos, PieceData::Team team);
    bool pawnJustMoveTwoSquare() const override;
    std::vector<MoveData> getPseudoLegalPositions(const Board& board, const Player&) const override;
    std::vector<Position> getControlledPositions(const Board&) const override;
    void moveTo(const Position& pos) override;
    void disableJustMovedTwoSquares();
    std::unique_ptr<Piece> clone() const override;

};

#endif