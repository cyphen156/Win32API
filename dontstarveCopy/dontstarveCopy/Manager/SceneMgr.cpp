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
		GenerateWorld(SceneType::START, 0);
		return;
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
	m_pCurScene->Exit();
	m_pCurScene = m_arrScene[(UINT)next];

	if (m_pCurScene->GetWorld() == nullptr)
	{
		GenerateWorld(next, 0);
		return;
	}
	m_pCurScene->Enter();

}

void SceneMgr::GenerateWorld(SceneType type, int tryCount)
{
	BOOL WLDSIZE = false;

	// 월드 생성, 초기화
	World* wld = new World(type == SceneType::CAVE ? WorldType::Cave : WorldType::Forest);
	wstring wldName = EnumString::ToWString(type);
	wld->SetWorldSize(WLDSIZE);
	wld->generateWorld();

	LARGE_INTEGER seed = {};
	QueryPerformanceCounter(&seed);

	WorldGenerator generator;
	if (generator.generateWorld(wld->getTerrains(), wld->getWorldType(), (unsigned int)seed.QuadPart) == false)
	{
		delete wld;
		if (tryCount + 1 < WORLD_TRY_COUNT)
		{
			tEvent event = {};
			event.eEvent = EVENT_TYPE::REGENERATE_WORLD;
			event.lParam = (DWORD)type;
			event.wParam = (DWORD_PTR)(tryCount + 1);

			EventMgr::GetInst()->AddEvent(event);
		}
		else
		{
			MessageBox(nullptr, (L"World generation failed :: " + wldName).c_str(), L"Error", MB_OK);
		}
		return;
	}

	m_arrScene[(UINT)type]->SetWorld(wld);
	m_arrScene[(UINT)type]->Enter();
}
