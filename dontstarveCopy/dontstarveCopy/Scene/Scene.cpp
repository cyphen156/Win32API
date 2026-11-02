#include "pch.h"
#include "Scene.h"
#include "CameraMgr.h"
#include "World.h"

Scene::Scene()
	: world(nullptr)
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
	delete world;
}

void Scene::update()
{
	for (UINT i = 0; i < (UINT)ObjType::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			if (!m_arrObj[i][j]->isDead())
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
	CameraMgr* camera = CameraMgr::GetInst();

	XFORM view = camera->GetViewTransform();
	XFORM pitch = camera->GetPitchTransform();

	XFORM ground = {};
	CombineTransform(&ground, &pitch, &view);

	SetGraphicsMode(hdc, GM_ADVANCED);
	SetWorldTransform(hdc, &ground);

	if (world != nullptr)
	{
		world->Render(hdc);
	}

	for (UINT i = 0; i < (UINT)ObjType::END; ++i)
	{
		bool isStand = (i == (UINT)ObjType::Player || i == (UINT)ObjType::Monster);
		vector<Object*>::iterator iter = m_arrObj[i].begin();

		for (; iter != m_arrObj[i].end();)
		{
			if (!(*iter)->isDead())
			{
				if (isStand)
				{
					float deltaY = (*iter)->GetPos().y * (pitch.eM22 - 1.f) + pitch.eDy;
					XFORM placement = { 1.f, 0.f, 0.f, 1.f, 0.f, deltaY };
					XFORM stand = {};
					CombineTransform(&stand, &placement, &view);
					SetWorldTransform(hdc, &stand);
				}
				(*iter)->render(hdc);
				++iter;
			}
			else
			{
				iter = m_arrObj[i].erase(iter);
			}
		}
		SetWorldTransform(hdc, &ground);
	}

	ModifyWorldTransform(hdc, nullptr, MWT_IDENTITY);
	SetGraphicsMode(hdc, GM_COMPATIBLE);
}

void Scene::DeleteAll()
{
	for (UINT i = 0; i < (UINT)ObjType::END; ++i)
	{
		DeleteGroup((ObjType)i);
	}
}

void Scene::DeleteGroup(ObjType target)
{
	Safe_Delete_Vec<Object*>(m_arrObj[(UINT)target]);
}
