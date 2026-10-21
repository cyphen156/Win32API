#pragma once
#include "Vec2D.h"

class Object;

// 대상(플레이어)을 따라가는 카메라. 객체와 지형은 월드 좌표를 갖고, 그릴 때만 화면 좌표로 바꾼다
// 원작 카메라(followcamera.lua)처럼 거리로 줌하고, 거리에 따라 내려다보는 기울기가 바뀐다
// 월드는 평평한 정사각 칸이고, 화면에서는 바닥의 세로만 sin(기울기)만큼 눌린다. 세운 그림은 눌리지 않는다
class CameraMgr
{
	SINGLE(CameraMgr);

private:
	Vec2D lookAt;		
	Vec2D resolution;	
	Object* target;		
	float distance;		
	float zoom;			
	float pitchScale;	

public:
	void Init(Vec2D screenResolution);
	void Update();

	void SetTarget(Object* followTarget) { target = followTarget; }
	void SetLookAt(Vec2D pos) { lookAt = pos; }
	void SetResolution(Vec2D res) { resolution = res; }
	void SetZoom(float z) { zoom = z; }
	void ZoomIn();
	void ZoomOut();

	Vec2D GetLookAt() { return lookAt; }
	Vec2D GetResolution() { return resolution; }
	float GetZoom() { return zoom; }
	float GetPitchScale() { return pitchScale; }
	Vec2D GetRenderPos(Vec2D worldPos);
};
