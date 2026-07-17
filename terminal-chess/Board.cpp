//Points to the piece that player owned
#include "Constants.h"
#include "Position.h"
#include "Player.h"
#include "pieces/AllPieces.h"
#include "Board.h"
#include "Game.h"
#include "Movement.h"

#include <cassert>
#include <array>
#include <iostream>
#include <typeinfo>
#include <algorithm>
#include <vector>
#include <ranges>
#include <cmath>

#define RESET   "\033[0m"
#define RED     "\033[31m" 

using ChessBoard = 
std::array<std::array<Piece*,ChessConstants::BoardDimension>,ChessConstants::BoardDimension>;

void Board::makeEveryTileNull()
{
    for(auto row {0uz}; row < ChessConstants::BoardDimension; ++row)
    {
        for(auto col {0uz}; col < ChessConstants::BoardDimension; ++col)
        {
            chessboard[row][col] = nullptr;
        }
    }
}
std::size_t Board::getFileBoardIndex(const Position& pos) const
{
    return static_cast<std::size_t>(pos.file - 'a');
}
std::size_t Board::getRankBoardIndex(const Position& pos) const
{
    return static_cast<std::size_t>(ChessConstants::BoardDimension - pos.rank);
}
Board::Board(const Player& p1, const Player& p2)
{
    makeEveryTileNull();

    //get p1 piece data
    for(const auto& piece : p1.getPieceVector())
    {
        //chessboard[rank][file]
        chessboard[getRankBoardIndex(piece->getPosition())]
        [getFileBoardIndex(piece->getPosition())] = piece.get(); 
    }

    //get p2 piece data
    for(const auto& piece : p2.getPieceVector())
    {
        //chessboard[rank][file]
        chessboard[getRankBoardIndex(piece->getPosition())]
        [getFileBoardIndex(piece->getPosition())] = piece.get(); 
    }
}
void Board::drawBoard() const
{
    std::cout << "|A B C D E F G H|\n";
    std::cout << "=================\n";
    for(auto row {0uz}; row < ChessConstants::BoardDimension; ++row)
    {
        std::cout << "|";
        for(auto col {0uz}; col < ChessConstants::BoardDimension; ++col)
        {
            if(chessboard[row][col] != nullptr)
            {
                if(chessboard[row][col]->isWhitePiece())
                {
                    std::cout << chessboard[row][col]->getSymbol();
                }
                else
                {
                    std::cout << RED << chessboard[row][col]->getSymbol() << RESET;
                }
                
            }
            else
            {
                std::cout << ".";
            }

            if(col != ChessConstants::BoardDimension - 1)
            {
                std::cout << " ";
            }
        }
        std::cout << "|";
        std::cout << "  " << std::abs(static_cast<int>(row) - ChessConstants::BoardDimension) << '\n';
    }
    std::cout << "=================\n";
};
bool Board::isOccupied(const Position& pos) const
{
    return chessboard[getRankBoardIndex(pos)][getFileBoardIndex(pos)] != nullptr;
}
Piece& Board::getPieceAtPosition(const Position& pos)
{
    assert(chessboard[getRankBoardIndex(pos)][getFileBoardIndex(pos)] != nullptr
    && "Attempt getting a piece at empty square");

    return *chessboard[getRankBoardIndex(pos)][getFileBoardIndex(pos)];
}
const Piece& Board::getPieceAtPosition(const Position& pos) const
{
    assert(chessboard[getRankBoardIndex(pos)][getFileBoardIndex(pos)] != nullptr
    && "Attempt getting a piece at empty square");

    return *chessboard[getRankBoardIndex(pos)][getFileBoardIndex(pos)];
}
bool Board::differentTeams(const Position& pos1, const Position& pos2) const
{
    assert(isOccupied(pos1) && isOccupied(pos2) && "Attempt compare sides on empty square");
    
    return getPieceAtPosition(pos1).getTeam() != getPieceAtPosition(pos2).getTeam();
}
bool Board::sameTeams(const Position& pos1, const Position& pos2) const
{
    assert(isOccupied(pos1) && isOccupied(pos2) && "Attempt compare sides on empty square");
    
    return getPieceAtPosition(pos1).getTeam() == getPieceAtPosition(pos2).getTeam();
}
void Board::setToNull(const Position& pos)
{
    chessboard[getRankBoardIndex(pos)][getFileBoardIndex(pos)] = nullptr;
}
void Board::overwriteTileData(const Position& source, const Position& dest)
{
    chessboard[getRankBoardIndex(dest)][getFileBoardIndex(dest)] = 
        chessboard[getRankBoardIndex(source)][getFileBoardIndex(source)];

    setToNull(source);
}
void Board::movePiece(Player& player, const Movement& move)
{
    Position source { move.source };
    Position dest { move.dest };
    
    //Safety
    assert(isOccupied(source) && "Tried to move empty square\n");

    /*
    Must moveTo before overwrite tile data
    getPieceAtPosition -> get whatever that tile index points to 
    (Ex. getPiece(e2) = *chessboard[7][4])

    moveTo -> only change piece position variable, set bool moved = true
    overwrite -> change what board points to

    Example: overwrite before moveTo, move pawn e2 to e4
    e2 is overwrite to null, now getPieceAtPosition gets a null, assert
    */
    
    Piece& movingPiece { getPieceAtPosition(source) };
    switch(move.type)
    {
        case MoveData::Type::normal: 
            handleCapture(source, dest);
            movingPiece.moveTo(dest);
            overwriteTileData(source, dest);
            break;

        case MoveData::Type::enpassant: 
            //Make a scope so no initialize in switch block error
            {
                int dir { getPieceAtPosition(source).isWhitePiece() ? 1 : -1 };
                Position capturedPawnLoc { dest - std::pair{0,1 * dir} };
                getPieceAtPosition(capturedPawnLoc).setToCaptured();
                movingPiece.moveTo(dest);
                setToNull(capturedPawnLoc);
                overwriteTileData(source, dest);
            }
            break;

        case MoveData::Type::castlingK:
            {
                Position rookSource { source + std::pair{3,0} };
                Position kingDest { source + std::pair{2,0} };
                Position rookDest { source + std::pair{1,0} };
                movingPiece.moveTo(kingDest);
                getPieceAtPosition(rookSource).moveTo(rookDest);
                overwriteTileData(source, kingDest);
                overwriteTileData(rookSource, rookDest);
            }
            break;
        
        case MoveData::Type::castlingQ:
            {
                Position rookSource { source - std::pair{4,0} };
                Position kingDest { source - std::pair{2,0} };
                Position rookDest { source - std::pair{1,0} };
                movingPiece.moveTo(kingDest);
                getPieceAtPosition(rookSource).moveTo(rookDest);
                overwriteTileData(source, kingDest);
                overwriteTileData(rookSource, rookDest);
            }
            break;
        
        //Promotion TBD
        case MoveData::Type::promoteB:
            handleCapture(source, dest);
            movingPiece.moveTo(dest);
            overwriteTileData(source, dest);
            
            chessboard[getRankBoardIndex(dest)][getFileBoardIndex(dest)]
            = player.replacePromotedPawn<Bishop>(dest);
            break;
        case MoveData::Type::promoteN:
            handleCapture(source, dest);
            movingPiece.moveTo(dest);
            overwriteTileData(source, dest);
            
            chessboard[getRankBoardIndex(dest)][getFileBoardIndex(dest)]
            = player.replacePromotedPawn<Knight>(dest);
            break;
        case MoveData::Type::promoteQ:
            handleCapture(source, dest);
            movingPiece.moveTo(dest);
            overwriteTileData(source, dest);
            
            chessboard[getRankBoardIndex(dest)][getFileBoardIndex(dest)]
            = player.replacePromotedPawn<Queen>(dest);
            break;
        case MoveData::Type::promoteR:
            handleCapture(source, dest);
            movingPiece.moveTo(dest);
            overwriteTileData(source, dest);
            
            chessboard[getRankBoardIndex(dest)][getFileBoardIndex(dest)]
            = player.replacePromotedPawn<Rook>(dest);
            break;
        default:
            assert(false && "This shouldn't appear, unknown MoveData::Type name\n");
    }
}
bool Board::isControlled(const Position& targetPos, const Board& board, const Player& opponent) const
{
    std::vector<Position> enemyAllControlPos { Game::getAllControlledSquare(opponent, board) };
    auto it { std::ranges::find_if(enemyAllControlPos, 
        [targetPos](const Position& pos) -> bool
        {
            return targetPos == pos;
        })
    };

    return it != enemyAllControlPos.end(); //Found match?
}
bool Board::kingInCheck(const Player& player, const Player& opponent) const
{
    auto it { std::ranges::find_if(player.getPieceVector(),
        [](const auto& piecePtr) -> bool
        {
            return piecePtr->getPieceType() == PieceData::Type::king;
        })
    };

    assert(it != player.getPieceVector().end() && "Unable to find king in Board::kingInCheck()\n");

    //Downcasting
    King* targetKing = dynamic_cast<King*>(it->get()); //address of what unique_ptr points to

    assert(targetKing && "Somehow downcast fail\n");

    return targetKing->inCheck(*this,opponent);
}
void Board::handleCapture(const Position& source, const Position& dest)
{
    if(isOccupied(dest))
    {
        assert(differentTeams(source,dest) && "Tried to Capture friendly piece\n");
        getPieceAtPosition(dest).setToCaptured();
    }
}