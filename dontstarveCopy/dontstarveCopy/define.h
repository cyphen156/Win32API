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

enum class SceneType
{
	TOOL,
	START,

	END
};

enum class ObjType
{
	WORLD,
	UI,
	INSTANCE,
	INVENTORY,

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