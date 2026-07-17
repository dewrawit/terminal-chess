#include "Piece.h"
#include "Rook.h"
#include "../Position.h"
#include "../Board.h"
#include "../MoveData.h"

#include <array>
#include <vector>
#include <utility>
#include <memory>

Rook::Rook(Position pos) : Piece{ PieceData{"Rook",PieceData::Type::rook,'R', pos} } {}
Rook::Rook(Position pos, PieceData::Team team) : 
    Piece{ PieceData{"Rook",PieceData::Type::rook,'R', pos, team} } {}

std::vector<MoveData> Rook::getPseudoLegalPositions(const Board& board, const Player&) const
{

    std::vector<MoveData> pseudoLegalPositions {};
    Position current{ m_data.position };

    //Rooks: Keep going in each direction until it hits enemy piece or out of bounds

    //Using structured bindings
    for(const auto& [dx, dy] : directions)
    {
        int distance { 0 };
        while(true)
        {
            ++distance;
            Position newPos { current + std::pair{dx * distance, dy * distance} };

            if(!newPos.validPosition()) //Check if it's within board bounds
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
std::vector<Position> Rook::getControlledPositions(const Board& board) const
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

            if(!newPos.validPosition()) //Check if it's within board bounds
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
std::unique_ptr<Piece> Rook::clone() const
{
    return std::make_unique<Rook>(*this);
}