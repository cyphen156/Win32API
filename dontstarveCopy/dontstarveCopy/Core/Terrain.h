#pragma once

class Terrain 
{
protected:
    BiomeType type;

public:
    Terrain(BiomeType type);
    ~Terrain();

    void update(BiomeType changetype);

    BiomeType getBiomeType() const;

    void render(HDC hdc);
};
