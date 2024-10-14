#pragma once
#include "Struct.h"

class Object 
{
private:
    Vec2D m_Pos;
    Vec2D m_objScale;

public:
    Object();
    ~Object();

    void SetPos(Vec2D pos) { m_Pos = pos; };
    void SetScale(Vec2D scale) { m_objScale = scale; };

    Vec2D GetPos() { return m_Pos; };
    Vec2D GetScale() { return m_objScale; };
};
