#include "pch.h"
#include "Scene_Start.h"
#include "Object.h"
#include "Player.h"
#include "Monster.h"
#include "Core.h"
#include "PathMgr.h"
#include "ResMgr.h"
#include "CollisionMgr.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "World.h"

Scene_Start::Scene_Start()
{
}

Scene_Start::~Scene_Start()
{
}

void Scene_Start::update()
{
	Scene::update();

	/*if (KEY_DOWN(KEY::ENTER))
	{
		ChangeScene(SceneType::CAVE);
	}*/
}

void Scene_Start::render(HDC hdc)
{
	Scene::render(hdc);
}

void Scene_Start::Enter()
{
	BOOL WLDSIZE = false;
	
	// ���� ����, �ʱ�ȭ
	World wld(SceneType::START);
	wstring wldName = to_wstring(SceneType::START);
	wld.setWldName(wldName);
	wld.SetWorldSize(WLDSIZE);
	wld.generateWorld();
	Vec2D charWorldPos = wld.getWorldSize() / Vec2D(2, 2); // ĳ���� �⺻ ��ǥ : ���� ���߾�
	//wld.generateWorld(&charWorldPos);

	// �÷��̾� �߰�
	Object* pObj = new Player;
	pObj->SetPos(charWorldPos);
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
	DeleteAll();

	CollisionMgr::GetInst()->Reset();
}
