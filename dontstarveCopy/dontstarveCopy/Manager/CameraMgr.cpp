#include "pch.h"
#include "CameraMgr.h"
#include "Object.h"
#include "Core.h"
#include <cmath>

static const float minDistance = 15.f;		
static const float maxDistance = 50.f;		
static const float zoomStep = 4.f;			
static const float normalDistance = 36.f;	
static const float minPitch = 30.f;			
static const float maxPitch = 60.f;			
static const float fieldOfView = 35.f;		
static const float focusHeight = 1.5f;		
static const float pixelsPerUnit = 30.f;	
static const float degreeToRadian = 3.14159265f / 180.f;

CameraMgr::CameraMgr()
	: lookAt{}
	, resolution{}
	, target(nullptr)
	, distance(normalDistance)
	, zoom(1.f)
	, pitchScale(1.f)
{
}

CameraMgr::~CameraMgr()
{
}

void CameraMgr::Init(Vec2D screenResolution)
{
	resolution = screenResolution;
}

void CameraMgr::Update()
{
	POINT windowSize = Core::GetInst()->GetResolution();
	resolution = Vec2D((float)windowSize.x, (float)windowSize.y);

	float pitch = minPitch + (distance - minDistance) / (maxDistance - minDistance) * (maxPitch - minPitch);
	pitchScale = sinf(pitch * degreeToRadian);
	float screenPixelsPerUnit = resolution.y / (2.f * tanf(fieldOfView * 0.5f * degreeToRadian) * (distance + focusHeight * pitchScale));
	zoom = screenPixelsPerUnit / pixelsPerUnit;

	if (target == nullptr)
	{
		return;
	}
	if (target->isDead())
	{
		target = nullptr;
		return;
	}

	Vec2D targetPos = target->GetPos();
	Vec2D targetScale = target->GetScale();
	Vec2D targetAnchor = target->GetFootAnchor();
	lookAt = Vec2D(targetPos.x + targetScale.x / 2.f + targetAnchor.x, targetPos.y + targetScale.y / 2.f + targetAnchor.y);
}

void CameraMgr::ZoomIn()
{
	distance -= zoomStep;
	if (distance < minDistance)
	{
		distance = minDistance;
	}
}

void CameraMgr::ZoomOut()
{
	distance += zoomStep;
	if (distance > maxDistance)
	{
		distance = maxDistance;
	}
}

Vec2D CameraMgr::GetRenderPos(Vec2D worldPos)
{
	// 바닥 위 점: 가로는 줌, 세로는 줌 x 기울기 눌림
	return Vec2D((worldPos.x - lookAt.x) * zoom + resolution.x / 2.f, (worldPos.y - lookAt.y) * zoom * pitchScale + resolution.y / 2.f);
}
