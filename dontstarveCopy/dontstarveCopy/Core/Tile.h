#pragma once

#define TILEDEFCOUNT 19

class Texture;

// 타일 조각 한 장
struct Tile
{
	Texture* texture;
};

// 타일 타입 하나의 조각 세트
struct TileDef
{
	TileType type;
	wstring setName;
	int tileCount;
	Tile tiles[48];
};

// 앞 12행은 TileType 순서의 Default, 뒤는 변형
extern TileDef tileDefs[TILEDEFCOUNT];