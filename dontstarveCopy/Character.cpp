#pragma once

#include "Character.h"
#include <windows.h>

void Character::loadBitmap() {

}

Character::Character(int initX, int initY, int initHealth, std::string initName, int initSpeed)
    : Object(initX, initY, initHealth), name(initName), speed(initSpeed) {
    // ���ҽ� �ε�
    loadBitmap();
}

// update �Լ�
void Character::update() {
    pt.x += speed;
}

// render �Լ�
void Character::render(HDC hdc) {
}

// �Ҹ���: ��Ʈ�� ����
Character::~Character() {
}