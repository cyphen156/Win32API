#pragma once
#include "Object.h"

class Monster :
    public Object
{
private:
    Vec2D   m_vCenterPos;
    float   m_fspeed;
    float   m_fMaxDist;
    int     m_iDirection;

public:
    Monster();
    ~Monster();

    virtual void update();
    void render();

    Vec2D GetCenterPos() { return m_vCenterPos; }
    void SetCenterPos(Vec2D centerPos) { m_vCenterPos = centerPos; }

    float GetSpeed() { return m_fspeed; }
    void SetSpeed(float speed) { m_fspeed = speed; }

    void SetMaxMoveDistance(float distance) { m_fMaxDist = distance; }
};

