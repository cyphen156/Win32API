#pragma once

#include "Struct.h"
#include "Terrain.h"
#include "PathMgr.h"
#include "ResMgr.h"
#include "ResTex.h"

class ResTex;

class World {
private:
    wstring worldName;
    Vec2D wldSize;
    vector<ResTex*> tiles;
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

    void LoadTile(wstring& TexPath);
    
    wstring getWorldName() { return worldName; }
    Vec2D getWorldSize() { return wldSize; }
    int getTileSize() { return tileSize; }
};
