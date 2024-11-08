#pragma once
#include "Scene.h"

class Scene_Cave :
    public Scene
{
public:
    Scene_Cave();
    ~Scene_Cave();

    virtual void Enter();
    virtual void Exit();
};
