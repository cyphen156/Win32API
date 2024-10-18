#include "pch.h"
#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	for (UINT i = 0; i < (UINT)ObjType::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			delete m_arrObj[i][j];
		}
	}
}

void Scene::update()
{
	for (UINT i = 0; i < (UINT)ObjType::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			if (m_arrObj[i][j]->isDead())
			{
				m_arrObj[i][j]->update();
			}
		}
	}
}

void Scene::finalupdate()
{
	for (UINT i = 0; i < (UINT)ObjType::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			m_arrObj[i][j]->finalupdate();
		}
	}
}

void Scene::render(HDC hdc)
{
	for (UINT i = 0; i < (UINT)ObjType::END; ++i)
	{
		vector<Object*>::iterator iter = m_arrObj[i].begin();

		for (; iter != m_arrObj[i].end();)
		{
			if (!(*iter)->isDead())
			{
				(*iter)->render(hdc);
				++iter;
			}
			else 
			{
				iter = m_arrObj[i].erase(iter);
			}
		}
	}
}

void Scene::Enter()
{
}

void Scene::Exit()
{
}
