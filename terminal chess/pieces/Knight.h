#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"
#include <vector>
#include <memory>

class Board;
struct Position;
struct MoveData;

class Knight : public Piece
{
private:
    //L shape directions
    static constexpr std::array directions {
        //Up
        std::pair{1,2},
        std::pair{-1,2},
        //Down
        std::pair{1,-2},
        std::pair{-1,-2},
        //Left
        std::pair{-2,1},
        std::pair{-2,-1},
        //Right
        std::pair{2,1},
        std::pair{2,-1},
    };
public:
    Knight(Position pos);
    Knight(Position pos, PieceData::Team team);

    std::vector<MoveData> getPseudoLegalPositions(const Board& board, const Player&) const override;
    std::vector<Position> getControlledPositions(const Board&) const override;
    std::unique_ptr<Piece> clone() const override;
};

#endif