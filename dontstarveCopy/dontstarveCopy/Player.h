#pragma once
#include "Object.h"

class Player :
    public Object 
{
private:
    float m_fspeed;

public:
    Player();
    ~Player();

    virtual void update();
    void render();
};

