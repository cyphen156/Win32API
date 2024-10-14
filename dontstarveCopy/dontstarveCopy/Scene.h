#pragma once
#include <vector>
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

	void update();
	void render(HDC hdc);

	virtual void Enter() = 0;
	virtual void Exit() = 0;

protected:
	void AddObj(Object* pObj, ObjType objType)
	{
		m_arrObj[(UINT)objType].push_back(pObj);
	}
};

