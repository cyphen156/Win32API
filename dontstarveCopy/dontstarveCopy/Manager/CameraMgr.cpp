#include "pch.h"
#include "CameraMgr.h"
#include "Object.h"
#include "Core.h"
#include <cmath>

static const float minDistance = 15.f;
static const float maxDistance = 50.f;
static const float normalDistance = 36.f;
static const float minPitch = 30.f;
static const float maxPitch = 60.f;
static const float focusHeight = 1.5f;

static const int minZoomLevel = -3;
static const int maxZoomLevel = 8;
static const float zoomStep = 1.1135f;

static const float fieldOfView = 35.f;
static const float pixelsPerUnit = 30.f;
static const float degreeToRadian = 3.14159265f / 180.f;

CameraMgr::CameraMgr()
	: worldPosition{}
	, orbitPosition{}
	, distance(normalDistance)
	, pitch(0.f)
	, pitchScale(1.f)
	, resolution{}
	, zoomLevel(0)
	, zoom(1.f)
	, target(nullptr)
{
}

CameraMgr::~CameraMgr()
{
}

void CameraMgr::init()
{
	UpdateOrbitPose();
}

void CameraMgr::Update()
{
	// 적응형 해상도
	POINT windowSize = Core::GetInst()->GetResolution();
	resolution = Vec2D((float)windowSize.x, (float)windowSize.y);
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

	Vec2D foot = target->GetPos();
	worldPosition = Vec2D(foot.x, foot.y + orbitPosition.x);
}

void CameraMgr::ZoomIn()
{
	zoomLevel = min(maxZoomLevel, zoomLevel + 1);
	UpdateOrbitPose();
}

void CameraMgr::ZoomOut()
{
	zoomLevel = max(minZoomLevel, zoomLevel - 1);
	UpdateOrbitPose();
}

void CameraMgr::UpdateOrbitPose()
{
	float normalPitch = (minPitch + (normalDistance - minDistance) / (maxDistance - minDistance) * (maxPitch - minPitch)) * degreeToRadian;
	float focusDistance = (normalDistance + focusHeight * sinf(normalPitch)) / powf(zoomStep, (float)zoomLevel);

	distance = focusDistance;
	for (int i = 0; i < 3; ++i)
	{
		pitch = (minPitch + (distance - minDistance) / (maxDistance - minDistance) * (maxPitch - minPitch)) * degreeToRadian;
		distance = focusDistance - focusHeight * sinf(pitch);
	}

	pitchScale = sinf(pitch);
	orbitPosition = Vec2D(distance * cosf(pitch) * pixelsPerUnit, (focusHeight + distance * pitchScale) * pixelsPerUnit);
}

Vec2D CameraMgr::GetLookAt()
{
	return Vec2D(worldPosition.x, worldPosition.y - orbitPosition.x);
}

XFORM CameraMgr::GetViewTransform()
{
	Vec2D lookAt = GetLookAt();

	XFORM view = {
		zoom, 0.f,
		0.f, zoom,
		resolution.x / 2.f - lookAt.x * zoom,
		resolution.y / 2.f - lookAt.y * zoom
	};

	return view;
}

XFORM CameraMgr::GetPitchTransform()
{
	Vec2D lookAt = GetLookAt();

	XFORM pitch = {
		1.f, 0.f,
		0.f, pitchScale,
		0.f, lookAt.y * (1.f - pitchScale)
	};

	return pitch;
}

RECT CameraMgr::GetViewArea()
{
	Vec2D lookAt = GetLookAt();
	float halfWidth = resolution.x / 2.f / zoom;
	float halfHeight = resolution.y / 2.f / (zoom * pitchScale);
	RECT area = { (LONG)floor(lookAt.x - halfWidth), (LONG)floor(lookAt.y - halfHeight), (LONG)ceil(lookAt.x + halfWidth), (LONG)ceil(lookAt.y + halfHeight) };
	return area;
}