#ifndef MOVEDATA_H
#define MOVEDATA_H

#include "Position.h"

struct MoveData
{
    enum class Type { normal, castlingQ, castlingK, enpassant, promoteQ, promoteN, promoteB, promoteR };
    Position position{};
    Type type{ Type::normal };
};

#endif