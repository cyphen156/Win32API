#pragma once
#include "Object.h"
#include "Texture.h"

class Texture;

class Monster : public Object
{
private:
    float   m_fspeed;
    bool    bisRight;

    Vec2D   m_vCenterPos;
    float   m_fMaxDist;

    MonsterType m_MonsterType;

public:
    Monster();
    ~Monster();

    virtual void GenerateAnimations(wstring& TexPath);
    virtual void GetAnimations(vector<Texture*> frames, wstring& animationName, wstring& TexPath);
    virtual void update();

    void SetCenterPos(Vec2D centerPos) { m_vCenterPos = centerPos; }
    void SetMaxMoveDistance(float distance) { m_fMaxDist = distance; }

    MonsterType GetMonsterType() { return m_MonsterType; }
    void SetMonsterType(MonsterType type) { m_MonsterType = type; }
};