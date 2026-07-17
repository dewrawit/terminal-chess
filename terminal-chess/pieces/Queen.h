#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"
#include <vector>
#include <memory>

class Board;
struct Position;
struct MoveData;

class Queen : public Piece
{
private:
    static constexpr std::array directions {
        std::pair{0,1}, //Up
        std::pair{0,-1}, //Down
        std::pair{-1,0}, //Left
        std::pair{1,0}, //Right
        std::pair{1,1}, //top right corner
        std::pair{1,-1}, //bottom right corner
        std::pair{-1,-1}, //bottom left corner
        std::pair{-1,1}, //top left corner
    };
public:
    Queen(Position pos);
    Queen(Position pos, PieceData::Team team);
    std::vector<MoveData> getPseudoLegalPositions(const Board&, const Player&) const override;
    std::vector<Position> getControlledPositions(const Board&) const override;
    std::unique_ptr<Piece> clone() const override;
};

#endif