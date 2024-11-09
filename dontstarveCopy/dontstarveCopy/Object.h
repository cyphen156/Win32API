#pragma once
#include "Struct.h"
#include "Collider.h"
#include "Animator.h"
#include "func.h"
/**
* includes 
* Position
* Scale
*/

class Collider;
class Animator;

class Object 
{
private:
    wstring m_ObjName;
    Vec2D m_Pos;
    Vec2D m_objScale;
    bool m_bisDead;
    void SetDead() { m_bisDead = true; } 

    Collider* m_pCollider;
    Animator* m_pAnimator;

    wstring m_direction;
public:
    Object();
    Object(const Object& origin);
    virtual ~Object();

    void CreateCollider();
    void CreateAnimator();

    void SetPos(Vec2D pos) { m_Pos = pos; };
    void SetScale(Vec2D scale) { m_objScale = scale; };
    bool isDead() { return m_bisDead; }
    Vec2D GetPos() { return m_Pos; };
    Vec2D GetScale() { return m_objScale; };
    wstring GetObjName() { return m_ObjName; }
    Collider* GetCollider() { return m_pCollider; }
    Animator* GetAnimator() { return m_pAnimator;  }

    void SetCurrDir(wstring dir) { m_direction = dir; }
    wstring GetCurrDir() const { return m_direction; }
    /*void SetKeyState(KEY_STATE state) { m_currentKeyState = state; }
    KEY_STATE GetKeyState() const { return m_currentKeyState; }*/

    virtual void GenerateAnimations(wstring& TexPath) = 0;
    virtual void GetAnimations(vector<ResTex*> frames, wstring& animationName, wstring& TexPath) = 0;
    //virtual void GetAnimations(wstring& TexPath) = 0;
    virtual void update() = 0;
    virtual void finalupdate() final;
    virtual void render(HDC hdc);

    void componentRender(HDC hdc);

    virtual void OnCollision(Collider* pOther) {};
    virtual void OnCollisionEnter(Collider* pOther) {};
    virtual void OnCollisionExit(Collider* pOther) {};

    // CLONE(OBJECT);
    virtual Object* Clone() = 0;
    friend class EventMgr;
};
