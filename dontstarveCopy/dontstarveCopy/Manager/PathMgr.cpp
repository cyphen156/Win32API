#include "pch.h"
#include "PathMgr.h"
#include "Core.h"

PathMgr::PathMgr() : m_szContentPath{}
	, m_objGroups{}
{

}

PathMgr::~PathMgr()
{

}

void PathMgr::init()
{
	GetCurrentDirectory(255, m_szContentPath);

	int iLen = (int)wcslen(m_szContentPath);

	for (int i = iLen - 1; 0 <= i; --i)
	{
		if ('\\' == m_szContentPath[i])
		{
			m_szContentPath[i] = '\0';
			break;
		}
	}
	wcscat_s(m_szContentPath, 255, L"\\bin\\content\\");

	chkDirectory(m_szContentPath);
	SetWindowText(Core::GetInst()->GetMainHWND(), m_szContentPath);

}

bool PathMgr::chkDirectory(const wstring& path)
{
	WIN32_FIND_DATA findDir;
	HANDLE hFind = FindFirstFile((path + L"\\*").c_str(), &findDir);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		// 디렉토리가 없음
		MessageBox(nullptr, (L"Cannot find Dir" + path + L" Error :: Invalid Path.").c_str(), L"Error", MB_OK);
		FindClose(hFind);
		return false;
	}
	
	// 디렉토리가 있음
	FindClose(hFind);
	return true;
}

//void PathMgr::SetObjectGroup(ObjType type, const wstring& path)
//{
//	for (const auto& group : m_ObjectGroup) {
//		if (group.Type == type) {
//			return;
//		}
//	}
//	m_ObjectGroup.push_back({ type, path });
//}
//
//const wstring PathMgr::GetObjectGroup(ObjType type)
//{
//	for (const auto& group : m_ObjectGroup) {
//		if (group.Type == type) {
//			return group.ObjectPath;
//		}
//	}
//
//	MessageBox(nullptr, L"PathMgr:: GetObjGroup Error, there Invalid Path", L"Error", MB_OK);
//	return L"";
//}