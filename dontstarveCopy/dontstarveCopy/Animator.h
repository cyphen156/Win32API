#pragma once
#include "Struct.h"

class Object;
class Animation;
class ResTex;

class Animator
{
private:
	map<wstring, Animation*>	m_mapAnimation;
	Animation*				    m_pCurAnimation;
	Object*						m_pOwner;
	bool						m_bRepeat;

public:
	Animator();
	~Animator();

	/**
	* CreateAnimation(텍스쳐, 시작위치, 프레임 사이즈, 프레임 간격)
	*/
	void CreateAnimation(const wstring& name
		, vector<ResTex*> frames
		, float fDuration);
	Animation* FindAnimation(const wstring& name);
	void Play(ObjType objtpye, const wstring& strName, bool isrepeat, wstring dir);
	Object* GetObj() { return m_pOwner; };

	void update();
	void render(HDC hdc);
	friend class Object;
};