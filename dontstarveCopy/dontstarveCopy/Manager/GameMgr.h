#pragma once

// 게임 진행. 멈춰 있는 동안에는 UI만 갱신하고 그린다
class GameMgr
{
	SINGLE(GameMgr);

private:
	bool isPaused;

public:
	void init();
	void update();
	void render(HDC hdc);

	void SetPause(bool pause) { isPaused = pause; }
};
