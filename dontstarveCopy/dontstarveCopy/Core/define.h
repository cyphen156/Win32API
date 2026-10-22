#pragma once

#define NOMINMAX

#define SINGLE(type);	public:\
							static type* GetInst()\
							{\
								static type mgr;\
								return &mgr;\
							}\
						private:\
							type();\
							~type();

#define	fDT	TimeMgr::GetInst()->GetfDeltaTime();
#define	DT	TimeMgr::GetInst()->GetDeltaTime();

#define KEY_CHECK(key, state)	InputMgr::GetInst()->GetKeyState(key) == state
#define KEY_PRESS(key)			KEY_CHECK(key, KEY_STATE::PRESS)
#define KEY_DOWN(key)			KEY_CHECK(key, KEY_STATE::KEYDOWN)
#define KEY_UP(key)				KEY_CHECK(key, KEY_STATE::KEYUP)
#define KEY_NONE(key)			KEY_CHECK(key, KEY_STATE::NONE)

#define CLONE(type) type* Clone {return new type(*this);}

#define WSREGULAR 500
#define WSLARGE 1000

#define WORLD_TRY_COUNT 10

enum class SceneType
{
	START,
	CAVE,

	END
};

enum class ObjType
{
	Environment,
	Player,
	Monster,
	Component,

	END
};

// 열거형 오브젝트 타입
// 바이옴 타입

enum class BiomeType {
	Water,
	Spawn,
	Forest,
	Savanna,
	Deciduous,
	Marsh,
	Desert,
	Grass,
	Rocky,

	END
};

// 타일 타입

enum class TileType {
	OceanCoastal,
	OceanSwell,
	OceanRough,
	OceanHazardous,
	Marsh,
	Rocky,
	Savanna,
	Forest,
	Grass,
	Dirt,
	Deciduous,
	DesertDirt,

	END
};

enum class BRUSH_TYPE
{
	HOLLOW,

	END
};

enum class PEN_TYPE
{
	RED,
	GREEN,
	BLUE,
	BLACK,
	WHITE,

	END
};

enum class EVENT_TYPE
{
	CREATE_OBJ,
	DELETE_OBJ,
	CHANGE_SCENE,
	REGENERATE_WORLD,
	ATTACK,
	HIT,

	END,
};