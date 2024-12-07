#include "pch.h"
#include "Player.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
#include "PathMgr.h"
#include "ResMgr.h"
#include "Animator.h"
#include "Animation.h"

Player::Player()
	: m_fspeed(100.f)
	, bisRight(true)
{
	// ���ҽ� �ε� - Texture

	wstring resPath = PathMgr::GetInst()->GetContentPath();
	resPath += L"Textures\\Player";
	// ĳ���� ���� ���� Ȯ��
	/*if (!wcscmp(character, "wilson"))
	{
		resPath += L"\\Wilson";
	}*/
	CreateAnimator();
	GenerateAnimations(resPath);
	MessageBox(nullptr, (L"Player Resource has Loaded"), L"Notice", MB_OK);
	SetCurrDir(L"Down");
	wstring dir = GetCurrDir();
	GetAnimator()->Play(ObjType::Player, L"Attack", true, dir);
	ResTex* pTex = GetAnimator()->FindAnimation(L"Player_Attack_Down")->GetFrame(0);
	//GetAnimation()->GetFrame()
	Image* img = pTex->GetImage();
	float width = (float)img->GetWidth();
	float height= (float)img->GetHeight();
	Vec2D baseScale(width, height);
	SetPos(Vec2D(300.f, 300.f));
	SetScale(baseScale);

	Vec2D reScale(0.8f, 0.8f);
	Vec2D colSize = GetScale();
	
	CreateCollider();
	GetCollider()->SetOffsetPos(colSize/Vec2D(2, 2));

	colSize = colSize * reScale;
	GetCollider()->SetColliderSize(colSize);

	//MessageBox(nullptr, (L"All Resource has Loaded"), L"Notice", MB_OK);

}

Player::~Player()
{
	
}

void Player::GenerateAnimations(wstring& TexPath)
{
	vector<ResTex*> frames;
	wstring animationName = L"";

	GetAnimations(frames, animationName, TexPath);
	//GetAnimations(TexPath);
}
void Player::GetAnimations(vector<ResTex*> frames, wstring& animationName, wstring& TexPath)
//void Player::GetAnimations(wstring& TexPath)
{
	WIN32_FIND_DATA findDir;
	HANDLE hFind = FindFirstFile((TexPath + L"\\*").c_str(), &findDir);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		// �δ� ��θ� ���� �߻�
		MessageBox(nullptr, (L"Cannot find Dir" + TexPath + L" Error :: Invalid Path.").c_str(), L"Error", MB_OK);
		return;
	}

	bool isEmpty = true;

	// ���� ����� ������ �������� �ִϸ��̼� �̸����� ���
	
	do {
		if (wcscmp(findDir.cFileName, L".") != 0 && wcscmp(findDir.cFileName, L"..") != 0)
		{
			isEmpty = false;
			wstring fullPath = TexPath + L"\\" + findDir.cFileName;

			if (findDir.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// ���� ������ �ִ� ��� ��� ȣ��
				GetAnimations(frames, animationName, fullPath);
				//GetAnimations(fullPath);
			}
			else if (wstring(findDir.cFileName).find(L".png") != wstring::npos)
			{
				// ���� ������ ����, png������ �ִٸ�
				// ���� ������ �ε��ؼ� ���� ���������� ����Ͽ� �ִϸ��̼� ����
				// �ִϸ��̼� ����
				// PNG ������ �ε��Ͽ� ���������� �߰�
				wstring fileName = findDir.cFileName;

				// Ȯ���� ����
				animationName = fileName.substr(0, fileName.find_last_of(L'_'));

				//animationName = TexPath.substr(TexPath.find_last_of(L"\\") + 1);
				ResTex* pTex = ResMgr::GetInst()->LoadTex(fileName, fullPath);
				if (pTex)
				{
					frames.push_back(pTex);
					//MessageBox(nullptr, (fileName + L" is loaded " + std::to_wstring(frames.size()) + L" frames").c_str(), L"Notice", MB_OK);
				}

				// ���� �� �Ѱ��� ���Ͽ��� �������� �߶󳻾� �ִϸ��̼� ����
				//ResTex* pTex = ResMgr::GetInst()->LoadTex(animationName + L"_" + findDir.cFileName, fullPath);

				//if (pTex)
				//{
				//	// �ִϸ��̼� ����: ������ �Ӽ� ���� �� ���
				//	GetAnimator()->CreateAnimation(animationName, pTex, Vec2D(0.f, 0.f), Vec2D(100.f, 100.f), Vec2D(100.f, 0.f), 0.1f, 1);
				//}
			}
		}
	} while (FindNextFile(hFind, &findDir));

	// �ִϸ��̼� ����
	if (!frames.empty())
	{
		double sec = 1;
		double frameTime = (frames.size() / (double)1000 * sec);	// size = deltatime * sec;

		GetAnimator()->CreateAnimation(animationName, frames, frameTime);
		//MessageBox(nullptr, (animationName + L" is loaded " + std::to_wstring(frames.size()) + L" frames").c_str(), L"Notice", MB_OK);
		frames.clear();
		animationName = L"";
	}
	
	if (isEmpty)
	{
		// ������ ������ ������ �����̳� ������ ���� ���
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

	//���ҽ� �����ε��� ��Ծ��� �ڵ�

	Vec2D vPlayerPos = GetPos();
	Vec2D vPlayerScale = GetScale();
	/*UINT iWidth = 20;
	UINT iHeight = 20;*/
	//MessageBox(nullptr, (L"Before Call Anim Render"), L"Notice", MB_OK);
	HPEN hOldPen = (HPEN)CreatePen(PS_SOLID, 5, RGB(0, 0, 255));
	SelectObject(hdc, hOldPen);
	Rectangle(hdc, vPlayerPos.x
		, vPlayerPos.y
		, vPlayerPos.x + vPlayerScale.x
		, vPlayerPos.y + vPlayerScale.y);
	DeleteObject(hOldPen);
	
	//GetAnimator()->render(hdc);


	// �ִϸ��̼� ������

	componentRender(hdc);
}