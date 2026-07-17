
#include <cassert>

#include "Player.h"
#include "Board.h"
#include "Game.h"

int main()
{
    Player whitePlayer {"White", Player::Side::white};
    Player blackPlayer {"Red", Player::Side::black}; //because I paint "black" pieces red
    //if it's black the terminal background will make piece invisible!

    Board chessboard { whitePlayer, blackPlayer };
    
    assert(whitePlayer.isWhite() && blackPlayer.isBlack() && "Incorrect side assignment");

    chessboard.drawBoard();

    while(true)
    {
        bool whiteGameEnd { Game::takeTurn(whitePlayer,blackPlayer,chessboard) };
        if(whiteGameEnd)
            break;

        bool blackGameEnd { Game::takeTurn(blackPlayer,whitePlayer,chessboard) };
        if(blackGameEnd)
            break;
    }
    return 0;
};
