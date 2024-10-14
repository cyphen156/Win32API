#pragma once

class TimeMgr
{
	SINGLE(TimeMgr);

private:
	LARGE_INTEGER	m_liPrevCnt;
	LARGE_INTEGER	m_liCurrCnt;
	LARGE_INTEGER	m_liFrequency;

	double			m_dDT;
	double			m_dAccT;

	UINT			m_iCallCnt;
	UINT			m_iFPS;

public:
	void init();
	void update();

	double GetfDeltaTime() { return m_dDT; }
	float GetDeltaTime() { return (float)m_dDT; }
};