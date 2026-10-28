#pragma once

#include "Terrain.h"
#include "World.h"

class WorldGenerator
{
private:
    static bool tryGenerateWorld(WorldType worldType, int tryCount, World*& world);

    static int calcDist(const POINT& a, const POINT& b);
    static vector<pair<int, int>> generateMST(const vector<POINT>& biomeCenters);
    static void bfsExpandBiome(const POINT& center, int& biomeTiles, BiomeType biome, vector<vector<Terrain>>& worldMap, int worldWidth, int worldHeight);

    friend class SceneMgr;
};
