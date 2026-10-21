#include "pch.h"
#include "Animation.h"
#include "CameraMgr.h"
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
{
}

Animation::~Animation()
{
}

void Animation::Create(vector<Texture*> frames, float fDuration)
{
	m_vecFrames = frames; // 프레임 벡터를 직접 저장
	m_fFrameDuration = fDuration; // 재생 주기 저장
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
	Vec2D	vScale = pObj->GetScale();
	Vec2D	vAnchor = pObj->GetFootAnchor();
	Vec2D	vFoot = CameraMgr::GetInst()->GetRenderPos(pObj->GetPos() + Vec2D(vScale.x / 2.f + vAnchor.x, vScale.y / 2.f + vAnchor.y));
	float	fZoom = CameraMgr::GetInst()->GetZoom();
	Vec2D	vCenter = Vec2D(vFoot.x - vAnchor.x * fZoom, vFoot.y - vAnchor.y * fZoom);
	
	Texture* currentFrameTex = GetFrame(m_iCurFrame);
	//if (currentFrameTex)
	//{
	//	HDC imgDC = currentFrameTex->GetDC();
	//	// 현재 프레임의 비트맵을 HDC에 그리기
	//	TransparentBlt(hdc,
	//		(int)vPos.x - (currentFrameTex->Width() / 2), // X 좌표
	//		(int)vPos.y - (currentFrameTex->Height() / 2), // Y 좌표
	//		currentFrameTex->Width(), // 너비
	//		currentFrameTex->Height(), // 높이
	//		imgDC, // 이미지 DC
	//		0, 0, // 이미지 원점
	//		currentFrameTex->Width(), // 원본 이미지 너비
	//		currentFrameTex->Height(), // 원본 이미지 높이
	//		RGB(255, 0, 255)); // 투명색
	//}

	if (currentFrameTex && currentFrameTex->GetImage())
	{
		Graphics graphics(hdc);
		Image* img = currentFrameTex->GetImage();
		// 현재 프레임의 이미지를 현재 객체 위치에 그리기
		REAL width = img->GetWidth() * fZoom, height = img->GetHeight() * fZoom;
		graphics.DrawImage(img, vCenter.x - width / 2.f, vCenter.y - height / 2.f, width, height);
	}
}