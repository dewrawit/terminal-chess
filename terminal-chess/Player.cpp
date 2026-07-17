#include "Player.h"

Player::Player(SV name, Side side)
    : m_name{ name }, m_side{ side }
{
    //Set up all the pieces
    int pawnStartingRank{};
    int pieceStartingRank{};

    switch(side)
    {
        case Side::white:
            pawnStartingRank = ChessConstants::WhitePawnStartingRank;
            pieceStartingRank = ChessConstants::WhitePieceStartingRank;
            break;
        case Side::black:
            pawnStartingRank = ChessConstants::BlackPawnStartingRank;
            pieceStartingRank = ChessConstants::BlackPieceStartingRank;
            break;
        default: assert(false && "Invalid Side detected in construction.\n");
    }

    //Pawn
    for(int i {0}; i < ChessConstants::InitialPawnCount; ++i)
    {
        m_pieces.emplace_back( 
            std::make_unique<Pawn>(
                Position(static_cast<char>('a' + i), pawnStartingRank)
            )
        );
    }

    //Other Pieces
    for(int i {1}; i <= ChessConstants::InitialValuablePieceCount; ++i)
    {
        if(i == 1 || i == 8)
        {
            m_pieces.emplace_back(
                std::make_unique<Rook>(
                    Position(static_cast<char>('a' + i - 1), pieceStartingRank)
                )
            );
        }
        else if(i == 2 || i == 7)
        {
            m_pieces.emplace_back(
                std::make_unique<Knight>(
                    Position(static_cast<char>('b' + i - 2), pieceStartingRank)
                )
            );
        }
        else if(i == 3 || i == 6)
        {
            m_pieces.emplace_back(
                std::make_unique<Bishop>(
                    Position(static_cast<char>('c' + i - 3), pieceStartingRank)
                )
            );
        }
        else if(i == 4)
        {
            m_pieces.emplace_back(
                std::make_unique<Queen>(
                    Position('d', pieceStartingRank)
                )
            );
        }
        else
        {
            m_pieces.emplace_back(
                std::make_unique<King>(
                    Position('e', pieceStartingRank)
                )
            );
        }
    }

    //To not confused with white pieces, black pieces will be lowercase symbol
    if(isBlack())
    {
        for(auto& piece : m_pieces)
        {
            piece->assignTeam(PieceData::Team::black);
        }
    }
    else
    {
        for(auto& piece : m_pieces)
        {
            piece->assignTeam(PieceData::Team::white);
        }
    }
}

//Copying a player
Player::Player(const Player& player)
    : m_name{ player.m_name }, m_side{ player.m_side }
{
    //Copying each piece
    for(const auto& piecePtr : player.m_pieces)
    {
        this->m_pieces.push_back(piecePtr->clone());
    }
}