#include "pch.h"
#include "SceneMgr.h"
#include "Scene_Start.h"
#include "Scene_Cave.h"
#include "EventMgr.h"
#include "World.h"
#include "WorldGenerator.h"
#include "EnumString.h"

SceneMgr::SceneMgr() : m_arrScene{}, m_pCurScene{} {};

SceneMgr::~SceneMgr() 
{
	for (UINT i = 0; i < (UINT)SceneType::END; ++i)
	{
		if (m_arrScene[i] != nullptr)
		{
			delete m_arrScene[i];
		}
	}
};
void SceneMgr::init()
{
	m_arrScene[(UINT)SceneType::START] = new Scene_Start;
	m_arrScene[(UINT)SceneType::START]->SetName(L"Start Scene");
	m_arrScene[(UINT)SceneType::CAVE] = new Scene_Cave;
	m_arrScene[(UINT)SceneType::CAVE]->SetName(L"Cave Scene");

	// 시작 씬 지정
	m_pCurScene = m_arrScene[(UINT)SceneType::START];

	if (m_pCurScene->GetWorld() == nullptr)
	{
		int tryCount = 50;
		World* wld = nullptr;
		if (WorldGenerator::tryGenerateWorld(WorldType::Forest, tryCount, wld) == false)
		{
			MessageBox(nullptr, L"월드 생성 실패!", L"Error", MB_OK);
			return;
		}
		m_pCurScene->SetWorld(wld);
	}
	m_pCurScene->Enter();
}

void SceneMgr::update()
{
	m_pCurScene->update();

	m_pCurScene->finalupdate();
}

void SceneMgr::render(HDC hdc)
{
	m_pCurScene->render(hdc);
}

void SceneMgr::ChangeScene(SceneType next)
{
	if (m_arrScene[(UINT)next]->GetWorld() == nullptr)
	{
		int tryCount = 50;
		World* wld = nullptr;
		if (WorldGenerator::tryGenerateWorld(next == SceneType::CAVE ? WorldType::Cave : WorldType::Forest, tryCount, wld) == false)
		{
			MessageBox(nullptr, L"월드 생성 실패!, 기존 월드로 돌아갑니다.", L"Error", MB_OK);
			return;
		}
		m_arrScene[(UINT)next]->SetWorld(wld);
	}
	m_pCurScene->Exit();
	m_pCurScene = m_arrScene[(UINT)next];
	m_pCurScene->Enter();
}
