#include "pch.h"
#include "ResMgr.h"
#include "PathMgr.h"

ResMgr::ResMgr()
{
}

ResMgr::~ResMgr()
{
	map<wstring, ResTex*>::iterator iter = m_mapTex.begin();

	for (; iter != m_mapTex.end(); ++iter)
	{
		delete iter->second;
	}
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
	TexPath += texPath;

	ResTex* pTex = new ResTex;
	pTex->Load(TexPath);
	pTex->SetKey(texID);
	pTex->SetPaht(texPath);

	m_mapTex.insert(make_pair(texID, pTex));

	return pTex;
}

ResTex* ResMgr::FindTex(const wstring& texID)
{
	map<wstring, ResTex*>::iterator iter = m_mapTex.find(texID);

	if (m_mapTex.end() == iter)
	{
		return nullptr;
	}

	return iter->second;
}
