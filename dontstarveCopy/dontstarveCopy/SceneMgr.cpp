#include "pch.h"
#include "SceneMgr.h"
#include "Scene_Start.h"

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

	m_pCurScene = m_arrScene[(UINT)SceneType::START];
	m_pCurScene->Enter();
}

void SceneMgr::update()
{
	m_pCurScene->update();
}

void SceneMgr::render(HDC hdc)
{
	m_pCurScene->render(hdc);
}
