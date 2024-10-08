#pragma once
#include <windows.h>

class Core {
public:
    Core();  // ������
    ~Core(); // �Ҹ���

    void Initialize(HWND hWnd);  // �ʱ�ȭ �Լ�
    void Run();                  // ���� ���� ����
    void RenderFrame();           // ������ ������

private:
    HWND hWnd;
    DWORD lastFrameTime;          // ������ ������ �ð�
    static const DWORD FRAME_DURATION = 1000 / 60;  // 60FPS ����
};
