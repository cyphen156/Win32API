#include "pch.h"
#include "Terrain.h"
#include "Tile.h"
#include "Texture.h"

// Terrain 생성자
Terrain::Terrain(BiomeType biomeType, TileType tileType, Vec2D position) 
    : biomeType(biomeType),
    tileType(tileType),
    tileIndex(0),
	position(position)
{
}

// Terrain 소멸자
Terrain::~Terrain() 
{
}

void Terrain::setBiomeType(BiomeType newType)
{
    biomeType = newType;
}

void Terrain::setTileType(TileType newTile)
{
    tileType = newTile;
}

BiomeType Terrain::getBiomeType() const
{
    return biomeType;
}

TileType Terrain::getTileType() const
{
    return tileType;
}

// 지형 렌더링
void Terrain::render(HDC hdc)
{
    Texture* texture = tileDefs[(UINT)tileType].tiles[tileIndex].texture;
    StretchBlt(hdc, (int)position.x, (int)position.y, 1, 1, texture->GetDC(), 0, 0, texture->Width(), texture->Height(), SRCCOPY);
}