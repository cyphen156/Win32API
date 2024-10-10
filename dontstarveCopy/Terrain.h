#pragma once
#include <Windows.h>
#include "pch.h"

class Terrain {
protected:
    POINT position;
    int tileSize;
    HBITMAP texture;
    BiomeType type;

public:
    Terrain(int x, int y, int tileSize, BiomeType type);
    ~Terrain();

    void render(HDC hdc);
};
