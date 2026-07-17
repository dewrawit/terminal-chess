#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include <algorithm>
#include <ranges>
#include "Constants.h"
#include "pieces/AllPieces.h"
#include "Board.h"

class Player
{
    public:
    using SV = std::string_view;
    enum class Side { white, black, unknown };

    private:
    std::string m_name{};
    std::vector<std::unique_ptr<Piece>> m_pieces{};
    Side m_side{ Side::unknown };

    public:
    Player(SV name, Side side);

    //Copying a player
    Player(const Player& player);

    SV getName() const { return m_name; }
    Side getSide() const { return m_side; }
    std::vector<std::unique_ptr<Piece>>& getPieceVector() { return m_pieces; }
    const std::vector<std::unique_ptr<Piece>>& getPieceVector() const { return m_pieces; }
    bool isWhite() const { return m_side == Player::Side::white; }
    bool isBlack() const { return m_side == Player::Side::black; }

    template <typename PromotedPiece>
    Piece* replacePromotedPawn(const Position& pos)
    {
        auto pawnIt { std::ranges::find_if(m_pieces, 
            [pos](const auto& piecePtr) -> bool
            {
                return piecePtr->getPosition() == pos &&
                piecePtr->getPieceType() == PieceData::Type::pawn;
            }
        )};

        if(pawnIt == m_pieces.end())
        {
            assert(false && "Promoted piece at this position does not qualify.\n");
        }

        PieceData::Team team { isWhite() ? PieceData::Team::white : PieceData::Team::black };
        
        //TBD test why this segfault (maybe overwrite data did something...)
        auto newPiecePtr { std::make_unique<PromotedPiece>(pos, team) };
        *pawnIt = std::move(newPiecePtr); 

        //Aha found it it's Move semantic
        //pawnIt is unique_ptr points to Piece... then std::move(Queen)
        //makes pawnIt destroys Pawn and point to Queen instead, then newPiecePtr becomes nullptr
        //But the board still points at the dead pawn -> segfault

        //Gotta fix by update board pointer
        return pawnIt->get();
    }
};

#endif