#include "pch.h"
#include "Terrain.h"

// Terrain ������
Terrain::Terrain(BiomeType type) : type(type) 
{
}

// Terrain �Ҹ���
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

// ���� ������
void Terrain::render(HDC hdc) 
{
    
}
