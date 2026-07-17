#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"
#include <vector>
#include <array>
#include <utility>
#include <memory>

class Board;
struct Position;
struct MoveData;

class Bishop : public Piece
{
private:
    static constexpr std::array directions {
        std::pair{1,1}, //top right corner
        std::pair{1,-1}, //bottom right corner
        std::pair{-1,-1}, //bottom left corner
        std::pair{-1,1}, //top left corner
    };
public:
    Bishop(Position pos);
    Bishop(Position pos, PieceData::Team team);

    std::vector<MoveData> getPseudoLegalPositions(const Board& board, const Player&) const override;
    std::vector<Position> getControlledPositions(const Board&) const override;
    std::unique_ptr<Piece> clone() const override;
};

#endif