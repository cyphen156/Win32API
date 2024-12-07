#pragma once

class PathMgr
{
	SINGLE(PathMgr);

private:
	wchar_t m_szContentPath[255];
	vector<ObjectGroup> m_ObjectGroup;

public:
	void init();
	void SetObjectGroup(ObjType type, const wstring& path);
	const wchar_t* GetContentPath() { return m_szContentPath; }

	const wstring GetObjectGroup(ObjType type);

};

