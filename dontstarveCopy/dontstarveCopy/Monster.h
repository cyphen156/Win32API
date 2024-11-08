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
    Monster(const Monster& origin);
    ~Monster();

    virtual void OnCollisionEnter(Collider* pOther);
    virtual void GetAnimations(wstring& TexPath);
    virtual void update();
    virtual void render(HDC hdc);

    Vec2D GetCenterPos() { return m_vCenterPos; }
    void SetCenterPos(Vec2D centerPos) { m_vCenterPos = centerPos; }

    float GetSpeed() { return m_fspeed; }
    void SetSpeed(float speed) { m_fspeed = speed; }

    void SetMaxMoveDistance(float distance) { m_fMaxDist = distance; }

    Monster* Clone() { return new Monster(*this); }
};

