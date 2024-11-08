#include "pch.h"
#include "ResMgr.h"
#include "Res.h"
#include "ResTex.h"
#include "PathMgr.h"
#include <map>
#include <filesystem>

ResMgr::ResMgr()
{
}

ResMgr::~ResMgr()
{
	Safe_Delete_Map(m_mapTex);
};

ResTex* ResMgr::LoadTex(const wstring& texID, const wstring& texPath)
{
	ResTex* chkTex = FindTex(texID);
	if (chkTex != nullptr)
	{
		return chkTex;
	}
	
	// 이미지 로드
	wstring TexPath = PathMgr::GetInst()->GetContentPath();
	TexPath = texPath;

	ResTex* pTex = new ResTex;
	pTex->Load(TexPath);
	pTex->SetKey(texID);
	pTex->SetPath(texPath);

	m_mapTex.insert(make_pair(texID, pTex));

	return pTex;
}

ResTex* ResMgr::FindTex(const wstring& texID)
{
	map<wstring, Res*>::iterator iter = m_mapTex.find(texID);

	if (m_mapTex.end() == iter)
	{
		return nullptr;
	}

	return (ResTex*)iter->second;
}
