#include "pch.h"
#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	for (UINT i = 0; i < (UINT)SceneType::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			delete m_arrObj[i][j];
		}
	}
}

void Scene::update()
{
	for (UINT i = 0; i < (UINT)SceneType::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			m_arrObj[i][j]->update();
		}
	}
}

void Scene::render(HDC hdc)
{
	for (UINT i = 0; i < (UINT)SceneType::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			m_arrObj[i][j]->render(hdc);
		}
	}
}

void Scene::Enter()
{
}

void Scene::Exit()
{
}
