#pragma once
#include "Struct.h"

/**
* includes 
* Position
* Scale
*/
class Object 
{
private:
    Vec2D m_Pos;
    Vec2D m_objScale;
public:
    Object();
    virtual ~Object();

    void SetPos(Vec2D pos) { m_Pos = pos; };
    void SetScale(Vec2D scale) { m_objScale = scale; };

    Vec2D GetPos() { return m_Pos; };
    Vec2D GetScale() { return m_objScale; };

    virtual void update() = 0;
    void render(HDC hdc);
};
