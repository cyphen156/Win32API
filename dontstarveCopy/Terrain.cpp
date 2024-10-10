#include "Terrain.h"

// Terrain ������
Terrain::Terrain(int x, int y, int tileSize, BiomeType type)
    : position({ x, y }), tileSize(tileSize), type(type)) {
}

// Terrain �Ҹ���
Terrain::~Terrain() {
    if (texture) {
        DeleteObject(texture);
    }
}



// ���� ������
void Terrain::render(HDC hdc) {
    
}
