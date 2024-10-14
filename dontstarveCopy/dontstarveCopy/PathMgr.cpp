#include "pch.h"
#include "PathMgr.h"
#include "Core.h"

PathMgr::PathMgr() : m_szContentPath{}
{

}

PathMgr::~PathMgr()
{

}

void PathMgr::init()
{
	GetCurrentDirectory(255, m_szContentPath);

	int iLen = wcslen(m_szContentPath);

	for (int i = iLen - 1; 0 <= i; --i)
	{
		if ('\\' == m_szContentPath[i])
		{
			m_szContentPath[i] = '\0';
			break;
		}
	}
	wcscat_s(m_szContentPath, 255, L"\\bin\\content\\");

	//SetWindowText(Core::GetInst()->GetMainHWND(), m_szContentPath);
}

