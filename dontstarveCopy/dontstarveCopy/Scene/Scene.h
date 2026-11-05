#pragma once
#include "Object.h"

class Object;
class World;

class Scene
{
private:
	vector <Object*>	m_arrObj[(UINT)ObjType::END];
	wstring				m_sceneName;
	World*				world;

public:
	Scene();
	virtual ~Scene();

	void SetName(const wstring name) { m_sceneName = name; }
	wstring GetSceneName() { return m_sceneName; }

	virtual void update();
	virtual void finalupdate();
	virtual void render(HDC hdc);

	virtual void Enter() = 0;
	virtual void Exit() = 0;

	void AddObj(Object* pObj, ObjType objType)
	{
		m_arrObj[(UINT)objType].push_back(pObj);
	}

	void RemoveObj(Object* pObj)
	{
		for (UINT i = 0; i < (UINT)ObjType::END; ++i)
		{
			vector<Object*>::iterator iter = find(m_arrObj[i].begin(), m_arrObj[i].end(), pObj);
			if (iter != m_arrObj[i].end())
			{
				m_arrObj[i].erase(iter);
				return;
			}
		}
	}

	const vector<Object*>& GetObjectGroup(ObjType objType) { return m_arrObj[(UINT)objType]; }

	World* GetWorld() { return world; }
	void SetWorld(World* ownedWorld) { world = ownedWorld; }
	
	void DeleteAll();
	void DeleteGroup(ObjType target);
};

