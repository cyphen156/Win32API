#pragma once

class Texture;

// 타일 정의. 타일 세트(Terrain/지면 폴더/세트 폴더) 안에서 이 타일이 쓰는 조각 범위와 그릴 조각
struct Tile
{
	TileType type;
	wstring groundName;
	wstring setName;
	int firstIndex;
	int lastIndex;
	int drawIndex;
	Texture* texture;
	Texture* mapTexture;

	void render(HDC hdc, const RECT& tileRect);
	Texture* getMapTexture();
};

// TileType 순서
extern Tile tileDefs[(UINT)TileType::END];
