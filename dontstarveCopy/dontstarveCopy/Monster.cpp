#include "pch.h"
#include "Monster.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
#include "PathMgr.h"
#include "ResMgr.h"
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
	resPath += L"Textures\\Monsters";
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
	GetAnimator()->Play(ObjType::Player, L"Attack", true, dir);
	ResTex* pTex = GetAnimator()->FindAnimation(L"Player_Attack_Down")->GetFrame(0);
	//GetAnimation()->GetFrame()
	Image* img = pTex->GetImage();
	float width = (float)img->GetWidth();
	float height = (float)img->GetHeight();
	Vec2D baseScale(width, height);
	SetPos(Vec2D(300.f, 300.f));
	SetScale(baseScale);

	Vec2D reScale(0.8f, 0.8f);
	Vec2D colSize = GetScale();

	CreateCollider();
	GetCollider()->SetOffsetPos(colSize / Vec2D(2, 2));

	colSize = colSize * reScale;
	GetCollider()->SetColliderSize(colSize);
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
	Object* pOtherObj = pOther->GetObj();
	if (pOtherObj->GetObjName() == L"Player")
	{
		DeleteObject(this);
	}
}

void Monster::GenerateAnimations(wstring& TexPath) {
    vector<ResTex*> frames;
    wstring animationName = L"";

    GetAnimations(frames, animationName, TexPath);
}

void Monster::GetAnimations(vector<ResTex*> frames, wstring& animationName, wstring& TexPath) {
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

                ResTex* pTex = ResMgr::GetInst()->LoadTex(fileName, fullPath);
                if (pTex) {
                    frames.push_back(pTex);
                }
            }
        }
    } while (FindNextFile(hFind, &findDir));

    if (!frames.empty()) {
        double sec = 1;
        double frameTime = (frames.size() / (double)1000 * sec);

        GetAnimator()->CreateAnimation(animationName, frames, frameTime);
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
