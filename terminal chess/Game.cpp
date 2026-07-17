#include "Game.h"
#include "Player.h"
#include "pieces/AllPieces.h"
#include "Board.h"
#include "Position.h"
#include "Movement.h"
#include "TurnInput.h"
#include "MoveData.h"
#include "StartInput.h"

#include <iostream>
#include <string>
#include <limits>
#include <algorithm>
#include <memory>
#include <sstream>
#include <optional>
#include <ranges>
#include <cctype>

namespace Game
{  
    using SV = std::string_view;
    using uchar = unsigned char;

    StartInput getStartingPosition(const Player& player)
    {
        std::cout << "Enter location of your piece you want to move, "
        "\"draw\" or \"resign\": ";

        auto sameWord {
            [](SV a, SV b) -> bool
            {
                if(a.length() != b.length())
                    return false;

                return std::ranges::equal(a,b, [](uchar charA, uchar charB) -> bool
                {
                    return std::tolower(charA) == std::tolower(charB);
                });
            }
        };

        while(true)
        {
            std::string input{};
            std::getline(std::cin >> std::ws, input);

            while(!input.empty() && std::isspace(input[input.length() - 1]))
            {
                input.pop_back();
            }

            if(sameWord(input,"draw"))
            {
                return StartInput{ .action{ TurnInput::Action::offerDraw } };
            }
            else if(sameWord(input,"resign"))
            {
                return StartInput{ .action{ TurnInput::Action::resign } };
            }
            
            std::stringstream ss { input };
            Position startPos{};
            ss >> startPos;
            
            if(ss.fail())
            {
                ss.clear();
                std::cout << "Invalid, type again: ";
                ss.str("");
                continue;
            }

            ss >> std::ws;

            if(!ss.eof())
            {
                std::cout << "Invalid, type again: ";
                ss.str("");
                continue;
            }

            //Check if your piece exist on that square
            auto startIt { std::ranges::find_if(player.getPieceVector(),
                [startPos](const auto& piecePtr) -> bool
                {
                    return piecePtr->getPosition() == startPos;
                }
            )};

            if(startIt == player.getPieceVector().end() )
            {
                std::cout << "None of your piece exist on that square, try again: ";
                ss.str("");
                continue;
            }

            return StartInput{ .position{ startPos } };
        }
    }
    std::optional<MoveData> getDestinationPosition(const Position& startPos, const Board& board, 
        const Player& player, const Player& opponent)
    {
        const Piece& selectedPiece { board.getPieceAtPosition(startPos) };

        std::vector<MoveData> legalMoves { 
            selectedPiece.getLegalPositions(board, player, opponent) 
        };

        //For debugging
        std::cout << "All legal moves for the piece in " << startPos << ": ";
        if(legalMoves.empty())
        {
            std::cout << "[Empty!]";
        }
        else
        {
            for(const auto& move : legalMoves)
            {
                std::cout << move.position << ' ';
            }
        }
        std::cout << '\n';

        std::cout << "Enter the destination you want it to go to, '0' to change piece: ";

        while(true)
        {
            std::string input{};
            std::stringstream sstream{};
            std::getline(std::cin >> std::ws, input);
            sstream << input; //load input string into sstream

            //User cancels
            if(sstream.str() == "0")
            {
                return std::nullopt;
            }

            Position destPos{};
            sstream >> destPos; //Extract it into destPos, already overload >>
            
            if(sstream.fail())
            {
                sstream.clear();
                std::cout << "Invalid, type again: ";
                continue;
            }

            auto destIt { std::ranges::find_if( legalMoves, 
                [destPos](const auto& move) -> bool
                {
                    return move.position == destPos;
                }
            )};

            if(destIt == legalMoves.end())
            {
                std::cout << "Illegal move, try again: ";
                continue;
            }

            //Promotion check
            if(selectedPiece.getPieceType() == PieceData::Type::pawn
               && destPos.isLastRank())
            {
                std::optional<char> promotedPiece{ getPromotedPieceSymbol() };
                if(!promotedPiece.has_value())
                {
                    return std::nullopt;
                }
                switch(*promotedPiece)
                {
                    case 'Q': destIt->type = MoveData::Type::promoteQ; break;
                    case 'R': destIt->type = MoveData::Type::promoteR; break;
                    case 'N': destIt->type = MoveData::Type::promoteN; break;
                    case 'B': destIt->type = MoveData::Type::promoteB; break;
                    default: assert(false && "Invalid promoted piece.\n");
                }
            }
            return *destIt;
        }
    }
    TurnInput inputPosition(const Player& player, const Player& opponent, const Board& board)
    {
        
        std::cout << player.getName() << "'s turn.\n";

        while(true)
        {  
            //Should be able to return Movement or Resign or Draw
            StartInput startAction { getStartingPosition(player) };

            if(startAction.action == TurnInput::Action::resign || 
                startAction.action == TurnInput::Action::offerDraw)
            {
                return TurnInput{ startAction.action };
            }

            std::optional<MoveData> destPos { 
                getDestinationPosition(startAction.position, board, player, opponent) 
            };

            if(destPos.has_value()) //User didn't cancel
            {
                //gotta dereference to unwraps the optional
                Movement movement { 
                    Movement{ .source{ startAction.position }, .dest{destPos->position}, .type{destPos->type} } 
                };

                return TurnInput{ movement };
            }
        }
    }
    void updatePlayerPieces(Player& player)
    {
        std::erase_if(player.getPieceVector(),
            [](const std::unique_ptr<Piece>& piecePtr) -> bool
            {
                return piecePtr->isCaptured();
            });

        for(const auto& piecePtr : player.getPieceVector())
        {
            if(Pawn* pawn { dynamic_cast<Pawn*>(piecePtr.get()) })
            {
                if(pawn->pawnJustMoveTwoSquare())
                {
                    pawn->disableJustMovedTwoSquares();
                }
            }
        }
    }
    // bool kingCaptured(const Player& player)
    // {
    //     for(const auto& piecePtr : player.getPieceVector())
    //     {
    //         if(piecePtr->getPieceType() == PieceData::Type::king)
    //         {
    //             return false;
    //         }
    //     }
    //     return true;
    // }
    Result detectGameEnd(const Player& player, const Player& opponent, 
        const Board& board)
    {
        //Checkmate
        if(board.kingInCheck(opponent,player))
        {
            std::cout << "Check!\n";
            //Opponent in check and has no legal Moves  
            if(noLegalMoves(opponent, player, board))
            {
                return Result::checkmate;
            }      
        }
        //Stalemate
        else if(!board.kingInCheck(opponent, player) && noLegalMoves(opponent, player, board))
        {
            return Result::stalemate;
        }

        return Result::none;
    }
    std::vector<Position> getAllControlledSquare(const Player& player, const Board& board)
    {
        std::vector<Position> allControlledSquares{};

        for(const auto& piecePtr : player.getPieceVector())
        {
            std::vector<Position> controlSquare { piecePtr->getControlledPositions(board) };
            allControlledSquares.insert( 
                allControlledSquares.end(), controlSquare.begin(), controlSquare.end()
            );
        }
        
        return allControlledSquares;
    }
    bool noLegalMoves(const Player& player, const Player& opponent, const Board& board)
    {
        std::vector<MoveData> allLegalMoves{};

        for(const auto& piecePtr : player.getPieceVector())
        {
            //weird printing here
            std::vector<MoveData> legalMoves { 
                piecePtr->getLegalPositions(board, player, opponent) 
            };

            allLegalMoves.insert(allLegalMoves.end(), legalMoves.begin(), legalMoves.end());
        }

        return allLegalMoves.empty();

    }
    std::optional<char> getPromotedPieceSymbol()
    {
        std::cout << "Select Piece to promote to (Q,R,N,B) or '0' to cancel: ";

        while(true)
        {
            char input{};
            std::cin >> input;

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

            if(std::cin.gcount() > 1)
            {
                std::cout << "Invalid, try again: ";
                continue;
            }

            char symbol { static_cast<char>(std::toupper(static_cast<uchar>(input))) };
            switch(symbol)
            {
                case '0': return std::nullopt;
                case 'Q':
                case 'R':
                case 'B':
                case 'N': return symbol;
                default: break;
            }
            
            std::cout << "Invalid, try again: ";
        }
        
    }
    bool takeTurn(Player& player, Player& opponent, Board& board)
    {
        while(true)
        {
            TurnInput playerTurnInput { 
                Game::inputPosition(player, opponent, board) 
            };

            switch(playerTurnInput.action)
            {
                case TurnInput::Action::resign: 
                    std::cout << player.getName() << 
                    " resigns! " << opponent.getName() << " wins!\n";
                    return true;

                case TurnInput::Action::offerDraw:
                    std::cout << player.getName() << " offer a draw!\n";
                    if(acceptDraw())
                    {
                        std::cout << opponent.getName() << " agree to a draw!\n";
                        return true;
                    }
                    else
                    {
                        std::cout << opponent.getName() << " reject the draw! Play on!\n";
                        continue;
                    }
                default: break;
            }
            
            board.movePiece(player, playerTurnInput.movement);
            Game::updatePlayerPieces(opponent);

            board.drawBoard();

            Game::Result turnResult { 
                Game::detectGameEnd(player, opponent, board) 
            };

            if(turnResult == Game::Result::checkmate)
            {
                std::cout << "Checkmate! " << player.getName() << " wins!\n";
                return true;
            }
            else if(turnResult == Game::Result::stalemate)
            {
                std::cout << "Stalemate! Draw!";
                return true;
            }
            return false;
        }
    }
    bool acceptDraw()
    {
        std::cout << "Accept? [y/n]: ";

        while(true)
        {
            char input{};
            std::cin >> input;

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

            if(std::cin.gcount() > 1)
            {
                std::cout << "Invalid, try again: ";
                continue;
            }

            switch(input)
            {
                case 'y':
                case 'Y':
                    return true;
                case 'n':
                case 'N':
                    return false;
                default:
                    std::cout << "Invalid, try again: ";
                    continue;
            }
        }
        
    }
}; 
