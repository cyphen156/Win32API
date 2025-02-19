#pragma once
#include "Object.h"

class Object;

class Scene
{
private:
	vector <Object*>	m_arrObj[(UINT)ObjType::END];
	wstring				m_sceneName;

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

	const vector<Object*>& GetObjectGroup(ObjType objType) { return m_arrObj[(UINT)objType]; }
	
	void DeleteAll();
	void DeleteGroup(ObjType target);
};

