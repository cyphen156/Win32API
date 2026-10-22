#pragma once

#include "Vec2D.h"
#include "Terrain.h"
#include "PathMgr.h"
#include "ResourceMgr.h"
#include "Texture.h"

class Texture;

class World {
private:
    wstring worldName;
    Vec2D wldSize;
    vector<Texture*> tiles;
    vector<vector<Terrain>> worldMap;
    int tileSize;

public:
    World(SceneType Type);
    ~World();

    // 월드 크기 지정
    
    void setWldName(wstring type) { worldName = type; }
    void SetWorldSize(bool isLarge);
    void setTileSize(int size) { tileSize = size; }

    void generateWorld();

    void Render(HDC hdc);

    wstring getWorldName() { return worldName; }
    Vec2D getWorldSize() { return wldSize; }
    int getTileSize() { return tileSize; }
    vector<vector<Terrain>>& getWorldMap() { return worldMap; }
};
