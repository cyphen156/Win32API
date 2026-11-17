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

template<>
Texture* ResourceMgr::CreateResource<Texture>(const wstring& resID, Texture* source, UINT width, UINT height)
{
	Texture* chkTex = FindTexture(resID);
	if (chkTex != nullptr)
	{
		return chkTex;
	}

	if (source == nullptr || source->GetImage() == nullptr || width == 0 || height == 0)
	{
		return nullptr;
	}

	Image* origin = source->GetImage();
	Bitmap* bitmap = new Bitmap((INT)width, (INT)height, PixelFormat32bppARGB);
	{
		Graphics graphics(bitmap);
		graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
		graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);
		graphics.SetCompositingMode(CompositingModeSourceCopy);
		ImageAttributes attributes;
		attributes.SetWrapMode(WrapModeTileFlipXY);
		graphics.DrawImage(origin, Rect(0, 0, (INT)width, (INT)height), 0, 0, (INT)origin->GetWidth(), (INT)origin->GetHeight(), UnitPixel, &attributes);
	}

	Texture* pTex = new Texture;
	pTex->m_img = bitmap;
	pTex->SetKey(resID);

	m_mapResource.insert(make_pair(resID, pTex));

	return pTex;
}

template<>
void ResourceMgr::ReleaseResource<Texture>(const wstring& resID)
{
	map<wstring, Resource*>::iterator iter = m_mapResource.find(resID);

	if (m_mapResource.end() == iter)
	{
		return;
	}

	delete iter->second;
	m_mapResource.erase(iter);
}
