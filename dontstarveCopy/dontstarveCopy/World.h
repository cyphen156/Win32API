#pragma once

#include <vector>
#include "Terrain.h"

class World {
private:
    int worldWidth;
    int worldHeight;
    vector<vector<Terrain>> worldMap;
    int tileSize;

public:
    World(RECT* wlSize);
    ~World();

    void generateWorld(POINT* wldCenterPos);
};
