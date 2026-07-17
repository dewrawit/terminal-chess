#include "Piece.h"
#include "Pawn.h"
#include "../Position.h"
#include "../Board.h"
#include "../MoveData.h"

#include <array>
#include <vector>
#include <utility>
#include <memory>

//Pawn
Pawn::Pawn(Position pos) 
    : Piece{ PieceData{"Pawn",PieceData::Type::pawn,'P', pos} } {}

Pawn::Pawn(Position pos, PieceData::Team team) 
    : Piece{ PieceData{"Pawn",PieceData::Type::pawn,'P', pos, team} } {}

bool Pawn::pawnJustMoveTwoSquare() const
{
    return justMovedTwoSquares;
}
std::vector<MoveData> Pawn::getPseudoLegalPositions(const Board& board, const Player&) const
{
    std::vector<MoveData> pseudoLegalPositions {};
    Position current{ m_data.position };

    Position newPos {};
    int dir { isWhitePiece() ? 1 : -1 };

    //First move, two squares
    if(!hasMoved())
    {
        newPos = current + std::pair{0,2 * dir};
        if(newPos.validPosition() && 
        !board.isOccupied(newPos) && 
        !board.isOccupied(newPos - std::pair{0,1 * dir})) //Can move one square
        {
            pseudoLegalPositions.emplace_back(newPos, MoveData::Type::normal);
        }
    }

    //one square move
    newPos = current + std::pair{0,1 * dir};
    if(newPos.validPosition() && !board.isOccupied(newPos))
    {
        pseudoLegalPositions.emplace_back(newPos, MoveData::Type::normal);
    }

    //cant be constexpr because dir is known at runtime
    //Also can't be static because static variable created once 
    //So If white pawn creates it... black pawn will use white pawn's capture dir!)
    const std::array captureDirection {
        std::pair{-1,1 * dir},
        std::pair{1,1 * dir},
    };

    //Capture
    for(const auto& [dx, dy] : captureDirection)
    {
        newPos = current + std::pair{ dx, dy };
        if(newPos.validPosition() && 
            board.isOccupied(newPos) && 
            board.differentTeams(current,newPos))
        {
            pseudoLegalPositions.emplace_back(newPos, MoveData::Type::normal);
            continue;
        }

        //En passant
        if(newPos.validPosition() && 
        board.isOccupied(newPos - std::pair{0,1 * dir}) &&
        board.getPieceAtPosition(newPos - std::pair{0,1 * dir}).getPieceType() == PieceData::Type::pawn)
        {
            if(board.getPieceAtPosition(newPos - std::pair{0,1 * dir}).pawnJustMoveTwoSquare())
            {
                pseudoLegalPositions.emplace_back(newPos, MoveData::Type::enpassant);
                continue;
            }
        }
    }       
    return pseudoLegalPositions;
}
std::vector<Position> Pawn::getControlledPositions(const Board&) const
{
    std::vector<Position> controlPositions {};
    Position current{ m_data.position };

    Position newPos {};
    int dir { isWhitePiece() ? 1 : -1 };

    const std::array captureDirection {
        std::pair{-1,1 * dir},
        std::pair{1,1 * dir},
    };

    for(const auto& [dx, dy] : captureDirection)
    {
        newPos = current + std::pair{ dx, dy };
        if(newPos.validPosition())
        {
            controlPositions.push_back(newPos);
            continue;
        }
    }       
    return controlPositions;
}
void Pawn::moveTo(const Position& pos)
{

    if(justMovedTwoSquares)
    {
        justMovedTwoSquares = false;
    }

    if((pos + std::pair<int,int>{0,2} == this->getPosition()) || 
        (pos - std::pair<int,int>{0,2} == this->getPosition()))
    {
        justMovedTwoSquares = true;
    }

    Piece::moveTo(pos);
}
void Pawn::disableJustMovedTwoSquares()
{
    justMovedTwoSquares = false;
}
std::unique_ptr<Piece> Pawn::clone() const
{
    return std::make_unique<Pawn>(*this);
}