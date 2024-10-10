#include "Terrain.h"

// Terrain 생성자
Terrain::Terrain(int x, int y, int tileSize, BiomeType type)
    : position({ x, y }), tileSize(tileSize), type(type)) {
}

// Terrain 소멸자
Terrain::~Terrain() {
    if (texture) {
        DeleteObject(texture);
    }
}



// 지형 렌더링
void Terrain::render(HDC hdc) {
    
}
