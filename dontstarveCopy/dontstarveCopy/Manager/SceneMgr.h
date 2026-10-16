#pragma once

class Scene;

class SceneMgr 
{
	SINGLE(SceneMgr);

private:
	Scene* m_arrScene[(UINT)SceneType::END];
	Scene* m_pCurScene;

public:
	void init();
	void update();
	void render(HDC hdc);
	Scene* GetCurScene() { return m_pCurScene; }
	void ChangeScene(SceneType next);

	friend class EventMgr;
};