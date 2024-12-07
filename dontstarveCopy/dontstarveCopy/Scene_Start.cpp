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
#include "func2.h"

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
	wstring defaultPath = PathMgr::GetInst()->GetContentPath();

	// ���� ����, �ʱ�ȭ
	World wld(SceneType::START);
	wstring wldName = func2::Enum_To_Wstring(SceneType::START);
	wld.setWldName(wldName);
	wld.SetWorldSize(WLDSIZE);
	wld.generateWorld();
	Vec2D charWorldPos = wld.getWorldSize() / Vec2D(2, 2); // ĳ���� �⺻ ��ǥ : ���� ���߾�
	//wld.generateWorld(&charWorldPos);

	// �÷��̾� �߰�
	wstring PlayerPath = defaultPath;
	PlayerPath += L"Textures\\Player";
	Object* pObj = new Player;
	pObj->SetPos(charWorldPos);
	AddObj(pObj, ObjType::Player);

	// ���� �׷� 

	/*WIN32_FIND_DATA findDir;
	HANDLE hFind = FindFirstFile((MonsterGroupPath + L"\\*").c_str(), &findDir);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		MessageBox(nullptr, (L"Cannot find Dir" + MonsterGroupPath + L" Error :: Invalid Path.").c_str(), L"Error", MB_OK);
		return;
	}
	bool isEmpty = true;*/

	/*while (1)
	{
		if ()
	}*/
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
