#include "pch.h"
#include "Object.h"

Object::Object(int initX, int initY, int initHealth) {
    pt.x = initX;
    pt.y = initY;
    hp = initHealth;
}

void Object::update() {
}

void Object::render(HDC hdc) {
}

Object::~Object() {}
