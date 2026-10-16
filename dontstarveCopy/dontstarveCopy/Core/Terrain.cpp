#include "pch.h"
#include "Terrain.h"

// Terrain 생성자
Terrain::Terrain(BiomeType type) : type(type) 
{
}

// Terrain 소멸자
Terrain::~Terrain() 
{
}

void Terrain::update(BiomeType newType) 
{
    type = newType;
}

BiomeType Terrain::getBiomeType() const
{
    return type;
}

// 지형 렌더링
void Terrain::render(HDC hdc) 
{
    
}
