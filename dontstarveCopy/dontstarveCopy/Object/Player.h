#pragma once
#include "Object.h"
#include "Texture.h"
#include "StateMachine.h"

class Texture;
class PlayerController;

enum class PlayerState
{
    None,
    Alive,
    Dead,

    END
};

enum class PlayerBehavior
{
    Idle,
    Move,

    END
};

class Player :
    public Object
{
private:
    float   m_fspeed;
    StateMachine<PlayerState> stateMachine;
    PlayerController* controller;

public:
    Player();
    Player(const Player& origin);
    ~Player();

    virtual void GenerateAnimations(wstring& TexPath);
    virtual void GetAnimations(vector<Texture*> frames, wstring& animationName, wstring& TexPath);
    virtual void update();
    virtual void render(HDC hdc);

    Player* Clone() { return new Player(*this); }

    float GetSpeed() { return m_fspeed; }

    PlayerState GetState() { return stateMachine.GetState(); }
};