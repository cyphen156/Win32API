#include "pch.h"
#include "Monster.h"
#include "PathMgr.h"
#include "ResourceMgr.h"
#include "Animator.h"
#include "MonsterController.h"

Monster::Monster()
	: m_fspeed(100.f)
	, m_vCenterPos(Vec2D(0.f, 0.f))
	, m_fMaxDist(50.f)
	, m_MonsterType(MonsterType::END)
	, stateMachine(MonsterState::Alive)
	, controller(new MonsterController(this))
{
	CreateAnimator();
	SetCurrDir(L"Down");
}

Monster::Monster(const Monster& origin)
	: Object(origin)
	, m_fspeed(origin.m_fspeed)
	, m_vCenterPos(origin.m_vCenterPos)
	, m_fMaxDist(origin.m_fMaxDist)
	, m_MonsterType(origin.m_MonsterType)
	, stateMachine(origin.stateMachine)
	, controller(new MonsterController(this))
{
}

Monster::~Monster()
{
	delete controller;
}

void Monster::GenerateAnimations(wstring& TexPath)
{
	vector<Texture*> frames;
	wstring animationName = L"";

	wstring resPath = PathMgr::GetInst()->GetContentPath();
	resPath += L"Textures\\Monster\\" + TexPath;

	GetAnimations(frames, animationName, resPath);
}

void Monster::GetAnimations(vector<Texture*> frames, wstring& animationName, wstring& TexPath)
{
	WIN32_FIND_DATA findDir;
	HANDLE hFind = FindFirstFile((TexPath + L"\\*").c_str(), &findDir);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		// 로더 경로명 문제 발생
		MessageBox(nullptr, (L"Cannot find Dir" + TexPath + L" Error :: Invalid Path.").c_str(), L"Error", MB_OK);
		return;
	}

	bool isEmpty = true;

	// 현재 경로의 마지막 폴더명을 애니메이션 이름으로 사용
	do {
		if (wcscmp(findDir.cFileName, L".") != 0 && wcscmp(findDir.cFileName, L"..") != 0)
		{
			isEmpty = false;
			wstring fullPath = TexPath + L"\\" + findDir.cFileName;

			if (findDir.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				GetAnimations(frames, animationName, fullPath);
			}
			else if (wstring(findDir.cFileName).find(L".png") != wstring::npos)
			{
				wstring fileName = findDir.cFileName;

				animationName = fileName.substr(0, fileName.find_last_of(L'_'));

				Texture* pTex = ResourceMgr::GetInst()->LoadTexture(fileName, fullPath);
				if (pTex)
				{
					frames.push_back(pTex);
				}
			}
		}
	} while (FindNextFile(hFind, &findDir));

	// 애니메이션 생성
	if (!frames.empty())
	{
		double frameTime = 1.0 / 30.0;

		Vec2D vOffset;
		switch (GetMonsterType())
		{
		case MonsterType::Pigman: vOffset = Vec2D(5.f, -36.f); break;
		case MonsterType::Spider: vOffset = Vec2D(7.f, -33.f); break;
		default: break;
		}
		GetAnimator()->CreateAnimation(animationName, frames, frameTime, vOffset);
		frames.clear();
		animationName = L"";
	}

	if (isEmpty)
	{
		MessageBox(nullptr, (L"Find " + TexPath + L" but it is Empty Dir.").c_str(), L"Notice", MB_OK);
	}
	FindClose(hFind);
}

void Monster::update()
{
	controller->update();

	GetAnimator()->update();
}