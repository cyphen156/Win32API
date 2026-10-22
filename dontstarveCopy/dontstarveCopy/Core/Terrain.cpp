#include "pch.h"
#include "Terrain.h"
#include "Texture.h"

// Terrain 생성자
Terrain::Terrain(BiomeType type, TileType tile) : type(type), tile(tile) 
{
}

// Terrain 소멸자
Terrain::~Terrain() 
{
}

void Terrain::setBiomeType(BiomeType newType)
{
    type = newType;
}

void Terrain::setTileType(TileType newTile)
{
    tile = newTile;
}

BiomeType Terrain::getBiomeType() const
{
    return type;
}

TileType Terrain::getTileType() const
{
    return tile;
}

// 지형 렌더링
void Terrain::render(HDC hdc, const RECT& tileRect, Texture* tileTexture)
{
    StretchBlt(hdc, tileRect.left, tileRect.top, tileRect.right - tileRect.left, tileRect.bottom - tileRect.top,
        tileTexture->GetDC(), 0, 0, tileTexture->Width(), tileTexture->Height(), SRCCOPY);
}
