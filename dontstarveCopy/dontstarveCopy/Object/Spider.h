#pragma once
#include "Monster.h"

class Spider : public Monster
{
public:
    Spider();
    ~Spider();

    Spider* Clone() { return new Spider(*this); }
};