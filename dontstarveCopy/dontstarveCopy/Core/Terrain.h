#pragma once

class Terrain 
{
protected:
    BiomeType type;
    TileType tile;

public:
    Terrain(BiomeType type, TileType tile);
    ~Terrain();

    void setBiomeType(BiomeType changetype);
    void setTileType(TileType changetile);

    BiomeType getBiomeType() const;
    TileType getTileType() const;

    void render(HDC hdc, const RECT& tileRect);
};
