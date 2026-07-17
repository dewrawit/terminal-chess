#include "Piece.h"
#include "Bishop.h"
#include "../Position.h"
#include "../Board.h"
#include "../MoveData.h"

#include <vector>
#include <array>
#include <utility>
#include <memory>

Bishop::Bishop(Position pos)
    : Piece{ PieceData{"Bishop", PieceData::Type::bishop, 'B', pos} }
{
}
Bishop::Bishop(Position pos, PieceData::Team team)
    : Piece{ PieceData{"Bishop", PieceData::Type::bishop, 'B', pos, team} }
{
}

std::vector<MoveData> Bishop::getPseudoLegalPositions(const Board& board, const Player&) const
{
    
    std::vector<MoveData> pseudoLegalPositions {};
    Position current{ m_data.position };

    //Bishop, same as rooks but diffrent direction

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
                    pseudoLegalPositions.emplace_back(newPos, MoveData::Type::normal); //Can capture
                }
                break;
            }
            pseudoLegalPositions.emplace_back(newPos, MoveData::Type::normal);
        }
    }
    return pseudoLegalPositions;
}
std::vector<Position> Bishop::getControlledPositions(const Board& board) const
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
std::unique_ptr<Piece> Bishop::clone() const
{
    return std::make_unique<Bishop>(*this);
}