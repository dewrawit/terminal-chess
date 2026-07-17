#include "Piece.h"
#include "../Position.h"
#include "../Board.h"
#include "../Player.h"
#include "../Game.h"
#include "../MoveData.h"
#include "../Movement.h"

#include <vector>
#include <string_view>
#include <cassert>
#include <algorithm>
#include <ranges>
#include <memory>

using SV = std::string_view;

    //Piece
    Piece::Piece(const PieceData& data)
        : m_data{ data }
    {};

    PieceData::Type Piece::getPieceType() const { return m_data.type; }
    SV Piece::getName() const { return m_data.name; }
    const Position& Piece::getPosition() const { return m_data.position; }
    char Piece::getSymbol() const { return m_data.symbol; } 

    Piece::~Piece() = default;    

    std::vector<MoveData> Piece::getPseudoLegalPositions(const Board&, const Player&) const
    {
        return std::vector<MoveData>{};
    }
    bool Piece::pawnJustMoveTwoSquare() const
    {
        return false;
    }
    bool Piece::isWhitePiece() const
    {
        return m_data.team == PieceData::Team::white;
    }
    bool Piece::isBlackPiece() const
    {
        return m_data.team == PieceData::Team::black;
    }
    void Piece::moveTo(const Position& pos)
    {
        assert(pos.validPosition() && "Tried to moveTo() invalid position.");
        m_data.position = pos;
        if(!m_data.moved)
        {
            m_data.moved = true;
        }
    }
    bool Piece::isCaptured() const
    {
        return m_data.captured;
    }
    void Piece::setToCaptured()
    {
        m_data.captured = true;
    }
    void Piece::assignTeam(PieceData::Team team)
    {
        m_data.team = team;
    }
    PieceData::Team Piece::getTeam() const
    {
        return m_data.team;
    }
    std::vector<Position> Piece::getControlledPositions(const Board&) const
    {
        return std::vector<Position>{};
    }
    bool Piece::hasMoved() const
    {
        return m_data.moved;
    }
    std::vector<MoveData> Piece::getLegalPositions(const Board& board,
         const Player& player, const Player& opponent) const
    {
        //Only king needs the opponent parameter (to check control square for castling)
        std::vector<MoveData> pseudoLegalMoves { getPseudoLegalPositions(board, opponent) };

        std::erase_if(pseudoLegalMoves, 
            [&board, &player, &opponent, this](const MoveData& moveData) -> bool 
            {
                Player playerCopy { player };
                Player opponentCopy { opponent };

                Board newBoard { playerCopy, opponentCopy };
                Movement movement { this->getPosition(), moveData.position, moveData.type };

                newBoard.movePiece(playerCopy, movement);
                Game::updatePlayerPieces(opponentCopy); //remove any "setToCaptured" piece
                //So that it doesn't get calculate control square to see if king in check

                auto itKing { std::ranges::find_if(playerCopy.getPieceVector(), 
                    [](const std::unique_ptr<Piece>& piecePtr) -> bool
                    {
                        return piecePtr->getPieceType() == PieceData::Type::king;
                    }
                )};
                assert(itKing != playerCopy.getPieceVector().end() && "King not found.\n");

                King* king { dynamic_cast<King*>(itKing->get()) };
                assert(king && "King Downcast fail in legal move detection.\n");
                return king->inCheck(newBoard, opponentCopy);
            });
        return pseudoLegalMoves;
    }
    std::unique_ptr<Piece> Piece::clone() const
    {
        //since class Piece and it's derive doesn't have any pointers. We can just copy the value
        //using compiler-generated copy constructor
        return std::make_unique<Piece>(*this);
    }