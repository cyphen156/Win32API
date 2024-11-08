#include "pch.h"
#include "Animator.h"
#include "Animation.h"

Animator::Animator()
	: m_pCurAnimation(nullptr)
	, m_pOwner(nullptr)
	, m_bRepeat(false)
{
}

Animator::~Animator()
{
	Safe_Delete_Map(m_mapAnimation);
}

void Animator::CreateAnimation(const wstring& name
	, vector<ResTex*> frames
	, float fDuration)
{
	// 애니메이션이 이미 존재하는지 확인
	if (FindAnimation(name) != nullptr)
	{
		// 중복된 애니메이션에 대한 디버그 메시지 출력
		wcout << L"Warning: Animation '" << name << L"' already exists. Skipping creation." << endl;
		return;
	}

	Animation* pAnim = FindAnimation(name);
	assert(pAnim == nullptr);
	
	pAnim = new Animation;
	pAnim->SetName(name);
	pAnim->m_pAnimator = this;
	pAnim->Create(frames, fDuration);

	m_mapAnimation.insert(make_pair(name, pAnim));
}

Animation* Animator::FindAnimation(const wstring& name)
{
	map<wstring, Animation*>::iterator iter = m_mapAnimation.find(name);

	if (iter == m_mapAnimation.end())
	{
		return nullptr;
	}
	return iter->second;
}

void Animator::Play(ObjType objtpye, const wstring& strName, bool isrepeat, wstring dir)
{
	wstring objTypeStr;
	switch (objtpye)
	{
	case ObjType::Player:  objTypeStr = L"Player_"; break;
	case ObjType::Monster: objTypeStr = L"Monster_"; break;
		// 필요에 따라 다른 객체 타입 추가
	default: objTypeStr = L""; break;
	}

	wstring animationName = objTypeStr + strName + L"_" + dir;

	m_pCurAnimation = FindAnimation(animationName);
	m_bRepeat = isrepeat;
}

void Animator::update()
{
	if (m_pCurAnimation != nullptr)
	{
		if (m_bRepeat && m_pCurAnimation->IsFinish())
		{
			m_pCurAnimation->SetFrame(0);
		}
		m_pCurAnimation->update();
	}
	// 애니메이션 재생
}

void Animator::render(HDC hdc)
{
	//MessageBox(nullptr, (L"After Call Anim Render"), L"Notice", MB_OK);

	if (m_pCurAnimation != nullptr)
	{
		if (m_bRepeat && m_pCurAnimation->IsFinish())
		{
			m_pCurAnimation->SetFrame(0);
		}
		m_pCurAnimation->render(hdc);
	}
	// 애니메이션 렌더링
}