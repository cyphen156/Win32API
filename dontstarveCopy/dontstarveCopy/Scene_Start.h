#pragma once
#include "Scene.h"

class Scene_Start :
    public Scene
{
public:
    Scene_Start();
    ~Scene_Start();

    virtual void update();
    virtual void render(HDC hdc);
    virtual void Enter();
    virtual void Exit();
};

