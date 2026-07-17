#ifndef GAME_H
#define GAME_H

#include <optional>
#include <utility>
#include <vector>
#include <memory>

struct Position;
class Player;
class Board;
struct Movement;
struct MoveData;
struct TurnInput;
struct StartInput;

namespace Game
{

    enum class Result { checkmate, stalemate, none };

    StartInput getStartingPosition(const Player& player);
    std::optional<MoveData> getDestinationPosition(const Position& startPos, const Board& board, 
        const Player& player, const Player& opponent );
    TurnInput inputPosition(const Player& player, const Player& opponent, const Board& board);
    void updatePlayerPieces(Player& player);
    std::vector<Position> getAllControlledSquare(const Player& player, const Board& board);
    std::optional<char> getPromotedPieceSymbol();
   bool noLegalMoves(const Player& player, const Player& opponent, const Board& board);

    //bool kingCaptured(const Player& player); temporary win check
    Result detectGameEnd(const Player& player, const Player& opponent, 
        const Board& board);

    bool takeTurn(Player& player, Player& opponent, Board& board);
    bool acceptDraw();
}

#endif