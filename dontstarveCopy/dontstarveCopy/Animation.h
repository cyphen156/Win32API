#pragma once
#include "Struct.h"

//struct tAnimaFrame
//{
//	//Vec2D vLT;				// left-top
//	//Vec2D vSlice;			// slicing img
//	float fDuration;		// time
//	Vec2D m_vOffset;		// Offset
//};

class Animator;
class ResTex;

class Animation
{
private:
    wstring m_strName;
    Animator* m_pAnimator;
    vector<ResTex*> m_vecFrames; // �� �������� �����ϴ� ����
    int m_iCurFrame;
    float m_fAccTime;
    float m_fFrameDuration; // �������� ���� �ð�
    bool m_bFinish;

    void SetName(const wstring& name) { m_strName = name; }

public:
    Animation();
    ~Animation();
    const wstring& GetName() { return m_strName; }
    bool IsFinish() { return m_bFinish; }
    void SetFrame(int iFrameIDX)
    {
        m_bFinish = false;
        m_iCurFrame = iFrameIDX;
        m_fAccTime = 0.f;
    }

    ResTex* GetFrame(int iIDX) { return (iIDX >= 0 && iIDX < m_vecFrames.size()) ? m_vecFrames[iIDX] : nullptr; }
    void Create(vector<ResTex*> frames, float fDuration);
    int GetMaxFrame() { return (int)m_vecFrames.size(); }
    void update();
    void render(HDC hdc);

    friend class Animator;
};
