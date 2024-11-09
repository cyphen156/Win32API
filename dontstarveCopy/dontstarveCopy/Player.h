#pragma once
#include "Object.h"
#include "ResTex.h"

class ResTex;

class Player :
    public Object 
{
private:
    float   m_fspeed;
    bool    bisRight;
    //ResTex* m_pTex;   // 리소스 개별로딩때 써먹엇던거

public:
    Player();
    ~Player();

    virtual void GenerateAnimations(wstring& TexPath);
    virtual void GetAnimations(vector<ResTex*> frames, wstring& animationName, wstring& TexPath);
    //virtual void GetAnimations(wstring& TexPath);
    virtual void update();
    virtual void render(HDC hdc);

    Player* Clone() { return new Player(*this); }

};

