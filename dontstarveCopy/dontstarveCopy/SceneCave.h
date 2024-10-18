#pragma once
#include "Scene.h"

class SceneCave :
    public Scene
{
public:
    SceneCave();
    ~SceneCave();

    virtual void Enter();
    virtual void Exit();
};
