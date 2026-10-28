#include "pch.h"
#include "Monster.h"
#include "TimeMgr.h"
#include "PathMgr.h"
#include "ResourceMgr.h"
#include "Animator.h"
#include "Animation.h"

Monster::Monster()
	: m_vCenterPos(Vec2D(0.f, 0.f))
	, m_fspeed(100.f)
	, m_fMaxDist(50.f)
	, m_iDirection(1)
    , bisRight(true)

{
	// 리소스 로딩 - Texture
	wstring resPath = PathMgr::GetInst()->GetContentPath();
	resPath += L"Textures\\Monster";
	// 캐릭터 선택 여부 확인
	/*if (!wcscmp(character, "wilson"))
	{
		resPath += L"\\Wilson";
	}*/
	CreateAnimator();
	GenerateAnimations(resPath);
	MessageBox(nullptr, (L"Monster Resource has Loaded"), L"Notice", MB_OK);
	SetCurrDir(L"Down");
	wstring dir = GetCurrDir();
	GetAnimator()->Play(ObjType::Monster, L"IdleLoop", true, dir);
	Texture* pTex = GetAnimator()->FindAnimation(L"Pigman_IdleLoop_Down")->GetFrame(0);
	//GetAnimation()->GetFrame()
	Image* img = pTex->GetImage();
	float width = (float)img->GetWidth();
	float height = (float)img->GetHeight();
	Vec2D baseScale(width, height);
	SetPos(Vec2D(300.f, 300.f));
	SetScale(baseScale);

	// 충돌체는 대기 첫 프레임에서 그림이 차지하는 영역
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2D(0.f, -31.5f));
	GetCollider()->SetColliderSize(Vec2D(52.f, 65.f));
}

Monster::Monster(const Monster& origin)
	: Object(origin)
	, m_vCenterPos(origin.m_vCenterPos)
	, m_fspeed(origin.m_fspeed)
	, m_fMaxDist(origin.m_fMaxDist)
	, m_iDirection(origin.m_iDirection)
	, bisRight(origin.bisRight)
{
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2D(0.f, 0.f));
	GetCollider()->SetColliderSize(Vec2D(100.f, 100.f));
}
Monster::~Monster()
{
}

void Monster::OnCollisionEnter(Collider* pOther)
{
	Object* pOtherObj = pOther->GetOwner();
	if (pOtherObj->GetObjName() == L"Player")
	{
		DeleteObj(this);
	}
}

void Monster::GenerateAnimations(wstring& TexPath) {
    vector<Texture*> frames;
    wstring animationName = L"";

    GetAnimations(frames, animationName, TexPath);
}

void Monster::GetAnimations(vector<Texture*> frames, wstring& animationName, wstring& TexPath) {
    WIN32_FIND_DATA findDir;
    HANDLE hFind = FindFirstFile((TexPath + L"\\*").c_str(), &findDir);

    if (hFind == INVALID_HANDLE_VALUE) {
        MessageBox(nullptr, (L"Cannot find Dir" + TexPath + L" Error :: Invalid Path.").c_str(), L"Error", MB_OK);
        return;
    }

    bool isEmpty = true;

    do {
        if (wcscmp(findDir.cFileName, L".") != 0 && wcscmp(findDir.cFileName, L"..") != 0) {
            isEmpty = false;
            wstring fullPath = TexPath + L"\\" + findDir.cFileName;

            if (findDir.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                // ��� ȣ��
                GetAnimations(frames, animationName, fullPath);
            }
            else if (wstring(findDir.cFileName).find(L".png") != wstring::npos) {
                wstring fileName = findDir.cFileName;
                animationName = fileName.substr(0, fileName.find_last_of(L'_'));

                Texture* pTex = ResourceMgr::GetInst()->LoadTexture(fileName, fullPath);
                if (pTex) {
                    frames.push_back(pTex);
                }
            }
        }
    } while (FindNextFile(hFind, &findDir));

    if (!frames.empty()) {
        double frameTime = 1.0 / 30.0;

        GetAnimator()->CreateAnimation(animationName, frames, frameTime, Vec2D(5.f, -36.f));
        frames.clear();
        animationName = L"";
    }

    if (isEmpty) {
        MessageBox(nullptr, (L"Find " + TexPath + L" but it is Empty Dir.").c_str(), L"Notice", MB_OK);
    }

    FindClose(hFind);
}


void Monster::update()
{
	Vec2D vPos = GetPos();

	vPos.x += m_fspeed * m_iDirection * fDT;

	float fDist = abs(m_vCenterPos.x - vPos.x) - m_fMaxDist;

	if (fDist > 0)
	{
		m_iDirection *= -1;

		vPos.x += fDist * m_iDirection;
	}

	SetPos(vPos);
}

void Monster::render(HDC hdc)
{
}
