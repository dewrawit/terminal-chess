#ifndef KING_H
#define KING_H

#include "Piece.h"
#include <vector>
#include <array>
#include <utility>
#include <memory>

struct Position;
class Board;
struct MoveData;

class King : public Piece
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
    King(Position pos);
    King(Position pos, PieceData::Team team);
    
    std::vector<MoveData> getPseudoLegalPositions(const Board& board, const Player& opponent) const override;
    std::vector<Position> getControlledPositions(const Board&) const override;
    bool inCheck(const Board&, const Player& opponent) const;
    std::unique_ptr<Piece> clone() const override;
};

#endif