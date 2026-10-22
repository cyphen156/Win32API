#include "pch.h"
#include "GameMgr.h"
#include "SceneMgr.h"
#include "CameraMgr.h"
#include "CollisionMgr.h"
#include "UIMgr.h"

GameMgr::GameMgr()
	: isPaused(false)
{
}

GameMgr::~GameMgr()
{
}

void GameMgr::init()
{
	SceneMgr::GetInst()->init();
	UIMgr::GetInst()->init();
}

void GameMgr::update()
{
	UIMgr::GetInst()->update();

	if (isPaused)
	{
		return;
	}

	// 씬 업데이트
	SceneMgr::GetInst()->update();
	CameraMgr::GetInst()->Update();
	// 충돌체 업데이트
	CollisionMgr::GetInst()->update();
}

void GameMgr::render(HDC hdc)
{
	if (!isPaused)
	{
		// 씬 렌더링
		SceneMgr::GetInst()->render(hdc);
	}
	UIMgr::GetInst()->render(hdc);
}
