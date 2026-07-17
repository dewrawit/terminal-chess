#ifndef TURNINPUT_H
#define TURNINPUT_H

#include "Movement.h"

struct TurnInput
{
    enum class Action { move, resign, offerDraw };
    Action action { Action::move };
    Movement movement {};

    TurnInput(const Action& act) :  action{ act } {}
    TurnInput(const Movement& mov) : action{ Action::move }, movement{ mov } {} 
};

#endif