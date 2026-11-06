#pragma once
#include "Object.h"
#include "Texture.h"
#include "StateMachine.h"

class Texture;
class MonsterController;

enum class MonsterState
{
    None,
    Alive,
    Dead,

    END
};

enum class MonsterBehavior
{
    Idle,
    Move,

    END
};

class Monster : public Object
{
private:
    float   m_fspeed;

    Vec2D   m_vCenterPos;
    float   m_fMaxDist;

    MonsterType m_MonsterType;
    StateMachine<MonsterState> stateMachine;
    MonsterController* controller;

public:
    Monster();
    Monster(const Monster& origin);
    ~Monster();

    virtual void GenerateAnimations(wstring& TexPath);
    virtual void GetAnimations(vector<Texture*> frames, wstring& animationName, wstring& TexPath);
    virtual void update();

    float GetSpeed() { return m_fspeed; }

    void SetCenterPos(Vec2D centerPos) { m_vCenterPos = centerPos; }
    Vec2D GetCenterPos() { return m_vCenterPos; }

    void SetMaxMoveDistance(float distance) { m_fMaxDist = distance; }
    float GetMaxMoveDistance() { return m_fMaxDist; }

    MonsterType GetMonsterType() { return m_MonsterType; }
    void SetMonsterType(MonsterType type) { m_MonsterType = type; }

    MonsterState GetState() { return stateMachine.GetState(); }
};