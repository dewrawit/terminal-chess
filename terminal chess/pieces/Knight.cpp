#include "Knight.h"
#include "Piece.h"
#include "../Board.h"
#include "../Position.h"
#include "../MoveData.h"

#include <array>
#include <vector>
#include <utility>
#include <memory>

Knight::Knight(Position pos)
    : Piece{ PieceData{"Knight", PieceData::Type::knight, 'N', pos} }
{
}
Knight::Knight(Position pos, PieceData::Team team)
    : Piece{ PieceData{"Knight", PieceData::Type::knight, 'N', pos, team} }
{
}


std::vector<MoveData> Knight::getPseudoLegalPositions(const Board& board, const Player&) const
{
    std::vector<MoveData> pseudoLegalPositions {};
    Position current{ m_data.position };

    for(const auto& [dx, dy] : directions)
    {
        Position newPos { current + std::pair{dx, dy} };

        if(!newPos.validPosition())
        {
            continue;
        }

        //Occupied but from same team
        if(board.isOccupied(newPos) && board.sameTeams(current, newPos))
        {
            continue;
        }

        pseudoLegalPositions.emplace_back(newPos, MoveData::Type::normal);
    }

    return pseudoLegalPositions;
}
std::vector<Position> Knight::getControlledPositions(const Board&) const
{
    std::vector<Position> controlPositions {};
    Position current{ m_data.position };

    for(const auto& [dx, dy] : directions)
    {
        Position newPos { current + std::pair{dx, dy} };

        if(!newPos.validPosition())
        {
            continue;
        }

        controlPositions.push_back(newPos);
    }

    return controlPositions;
}
std::unique_ptr<Piece> Knight::clone() const
{
    return std::make_unique<Knight>(*this);
}