#pragma once
#include "Object.h"
#include "ResTex.h"

class ResTex;

class Player :
    public Object 
{
private:
    float m_fspeed;
    ResTex* m_pTex;

public:
    Player();
    ~Player();

    virtual void update();
    virtual void render(HDC hdc);
};

