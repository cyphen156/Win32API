#pragma once

class Resource;
class Texture;

class ResourceMgr
{
	SINGLE(ResourceMgr);

private:
	map<wstring, Resource*>	m_mapResource;

public:
	Texture* LoadTexture(const wstring& texID, const wstring& texPath);
	Texture* FindTexture(const wstring& texID);

	template<typename T>
	T* CreateResource(const wstring& resID, T* source, UINT width, UINT height);
	template<typename T>
	void ReleaseResource(const wstring& resID);
};

template<>
Texture* ResourceMgr::CreateResource<Texture>(const wstring& resID, Texture* source, UINT width, UINT height);
template<>
void ResourceMgr::ReleaseResource<Texture>(const wstring& resID);
