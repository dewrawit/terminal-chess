#include "Piece.h"
#include "Queen.h"
#include "../Board.h"
#include "../Position.h"
#include "../MoveData.h"

#include <array>
#include <vector>
#include <utility>
#include <memory>
Queen::Queen(Position pos)
    : Piece{ PieceData{"Queen", PieceData::Type::queen, 'Q', pos} }
{
}
Queen::Queen(Position pos, PieceData::Team team)
    : Piece{ PieceData{"Queen", PieceData::Type::queen, 'Q', pos, team} }
{
}

std::vector<MoveData> Queen::getPseudoLegalPositions(const Board& board, const Player&) const
{
    std::vector<MoveData> pseudoLegalPositions {};
    Position current{ m_data.position };

    //Queen = Rook + Bishop
    for(const auto& [dx, dy] : directions)
    {
        int distance { 0 };
        while(true)
        {
            ++distance;
            Position newPos { current + std::pair{dx * distance, dy * distance} };

            if(!newPos.validPosition())
            {
                break;
            }

            if(board.isOccupied(newPos))
            {
                if(board.differentTeams(current,newPos))
                {
                    pseudoLegalPositions.emplace_back(newPos, MoveData::Type::normal ); //Can capture
                }
                break;
            }

            pseudoLegalPositions.emplace_back(newPos, MoveData::Type::normal );
        }
    }

    return pseudoLegalPositions;
}
std::vector<Position> Queen::getControlledPositions(const Board& board) const
{
    std::vector<Position> controlPositions {};
    Position current{ m_data.position };

    for(const auto& [dx, dy] : directions)
    {
        int distance { 0 };
        while(true)
        {
            ++distance;
            Position newPos { current + std::pair{dx * distance, dy * distance} };

            if(!newPos.validPosition())
            {
                break;
            }
            if(board.isOccupied(newPos))
            {
                controlPositions.push_back(newPos);
                break;
            }

            controlPositions.push_back(newPos);
        }
    }

    return controlPositions;
}
std::unique_ptr<Piece> Queen::clone() const
{
    return std::make_unique<Queen>(*this);
}