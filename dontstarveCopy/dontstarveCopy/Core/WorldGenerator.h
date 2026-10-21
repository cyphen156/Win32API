#pragma once

#include "Terrain.h"

class WorldGenerator
{
public:
    bool generateWorld(vector<vector<Terrain>>& worldMap, unsigned int seed);

private:
    int calcDist(const POINT& a, const POINT& b);
    vector<pair<int, int>> generateMST(const vector<POINT>& biomeCenters);
    void bfsExpandBiome(const POINT& center, int& biomeTiles, BiomeType biome, vector<vector<Terrain>>& worldMap, int worldWidth, int worldHeight);
};
