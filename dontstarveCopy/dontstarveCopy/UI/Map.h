#pragma once
#include "UI.h"
#include "Vec2D.h"
#include "Sprite.h"
#include "Atlas.h"

class Map : public UI
{
private:
	Sprite frame;

	// representing the cached map textures for the visible area
	vector<vector<Atlas*>> cachedTileAtlases;	// 2D array of Atlas pointers for each tile type
	vector<vector<Sprite*>> cachedIcons;

	int zoomLevel;
	Transform drawTransformer;

	// cached DC
	HDC groundDC;
	HDC frameDC;

protected:
	void OnOpen();
	
	void BakeGround();
	void BakeFrame();
	void DrawIcons(HDC targetDC, Vec2D origin, Vec2D cellSize, Vec2D targetSize);

public:
	Map();
	~Map();

	virtual void update();
	virtual void render(HDC hdc);

	void Execute(const InputContext& ctx) override;
	void Refresh() override;
	void Resize() override;
};