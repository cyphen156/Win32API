#include "pch.h"
#include "TimeMgr.h"
#include "Core.h"

TimeMgr::TimeMgr() : m_liPrevCnt{}, m_liCurrCnt{}, m_liFrequency{}, m_dDT(0.), m_dAccT(0.), m_iCallCnt(0), m_iFPS(0)
{
}

TimeMgr::~TimeMgr()
{
}

void TimeMgr::init()
{
	// 현재 카운트
	QueryPerformanceCounter(&m_liCurrCnt);

	// 초당 카운트
	QueryPerformanceFrequency(&m_liFrequency);
}

void TimeMgr::update()
{
	QueryPerformanceCounter(&m_liCurrCnt);

	m_dDT = (double)(m_liCurrCnt.QuadPart - m_liPrevCnt.QuadPart) / (double)m_liFrequency.QuadPart;

	++m_iCallCnt;
	m_dAccT += m_dDT;
	m_liPrevCnt = m_liCurrCnt;

	if (m_dAccT >= 1.)
	{
		m_iFPS = m_iCallCnt;
		m_dAccT = 0.;
		m_iCallCnt = 0;

		wchar_t szBuff[255] = {};

		swprintf_s(szBuff, L"fps : %d, DT : %lf", m_iFPS, m_dDT);
		SetWindowText(Core::GetInst()->GetMainHWND(), szBuff);
	}
}