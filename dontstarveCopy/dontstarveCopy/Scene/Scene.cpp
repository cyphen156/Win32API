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

	Vec2D lookAt = camera->GetLookAt();

	XFORM toLookAt = { 1.f, 0.f, 0.f, 1.f, -lookAt.x, -lookAt.y };
	XFORM pitch = camera->GetPitchTransform();
	XFORM inversePitch = camera->GetInversePitchTransform();
	XFORM multiplier = camera->GetMultiplierTransform();

	XFORM focused = {};
	CombineTransform(&focused, &toLookAt, &pitch);
	XFORM ground = {};
	CombineTransform(&ground, &focused, &multiplier);

	SetGraphicsMode(hdc, GM_ADVANCED);
	SetWorldTransform(hdc, &ground);

	if (world != nullptr)
	{
		world->Render(hdc);
	}

	RECT viewArea = camera->GetViewArea();
	InflateRect(&viewArea, TILESIZE, TILESIZE);

	for (UINT i = 0; i < (UINT)ObjType::END; ++i)
	{
		bool isStand = (i == (UINT)ObjType::Player || i == (UINT)ObjType::Monster);
		vector<Object*>::iterator iter = m_arrObj[i].begin();

		for (; iter != m_arrObj[i].end();)
		{
			// Culling: 화면 밖에 있는 오브젝트는 렌더링하지 않음
			Vec2D position = (*iter)->GetPos();
			if (position.x < viewArea.left || position.x > viewArea.right 
				|| position.y < viewArea.top || position.y > viewArea.bottom)
			{
				++iter;
				continue;
			}

			if (isStand)
			{
				XFORM toPosition = { 1.f, 0.f, 0.f, 1.f, -position.x, -position.y };
				XFORM fromPosition = { 1.f, 0.f, 0.f, 1.f, position.x, position.y };

				XFORM unpressed = {};
				CombineTransform(&unpressed, &toPosition, &inversePitch);
				XFORM standing = {};
				CombineTransform(&standing, &unpressed, &fromPosition);
				XFORM stand = {};
				CombineTransform(&stand, &standing, &ground);
				SetWorldTransform(hdc, &stand);
			}
			(*iter)->render(hdc);
			++iter;
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
