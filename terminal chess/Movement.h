#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "Position.h" //also has MoveData
#include "MoveData.h"

struct Movement
{
    Position source{};
    Position dest{};
    MoveData::Type type { MoveData::Type::normal };  
};

#endif