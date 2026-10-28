#include "pch.h"
#include "Scene_Start.h"
#include "Object.h"
#include "Player.h"
#include "Monster.h"
#include "Core.h"
#include "PathMgr.h"
#include "ResourceMgr.h"
#include "CollisionMgr.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "World.h"
#include "EnumString.h"
#include "CameraMgr.h"

Scene_Start::Scene_Start()
{
}

Scene_Start::~Scene_Start()
{
}

void Scene_Start::update()
{
	Scene::update();
}

void Scene_Start::render(HDC hdc)
{
	Scene::render(hdc);
}

void Scene_Start::Enter()
{
	wstring defaultPath = PathMgr::GetInst()->GetContentPath();

	Vec2D charWorldPos = GetWorld()->getSpawnPos();

	// 플레이어 추가
	wstring PlayerPath = defaultPath;
	PlayerPath += L"Textures\\Character\\Wilson";
	Object* pObj = new Player;
	pObj->SetPos(charWorldPos);
	AddObj(pObj, ObjType::Player);
	CameraMgr::GetInst()->SetTarget(pObj);

	int iMonsterCnt = 0;
	float fMonsterMaxDist = 50.f;
	float fMonsterScale = 50.f;

	Monster* pMonsterObj = nullptr;

	for (int i = 0; i < iMonsterCnt; ++i)
	{
		pMonsterObj = new Monster;
		pMonsterObj->SetCenterPos(Vec2D(fMonsterMaxDist + fMonsterScale, 50.f));
		pMonsterObj->SetPos(Vec2D(pMonsterObj->GetCenterPos()));

		pMonsterObj->SetMaxMoveDistance(fMonsterMaxDist);
		pMonsterObj->SetScale(Vec2D(fMonsterScale, fMonsterScale));
		AddObj(pMonsterObj, ObjType::Monster);
	}

	CollisionMgr::GetInst()->CheckObjType(ObjType::Player, ObjType::Monster);
}

void Scene_Start::Exit()
{
	DeleteAll();

	CollisionMgr::GetInst()->Reset();
}
