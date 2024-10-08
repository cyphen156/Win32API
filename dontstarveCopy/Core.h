#pragma once
#include <windows.h>

class Core {
public:
    Core();  // 생성자
    ~Core(); // 소멸자

    void Initialize(HWND hWnd);  // 초기화 함수
    void Run();                  // 메인 루프 제어
    void RenderFrame();           // 프레임 렌더링

private:
    HWND hWnd;
    DWORD lastFrameTime;          // 마지막 프레임 시간
    static const DWORD FRAME_DURATION = 1000 / 60;  // 60FPS 설정
};
