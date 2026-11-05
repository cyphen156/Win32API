#pragma once
#include "Vec2D.h"

class Object;

// 대상(플레이어)을 따라가는 카메라. 
class CameraMgr
{
	SINGLE(CameraMgr);

private:
	Vec2D worldPosition;	// 카메라가 서 있는 바닥 자리 (월드 x, y)
	Vec2D orbitPosition;	// 보는 점(대상의 발) 기준 카메라 위치 (바닥 평면 거리, 높이). 줌 레벨에 따라 바뀜
	float distance;			// 카메라와 보는 점(발 위 1.5단위) 사이의 직선거리 (원작 단위). 높이 = 1.5 + distance x sin(pitch)
	float pitch;			// 내려다보는 기울기 (라디안)
	float pitchScale;		// sin(pitch). 바닥의 세로 눌림

	Vec2D resolution;

	int zoomLevel;
	float zoom;

	Object* target;

	void UpdateOrbitPose();	// 줌 레벨이 바뀔 때: 궤도 위 카메라 자리와 보는 각도를 갱신

public:
	void init();
	void Update();

	void SetTarget(Object* followTarget) { target = followTarget; }
	void ZoomIn();
	void ZoomOut();

	XFORM GetPitchTransform();
	XFORM GetInversePitchTransform();
	XFORM GetMultiplierTransform();

	RECT GetViewArea();
	Vec2D GetLookAt();
};