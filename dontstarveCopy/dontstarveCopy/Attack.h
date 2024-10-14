#pragma once
#include "Object.h"

class Attack :
    public Object
{
private:
    float m_fDir;

public:
    Attack();
    ~Attack();

    void SetDir(bool dir);

    virtual void update();
    virtual void render(HDC hdc);
};

