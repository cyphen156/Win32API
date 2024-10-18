#include "pch.h"
#include "SceneMgr.h"
#include "Scene_Start.h"
#include "SceneCave.h"

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
	m_arrScene[(UINT)SceneType::CAVE] = new Scene_Start;
	m_arrScene[(UINT)SceneType::CAVE]->SetName(L"Cave Scene");

	// ½ÃÀÛ ¾À ÁöÁ¤
	m_pCurScene = m_arrScene[(UINT)SceneType::START];
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
	m_pCurScene = m_arrScene[(UINT)SceneType::next];
	m_pCurScene->Enter();
}
