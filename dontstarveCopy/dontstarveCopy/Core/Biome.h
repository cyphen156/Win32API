#pragma once

// 바이옴 정의. 바이옴이 쓰는 타일, 첫 번째가 배경 타일
struct Biome
{
	BiomeType type;
	vector<TileType> tiles;
};

// BiomeType 순서
extern const Biome biomeDefs[(UINT)BiomeType::END];
