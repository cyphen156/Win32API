#include "pch.h"
#include "Object.h"

// ������ ����
Object::Object(int initX, int initY, int initHealth) {
    pt.x = initX;
    pt.y = initY;
    hp = initHealth;
}

// update �Լ� ����
void Object::update() {
}

// render �Լ� ����
void Object::render(HDC hdc) {
}

// �Ҹ��� ����
Object::~Object() {}
