#pragma once
#include "Transform.h"

class Terrain 
{
protected:
    BiomeType biomeType;
    TileType tileType;
    int tileIndex;
	Vec2D position;

public:
    Terrain(BiomeType biomeType, TileType tileType, Vec2D position);
    ~Terrain();

    void setBiomeType(BiomeType changetype);
    void setTileType(TileType changetile);

    BiomeType getBiomeType() const;
    TileType getTileType() const;

    void render(HDC hdc);
};
