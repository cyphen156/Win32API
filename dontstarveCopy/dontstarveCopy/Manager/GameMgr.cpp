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
}

void GameMgr::update()
{
	if (isPaused)
	{
		return;
	}
	// 씬 업데이트
	SceneMgr::GetInst()->update();
	// 충돌체 업데이트
	CollisionMgr::GetInst()->update();

	CameraMgr::GetInst()->Update();
}

void GameMgr::render(HDC hdc)
{
	if (!isPaused)
	{
		// 씬 렌더링
		SceneMgr::GetInst()->render(hdc);
	}
}
