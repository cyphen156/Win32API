#include "pch.h"
#include "Object.h"

// 생성자 구현
Object::Object(int initX, int initY, int initHealth) {
    pt.x = initX;
    pt.y = initY;
    hp = initHealth;
}

// update 함수 구현
void Object::update() {
}

// render 함수 구현
void Object::render(HDC hdc) {
}

// 소멸자 구현
Object::~Object() {}
