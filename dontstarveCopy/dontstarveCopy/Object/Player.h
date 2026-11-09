#pragma once
#include "Object.h"
#include "Texture.h"
#include "StateMachine.h"

#include "Interface/IHittable.h"

class Texture;
class PlayerController;

enum class PlayerState
{
    None,
    Alive,
    Invincible,
    Dead,

    END
};

enum class PlayerBehavior
{
    Idle,
    Move,
    Punch,

    END
};

class Player :
    public Object, public IHittable
{
private:
    int hp;
    int atk;
    float   m_fspeed;
    float atkRange;
    float atkWidth;

    StateMachine<PlayerState> stateMachine;
    PlayerController* controller;

    float invincibleDuration;
    float invincibleTime;

    void TakeDamage(int amount);

public:
    Player();
    Player(const Player& origin);
    ~Player();

    virtual void GenerateAnimations(wstring& TexPath);
    virtual void GetAnimations(vector<Texture*> frames, wstring& animationName, wstring& TexPath);
    virtual void update();
    virtual void render(HDC hdc);

    void OnHit(int damage) override;

    Player* Clone() { return new Player(*this); }

    float GetSpeed() { return m_fspeed; }

    PlayerState GetState() { return stateMachine.GetState(); }

    int GetHp() { return hp; }
    int GetAtk() { return atk; }
    float GetAtkRange() { return atkRange; }
    float GetAtkWidth() { return atkWidth; }
};