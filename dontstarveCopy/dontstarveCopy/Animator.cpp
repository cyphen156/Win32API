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
	// �ִϸ��̼��� �̹� �����ϴ��� Ȯ��
	if (FindAnimation(name) != nullptr)
	{
		// �ߺ��� �ִϸ��̼ǿ� ���� ����� �޽��� ���
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
		// �ʿ信 ���� �ٸ� ��ü Ÿ�� �߰�
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
	// �ִϸ��̼� ���
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
	// �ִϸ��̼� ������
}