#pragma once
#include "Struct.h"
#include  "Collider.h"

/**
* includes 
* Position
* Scale
*/

class Collider;

class Object 
{
private:
    wstring m_strName;
    Vec2D m_Pos;
    Vec2D m_objScale;

    Collider* m_pCollider;
    bool m_bisDead;
    void SetDead() { m_bisDead = false; } 

public:
    Object();
    virtual ~Object();

    void CreateCollider();

    void SetPos(Vec2D pos) { m_Pos = pos; };
    void SetScale(Vec2D scale) { m_objScale = scale; };
    bool isDead() { return !m_bisDead; }
    Vec2D GetPos() { return m_Pos; };
    Vec2D GetScale() { return m_objScale; };
    wstring GetObjName() { return m_strName; }
    Collider* GetCollider() { return m_pCollider; }

    virtual void update() = 0;
    virtual void finalupdate() final;
    virtual void render(HDC hdc);

    void componentRender(HDC hdc);

    virtual void OnCollision(Collider* pOther) {};
    virtual void OnCollisionEnter(Collider* pOther) {};
    virtual void OnCollisionExit(Collider* pOther) {};

    friend class EventMgr;
};
