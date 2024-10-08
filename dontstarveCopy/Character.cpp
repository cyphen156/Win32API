#pragma once

#include "Character.h"
#include <windows.h>

void Character::loadBitmap() {

}

Character::Character(int initX, int initY, int initHealth, std::string initName, int initSpeed)
    : Object(initX, initY, initHealth), name(initName), speed(initSpeed) {
    // 리소스 로드
    loadBitmap();
}

// update 함수
void Character::update() {
    pt.x += speed;
}

// render 함수
void Character::render(HDC hdc) {
}

// 소멸자: 비트맵 해제
Character::~Character() {
}