#pragma once

#include <windows.h>
#include <string>
#include <iostream>

class Object {
protected:
    POINT pt;
    int hp;

    // 持失切 protected 識情
    Object(int initX, int initY, int initHealth);

public:
    virtual void update();
    virtual void render(HDC hdc);
    virtual ~Object();
};
