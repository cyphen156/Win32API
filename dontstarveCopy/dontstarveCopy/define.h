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

#define KEY_CHECK(key, state)	KeyMgr::GetInst()->GetKeyState(key) == state
#define KEY_PRESS(key)			KEY_CHECK(key, KEY_STATE::PRESS)
#define KEY_DOWN(key)			KEY_CHECK(key, KEY_STATE::KEYDOWN)
#define KEY_UP(key)				KEY_CHECK(key, KEY_STATE::KEYUP)
#define KEY_NONE(key)			KEY_CHECK(key, KEY_STATE::NONE)

#define CLONE(type) type* Clone {return new type(*this);}

#define WSREGULAR 500
#define WSLARGE 1000


enum class SceneType
{
	START,
	CAVE,

	END
};

enum class ObjType
{
	WORLD,
	UI,
	Player,
	Monster,

	END
};

enum class InstType
{
	ENVIRONMENT,
	PLAYER,
	MONSTER,
	ITEM,

	END
};

enum class MONSTER
{
	Spider,
	Pig,

	END
};

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
	Rock,

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
	GREEM,
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
	ATTACK,
	HIT,

	END,
};