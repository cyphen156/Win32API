#pragma once

#include <iostream>
#include <Windows.h>
#include "ctype.h"
#include <string>
#include "define.h"

using namespace std;

// 열거형 오브젝트 타입
// 바이옴 타입

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

// 오브젝트 인스턴스
enum class ObjectInstanceType {
    Player,
    Monster,
    Environment,
    Item
};