#include "pch.h"
#include "Player.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
#include "PathMgr.h"
#include "ResMgr.h"
#include "Animator.h"
#include "Animation.h"
#include "PathMgr.h"

Player::Player()
	: m_fspeed(100.f)
	, bisRight(true)
{
	// 리소스 로딩 - Texture
	

	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2D(0.f, 0.f));
	GetCollider()->SetColliderSize(Vec2D(100.f, 100.f));

	wstring resPath = PathMgr::GetInst()->GetContentPath();
	resPath += L"Textures\\Player";
	// 캐릭터 선택 여부 확인
	/*if (!wcscmp(character, "wilson"))
	{
		resPath += L"\\Wilson";
	}*/
	CreateAnimator();
	GenerateAnimations(resPath);
	MessageBox(nullptr, (L"All Resource has Loaded"), L"Notice", MB_OK);
	SetCurrDir(L"Down");
	wstring dir = GetCurrDir();
	GetAnimator()->Play(ObjType::Player, L"Run", false, dir);


}

Player::~Player()
{
	
}

void Player::GenerateAnimations(wstring& TexPath)
{
	vector<ResTex*> frames;
	wstring animationName;

	GetAnimations(frames, animationName, TexPath);
}
void Player::GetAnimations(vector<ResTex*> frames, wstring animationName, wstring& TexPath)
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
				// 하위 폴더가 있는 경우 재귀 호출
				GetAnimations(fullPath);
			}
			else if (wstring(findDir.cFileName).find(L".png") != wstring::npos)
			{
				// 하위 폴더가 없고, png파일이 있다면
				// 여러 파일을 로드해서 각각 프레임으로 등록하여 애니메이션 생성
				// 애니메이션 생성
				// PNG 파일을 로드하여 프레임으로 추가
				wstring fileName = findDir.cFileName;

				// 확장자 제거
				animationName = fileName.substr(0, fileName.find_last_of(L'_'));

				//animationName = TexPath.substr(TexPath.find_last_of(L"\\") + 1);
				ResTex* pTex = ResMgr::GetInst()->LoadTex(fileName, fullPath);
				if (pTex)
				{
					frames.push_back(pTex);
					//MessageBox(nullptr, (fileName + L" is loaded " + std::to_wstring(frames.size()) + L" frames").c_str(), L"Notice", MB_OK);
				}

				// 수정 전 한개의 파일에서 프레임을 잘라내어 애니메이션 생성
				//ResTex* pTex = ResMgr::GetInst()->LoadTex(animationName + L"_" + findDir.cFileName, fullPath);

				//if (pTex)
				//{
				//	// 애니메이션 생성: 프레임 속성 설정 및 등록
				//	GetAnimator()->CreateAnimation(animationName, pTex, Vec2D(0.f, 0.f), Vec2D(100.f, 100.f), Vec2D(100.f, 0.f), 0.1f, 1);
				//}
			}
		}
	} while (FindNextFile(hFind, &findDir));

	// 애니메이션 생성
	if (!frames.empty())
	{
		GetAnimator()->CreateAnimation(animationName, frames, 0.5f); // 각 프레임의 지속 시간 0.1초
		//MessageBox(nullptr, (animationName + L" is loaded " + std::to_wstring(frames.size()) + L" frames").c_str(), L"Notice", MB_OK);
		frames.clear();
		animationName = L"";
	}
	
	if (isEmpty)
	{
		// 폴더는 있지만 하위에 파일이나 폴더가 없는 경우
		MessageBox(nullptr, (L"Find " + TexPath + L"but it is Empty Dir.").c_str(), L"Notice", MB_OK);
	}
	FindClose(hFind);
}

void Player::update()
{
	Vec2D vPos = GetPos();

	if (KEY_PRESS(KEY::W))
	{
		vPos.y -= m_fspeed * fDT;
		SetCurrDir(L"Up");
	}
	if (KEY_PRESS(KEY::A))
	{
		vPos.x -= m_fspeed * fDT;
		SetCurrDir(L"Side");
		bisRight = false;
	}
	if (KEY_PRESS(KEY::S))
	{
		vPos.y += m_fspeed * fDT;
		SetCurrDir(L"Down");
	}
	if (KEY_PRESS(KEY::D))
	{
		vPos.x += m_fspeed * fDT;
		SetCurrDir(L"Side");
		bisRight = true;
	}
	if (KEY_PRESS(KEY::SPACE))
	{
		//attack();, chop
	}

	SetPos(vPos);

	GetAnimator()->update();
}

void Player::render(HDC hdc)
{

	//리소스 개별로딩때 써먹엇던 코드

	Vec2D vPlayerPos = GetPos();

	UINT iWidth = 20;
	UINT iHeight = 20;
	//MessageBox(nullptr, (L"Before Call Anim Render"), L"Notice", MB_OK);


	GetAnimator()->render(hdc);

	/*Rectangle(hdc, 50, 50, iWidth, iHeight);*/

	// 애니메이션 렌더링

	componentRender(hdc);
}