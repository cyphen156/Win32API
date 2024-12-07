#pragma once

class Res;
class ResTex;

class ResMgr
{
	SINGLE(ResMgr);

private:
	map<wstring, Res*>	m_mapTex;


public:
	ResTex* LoadTex(const wstring& texID, const wstring& texPath);
	ResTex* FindTex(const wstring& texID);
};

