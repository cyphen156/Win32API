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
};

