#include "pch.h"
#include "Scene_Start.h"
#include "Object.h"
#include "Player.h"
#include "Monster.h"
#include "Core.h"
#include "PathMgr.h"
#include "ResMgr.h"
#include "CollisionMgr.h"

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

void Scene_Start::Enter()
{
	// �÷��̾� �߰�
	Object* pObj = new Player;

	pObj->SetPos(Vec2D{ 640.f, 384.f });
	pObj->SetScale(Vec2D{100.f, 100.f});

	AddObj(pObj, ObjType::Player);


	// ���� �׷� �߰�
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

	// �浹 ������Ʈ ����
	CollisionMgr::GetInst()->CheckObjType(ObjType::Player, ObjType::Monster);
}

void Scene_Start::Exit()
{
	CollisionMgr::GetInst()->Reset();
}
