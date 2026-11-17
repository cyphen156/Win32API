#pragma once

#define TILEDEFCOUNT 26

class Texture;

struct Tile
{
	Texture* texture;
};

struct TileDef
{
	TileType type;
	wstring setName;
	int tileCount;
	Tile tiles[48];
};

extern TileDef tileDefs[TILEDEFCOUNT];