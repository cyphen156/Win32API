#include "pch.h"
#include "ResourceMgr.h"
#include "Core/Resource.h"
#include "Texture.h"
#include "PathMgr.h"
#include <map>
#include <filesystem>

ResourceMgr::ResourceMgr()
{
}

ResourceMgr::~ResourceMgr()
{
	Safe_Delete_Map(m_mapResource);
};

Texture* ResourceMgr::LoadTexture(const wstring& texID, const wstring& texPath)
{
	Texture* chkTex = FindTexture(texID);
	if (chkTex != nullptr)
	{
		return chkTex;
	}
	
	// 이미지 로드
	wstring TexPath = PathMgr::GetInst()->GetContentPath();
	TexPath = texPath;

	Texture* pTex = new Texture;
	pTex->Load(TexPath);
	pTex->SetKey(texID);
	pTex->SetPath(texPath);

	m_mapResource.insert(make_pair(texID, pTex));

	return pTex;
}

Texture* ResourceMgr::FindTexture(const wstring& texID)
{
	map<wstring, Resource*>::iterator iter = m_mapResource.find(texID);

	if (m_mapResource.end() == iter)
	{
		return nullptr;
	}

	return (Texture*)iter->second;
}
