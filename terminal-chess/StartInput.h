#ifndef STARTINPUT_H
#define STARTINPUT_H

#include "Position.h"
#include "TurnInput.h"

struct StartInput
{
    Position position{};
    TurnInput::Action action{};
};

#endif