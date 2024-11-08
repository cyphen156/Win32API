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
	// ���ҽ� �ε� - Texture
	

	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2D(0.f, 0.f));
	GetCollider()->SetColliderSize(Vec2D(100.f, 100.f));

	wstring resPath = PathMgr::GetInst()->GetContentPath();
	resPath += L"Textures\\Player";
	// ĳ���� ���� ���� Ȯ��
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
				GetAnimations(fullPath);
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
		GetAnimator()->CreateAnimation(animationName, frames, 0.5f); // �� �������� ���� �ð� 0.1��
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

	UINT iWidth = 20;
	UINT iHeight = 20;
	//MessageBox(nullptr, (L"Before Call Anim Render"), L"Notice", MB_OK);


	GetAnimator()->render(hdc);

	/*Rectangle(hdc, 50, 50, iWidth, iHeight);*/

	// �ִϸ��̼� ������

	componentRender(hdc);
}