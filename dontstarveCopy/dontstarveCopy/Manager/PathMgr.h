#pragma once

class BaseComponent
{

};

struct Objects
{
	wstring ObjName;
	wstring ObjPath;
	vector<BaseComponent*> components;
};

struct ObjectGroup
{
	ObjType type;
	vector<Objects> objects;
	wstring typePath;

};

class PathMgr
{
	SINGLE(PathMgr);

private:
	wchar_t m_szContentPath[255];
	vector<ObjectGroup> m_objGroups;

public:
	void init();

	bool chkDirectory(const wstring& path);

	void SetObjectGroup(ObjType type, const wstring& path);
	const wchar_t* GetContentPath() { return m_szContentPath; }

	const wstring GetObjectGroup(ObjType type) const;

	const Objects* FindObject(ObjectType type, const wstring& objName) const;
};

