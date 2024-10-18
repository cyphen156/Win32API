#pragma once
#include "ResTex.h"
#include "ResAud.h"
#include <map>

class ResTex;

class ResMgr
{
	SINGLE(ResMgr);

private:
	map<wstring, ResTex*>	m_mapTex;

public:
	ResTex* LoadTex(const wstring& texID, const wstring& texPath);
	ResTex* FindTex(const wstring& texID);
};

