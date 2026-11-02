#pragma once
#include "Monster.h"

class Pigman : public Monster
{
public:
    Pigman();
    ~Pigman();

    Pigman* Clone() { return new Pigman(*this); }
};