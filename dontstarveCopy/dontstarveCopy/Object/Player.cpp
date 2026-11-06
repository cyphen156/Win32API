#include "pch.h"
#include "Player.h"
#include "TimeMgr.h"
#include "InputMgr.h"
#include "PathMgr.h"
#include "ResourceMgr.h"
#include "Animator.h"
#include "Animation.h"
#include "CameraMgr.h"
#include "PlayerController.h"

Player::Player()
	: m_fspeed(100.f)
	, stateMachine(PlayerState::Alive)
	, controller(new PlayerController(this))
{
	// 리소스 로딩 - Texture

	wstring resPath = PathMgr::GetInst()->GetContentPath();
	resPath += L"Textures\\Character\\Wilson";
	CreateAnimator();
	GenerateAnimations(resPath);
	//MessageBox(nullptr, (L"Player Resource has Loaded"), L"Notice", MB_OK);
	SetCurrDir(L"Down");
	wstring dir = GetCurrDir();
	GetAnimator()->Play(ObjType::Player, L"Idle", true, dir);
	Texture* pTex = GetAnimator()->FindAnimation(L"Wilson_Idle_Down")->GetFrame(0);
	Image* img = pTex->GetImage();
	float width = (float)img->GetWidth();
	float height = (float)img->GetHeight();
	Vec2D baseScale(width, height);
	SetPos(Vec2D(300.f, 300.f));
	SetScale(baseScale);

	// 충돌체는 대기 첫 프레임에서 그림이 차지하는 영역
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2D(-2.f, -34.f));
	GetCollider()->SetColliderSize(Vec2D(44.f, 72.f));
}

Player::Player(const Player& origin)
	: Object(origin)
	, m_fspeed(origin.m_fspeed)
	, stateMachine(origin.stateMachine)
	, controller(new PlayerController(this))
{
}

Player::~Player()
{
	delete controller;
}

void Player::GenerateAnimations(wstring& TexPath)
{
	vector<Texture*> frames;
	wstring animationName = L"";

	GetAnimations(frames, animationName, TexPath);
}
void Player::GetAnimations(vector<Texture*> frames, wstring& animationName, wstring& TexPath)
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

		GetAnimator()->CreateAnimation(animationName, frames, frameTime, Vec2D(0.f, -45.f));
		frames.clear();
		animationName = L"";
	}

	if (isEmpty)
	{
		MessageBox(nullptr, (L"Find " + TexPath + L"but it is Empty Dir.").c_str(), L"Notice", MB_OK);
	}
	FindClose(hFind);
}

void Player::update()
{
	controller->update();

	GetAnimator()->update();
}

void Player::render(HDC hdc)
{
	Vec2D vPlayerScale = GetScale();

	HPEN hOldPen = (HPEN)CreatePen(PS_SOLID, 5, RGB(0, 0, 255));
	HPEN hPrevPen = (HPEN)SelectObject(hdc, hOldPen);
	Rectangle(hdc, (int)(vPlayerScale.x / 2.f)
		, (int)(vPlayerScale.y / 2.f)
		, (int)(vPlayerScale.x / 2.f)
		, (int)(vPlayerScale.y / 2.f));
	SelectObject(hdc, hPrevPen);
	DeleteObject(hOldPen);

	componentRender(hdc);
}