#pragma once

#include <iostream>
#include <Windows.h>
#include "ctype.h"
#include <string>
#include "define.h"

using namespace std;

// ������ ������Ʈ Ÿ��
// ���̿� Ÿ��

enum class BiomeType {
    Water,
    Spawn,
    Forest,
    Savannah,
    Beechnut,
    Swamp,
    Desert,
    Grass,
    Rock
};

// ������Ʈ �ν��Ͻ�
enum class ObjectInstanceType {
    Player,
    Monster,
    Environment,
    Item
};