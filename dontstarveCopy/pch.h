#pragma once

#include <iostream>
#include <Windows.h>
#include "ctype.h"
#include <string>

// ������ ������Ʈ Ÿ��

// ���̿� Ÿ��
enum class BiomeType {
    Spawn,
    Forest,
    Savannah,
    Beechnut,
    Swamp,
    Desert,
    Grass,
    Rock
};/*
enum class TerrainType {
    Ground,
    Water
};*/
// ������Ʈ �ν��Ͻ�
enum class ObjectInstanceType {
    Player,
    Monster,
    Environment,
    Item
};