#include "Piece.h"
#include "King.h"
#include "../Position.h" // ../ means the parent folder
#include "../Board.h"
#include "../Game.h"
#include "../Player.h"
#include "../MoveData.h"

#include <array>
#include <utility>
#include <memory>

King::King(Position pos)
    : Piece{ PieceData{"King", PieceData::Type::king, 'K', pos} }
{
}
King::King(Position pos, PieceData::Team team)
    : Piece{ PieceData{"King", PieceData::Type::king, 'K', pos, team} }
{
}

std::vector<MoveData> King::getPseudoLegalPositions(const Board& board, const Player& opponent) const
{
    std::vector<MoveData> pseudoLegalPositions {};
    Position current{ m_data.position };

    //King: rook + bishop but only one square
    for(const auto& [dx, dy] : directions)
    {
        Position newPos { current + std::pair{ dx, dy } };

        if(!newPos.validPosition())
        {
            continue;
        }

        if(board.isOccupied(newPos) && board.sameTeams(current,newPos))
        {
            continue;
        }

        pseudoLegalPositions.emplace_back(newPos, MoveData::Type::normal);
    }

    //Castling
    //Check queenside then king. 
    //If queenside -> check 2 squares left of king
    //If kingside-> check 2 squares right of king
    
    static constexpr std::array rookDirRelativeKing
    {
        std::pair{-4, 0}, //Queenside Rook
        std::pair{3, 0}, //Kingside Rook
    };

    bool checkQueenSide { true };
    for(const auto& [dx, dy] : rookDirRelativeKing)
    {
        int dir { checkQueenSide ? -1 : 1 }; 

        //If king already move or is in check, theres no point checking, you can't castle
        if(hasMoved())
        {
            checkQueenSide = false;
            continue;
        }
            
        if(inCheck(board, opponent))
        {
            checkQueenSide = false;
            continue;
        }

        //Rook in corner hasn't moved, king not in check and the square they'll end up isnt controlled
        Position rookPos { current + std::pair{ dx, dy }};
        if(!(
            (board.isOccupied(rookPos)) &&
            (board.sameTeams(current, rookPos)) &&
            (board.getPieceAtPosition(rookPos).getPieceType() == PieceData::Type::rook) &&
            (!(board.getPieceAtPosition(rookPos).hasMoved()))
        ))
        {
            checkQueenSide = false;
            continue; //check the other rook
        }

        bool canCastle { true };
        for(int distance {1}; distance <= 2; ++distance)
        {
            Position checkedSquare { current + std::pair{distance * dir, 0} };
            if(board.isControlled(checkedSquare, board, opponent) || board.isOccupied(checkedSquare))
            {
                canCastle = false;
                break;
            }
        }

        if(!canCastle)
        {
            checkQueenSide = false;
            continue;
        }
            

        //Game convention: Castle by select destination as the rook
        if(checkQueenSide)
            pseudoLegalPositions.emplace_back(rookPos, MoveData::Type::castlingQ); 
        else
            pseudoLegalPositions.emplace_back(rookPos, MoveData::Type::castlingK);

        checkQueenSide = false;
    }

    return pseudoLegalPositions;
}
std::vector<Position> King::getControlledPositions(const Board&) const
{
    std::vector<Position> controlPositions {};
    Position current{ m_data.position };

    //King: rook + bishop but only one square
    for(const auto& [dx, dy] : directions)
    {
        Position newPos { current + std::pair{ dx, dy } };

        if(!newPos.validPosition())
        {
            continue;
        }
        controlPositions.push_back(newPos);
    }

    return controlPositions;
}
bool King::inCheck(const Board& board, const Player& opponent) const
{
    return board.isControlled(this->getPosition(), board, opponent);
}
std::unique_ptr<Piece> King::clone() const
{
    return std::make_unique<King>(*this);
}