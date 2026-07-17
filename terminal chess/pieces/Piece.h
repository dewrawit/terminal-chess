#ifndef PIECE_H
#define PIECE_H

#include "../Position.h"
#include <string>
#include <string_view>
#include <vector>
#include <memory>

struct Position;
class Board;
class Player;
struct MoveData;

using SV = std::string_view;
using uchar = unsigned char;

struct PieceData
{
    enum class Type { pawn, rook, king, queen, bishop, knight };
    enum class Team { white, black };

    std::string name{};
    Type type{};
    char symbol{};
    Position position{};
    Team team{ Team::white };
    bool captured{ false };
    bool moved { false };
};
class Piece
{       
    protected:
    PieceData m_data {};

    public:
    Piece(const PieceData& data);
    virtual ~Piece(); 

    PieceData::Type getPieceType() const;
    SV getName() const;
    const Position& getPosition() const;
    char getSymbol() const;

    //Had to pass a Player because the king needs to see squares opponent controlled
    virtual std::vector<MoveData> getPseudoLegalPositions(const Board&, const Player&) const;
    std::vector<MoveData> getLegalPositions(const Board& board
        , const Player& player, const Player& opponent) const;

    virtual bool pawnJustMoveTwoSquare() const;
    bool isWhitePiece() const;
    bool isBlackPiece() const;
    virtual void moveTo(const Position& pos);
    bool isCaptured() const;
    void setToCaptured();
    void assignTeam(PieceData::Team team);
    PieceData::Team getTeam() const;
    bool hasMoved() const;

    virtual std::vector<Position> getControlledPositions(const Board&) const;

    //Legal Positions VS Controlled Position
    //I think the only diffrence is it can 'control' friendly piece
    //whilst legal position if theres a peice it must be diffrent teams

    virtual std::unique_ptr<Piece> clone() const;
};

#endif