#include "pch.h"
#include "Scene_Start.h"
#include "Object.h"
#include "Player.h"
#include "Monster.h"
#include "Pigman.h"
#include "Spider.h"
#include "Core.h"
#include "PathMgr.h"
#include "ResourceMgr.h"
#include "CollisionMgr.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "World.h"
#include <cmath>
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

	// 플레이어 주변에 Pigman과 Spider를 5마리씩 고리 모양으로 배치 (시험 배치)
	float fMonsterMaxDist = 50.f;

	for (int i = 0; i < 5; ++i)
	{
		float pigmanAngle = 72.f * i * 3.14159265f / 180.f;
		Vec2D pigmanPos = charWorldPos + Vec2D(cosf(pigmanAngle) * 300.f, sinf(pigmanAngle) * 300.f);
		Monster* pigman = new Pigman;
		pigman->SetCenterPos(pigmanPos);
		pigman->SetPos(pigmanPos);
		pigman->SetMaxMoveDistance(fMonsterMaxDist);
		AddObj(pigman, ObjType::Monster);

		float spiderAngle = (72.f * i + 36.f) * 3.14159265f / 180.f;
		Vec2D spiderPos = charWorldPos + Vec2D(cosf(spiderAngle) * 450.f, sinf(spiderAngle) * 450.f);
		Monster* spider = new Spider;
		spider->SetCenterPos(spiderPos);
		spider->SetPos(spiderPos);
		spider->SetMaxMoveDistance(fMonsterMaxDist);
		AddObj(spider, ObjType::Monster);
	}

#ifdef _DEBUG
	// 부하 확인용: 플레이어 주변 격자(간격 250)에 Pigman/Spider를 번갈아 깐다
	const int debugSpawnCount = 500;
	int columns = (int)ceilf(sqrtf((float)debugSpawnCount));
	float spacing = 250.f;
	for (int k = 0; k < debugSpawnCount; ++k)
	{
		int col = k % columns, row = k / columns;
		Vec2D gridPos = charWorldPos + Vec2D((col - columns / 2) * spacing + 125.f, (row - columns / 2) * spacing + 125.f);
		Monster* monster = (k % 2 == 0) ? (Monster*)new Pigman : (Monster*)new Spider;
		monster->SetCenterPos(gridPos);
		monster->SetPos(gridPos);
		monster->SetMaxMoveDistance(fMonsterMaxDist);
		AddObj(monster, ObjType::Monster);
	}
#endif
	
	CollisionMgr::GetInst()->CheckObjType(ObjType::Player, ObjType::Monster);
}
void Scene_Start::Exit()
{
	DeleteAll();

	CollisionMgr::GetInst()->Reset();
}
