#pragma once
#include "pch.h"
#include <vector>
#include "Terrain.h"

class World {
private:
    Terrain** terrains;
    int worldWidth;
    int worldHeight;
    int tileSize;

public:
    World(RECT* wlSize, int tileSize);
    ~World();

    void generateWorld(POINT* wldCenterPos);
};
