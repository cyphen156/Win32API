#pragma once
#include "Object.h"
#include "Texture.h"

class Texture;

class Player :
    public Object 
{
private:
    float   m_fspeed;
    bool    bisRight;

public:
    Player();
    ~Player();

    virtual void GenerateAnimations(wstring& TexPath);
    virtual void GetAnimations(vector<Texture*> frames, wstring& animationName, wstring& TexPath);
    //virtual void GetAnimations(wstring& TexPath);
    virtual void update();
    virtual void render(HDC hdc);

    Player* Clone() { return new Player(*this); }

};

