#pragma once
#include "Object.h"
#include <windows.h>
#include <string>

class Character : public Object {
private:
    std::string name;
    int speed;
    HBITMAP hBitmap;

    void loadBitmap();
public:
    Character(int initX, int initY, int initHealth, std::string initName, int initSpeed);
    void update() override;
    void render(HDC hdc) override;
    ~Character();
};
