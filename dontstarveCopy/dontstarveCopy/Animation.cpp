#include "pch.h"
#include "Animation.h"
#include "Animator.h"
#include "Object.h"
#include "ResTex.h"
#include "TimeMgr.h"

Animation::Animation()
	: m_strName()
	, m_pAnimator(nullptr)
	, m_vecFrames{}
	, m_iCurFrame(0)
	, m_fAccTime(0.f)
	, m_fFrameDuration(0.f)
	, m_bFinish(false)
{
}

Animation::~Animation()
{
}

void Animation::Create(vector<ResTex*> frames, float fDuration)
{
	m_vecFrames = frames; // ������ ���͸� ���� ����
	m_fFrameDuration = fDuration; // ��� �ֱ� ����
}

void Animation::update()
{
	// ��� ���� ������
	if (m_bFinish)
	{
		return;
	}

	m_fAccTime += fDT;

	if (m_fAccTime > m_fFrameDuration) // �ִϸ��̼� ������ ����
	{
		++m_iCurFrame;

		if (m_vecFrames.size() <= m_iCurFrame)
		{
			m_iCurFrame = 0;
			m_bFinish = true;
			m_fAccTime = 0.f;
			return;
		}
		m_fAccTime = m_fAccTime - m_fFrameDuration;
	}
}

void Animation::render(HDC hdc)
{
	// ��� ���� ������
	if (m_bFinish)
	{
		return;
	}

	Object* pObj = m_pAnimator->GetObj();
	Vec2D	vPos = pObj->GetPos();
	
	ResTex* currentFrameTex = GetFrame(m_iCurFrame);
	//if (currentFrameTex)
	//{
	//	HDC imgDC = currentFrameTex->GetDC();
	//	// ���� �������� ��Ʈ���� HDC�� �׸���
	//	TransparentBlt(hdc,
	//		(int)vPos.x - (currentFrameTex->Width() / 2), // X ��ǥ
	//		(int)vPos.y - (currentFrameTex->Height() / 2), // Y ��ǥ
	//		currentFrameTex->Width(), // �ʺ�
	//		currentFrameTex->Height(), // ����
	//		imgDC, // �̹��� DC
	//		0, 0, // �̹��� ����
	//		currentFrameTex->Width(), // ���� �̹��� �ʺ�
	//		currentFrameTex->Height(), // ���� �̹��� ����
	//		RGB(255, 0, 255)); // �����
	//}

	if (currentFrameTex)
	{
		Graphics graphics(hdc);
		// ���� �������� �̹����� ���� ��ü ��ġ�� �׸���
		graphics.DrawImage(currentFrameTex->GetImage(), (int)vPos.x, (int)vPos.y);
	}
}