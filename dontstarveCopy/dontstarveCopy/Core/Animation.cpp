#include "pch.h"
#include "Animation.h"
#include "Animator.h"
#include "Object.h"
#include "Texture.h"
#include "TimeMgr.h"

Animation::Animation()
	: m_strName()
	, m_pAnimator(nullptr)
	, m_vecFrames{}
	, m_iCurFrame(0)
	, m_fAccTime(0.f)
	, m_fFrameDuration(0.f)
	, m_bFinish(false)
	, m_vOffset{}
{
}

Animation::~Animation()
{
}

void Animation::Create(vector<Texture*> frames, float fDuration, Vec2D vOffset)
{
	m_vecFrames = frames; // 프레임 벡터를 직접 저장
	m_fFrameDuration = fDuration; // 재생 주기 저장
	m_vOffset = vOffset;
}

void Animation::update()
{
	// 재생 종료 시퀸스
	if (m_bFinish)
	{
		return;
	}

	m_fAccTime += fDT;

	if (m_fAccTime > m_fFrameDuration) // 애니메이션 프레임 정보
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
	// 재생 종료 시퀸스
	if (m_bFinish)
	{
		return;
	}

	Object* pObj = m_pAnimator->GetOwner();
	Vec2D	vCenter = pObj->GetPos() + m_vOffset;

	Texture* currentFrameTex = GetFrame(m_iCurFrame);

	XFORM transform;
	GetWorldTransform(hdc, &transform);
	ModifyWorldTransform(hdc, nullptr, MWT_IDENTITY);
	{
		Graphics graphics(hdc);
		Matrix matrix(transform.eM11, transform.eM12, transform.eM21, transform.eM22, transform.eDx, transform.eDy);
		graphics.SetTransform(&matrix);

		Image* img = currentFrameTex->GetImage();
		REAL width = img->GetWidth(), height = img->GetHeight();
		const RECT& box = currentFrameTex->GetOpaqueBounds();
		REAL boxWidth = (REAL)(box.right - box.left), boxHeight = (REAL)(box.bottom - box.top);
		RectF dest(vCenter.x - width / 2.f + box.left, vCenter.y - height / 2.f + box.top, boxWidth, boxHeight);
		graphics.DrawImage(img, dest, (REAL)box.left, (REAL)box.top, boxWidth, boxHeight, UnitPixel);
	}
	SetWorldTransform(hdc, &transform);
}