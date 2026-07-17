#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"
#include <vector>
#include <array>
#include <utility>
#include <memory>

struct Position;
class Board;
struct MoveData;

class Rook : public Piece
{
    private:
    static constexpr std::array directions {
        std::pair{0,1}, //Up
        std::pair{0,-1}, //Down
        std::pair{-1,0}, //Left
        std::pair{1,0}, //Right
    };

    public:
    Rook(Position pos);
    Rook(Position pos, PieceData::Team team);
    std::vector<MoveData> getPseudoLegalPositions(const Board& board, const Player&) const override;
    std::vector<Position> getControlledPositions(const Board&) const override;
    std::unique_ptr<Piece> clone() const override;
};

#endif