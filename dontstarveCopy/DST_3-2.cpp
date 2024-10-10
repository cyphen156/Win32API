// dontstarveCopy.cpp : ���ø����̼ǿ� ���� �������� �����մϴ�.
//
#include "pch.h"
#include "framework.h"
#include "dontstarveCopy.h"

#define MAX_LOADSTRING 100
#define WSREGULAR 5000
#define WSLARGE 10000

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
HWND hWnd;                                      // ���� ������
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// Ŀ���� ��������
RECT worldSize;
RECT windowInfo;
POINT charWorldPos;
POINT charWinPos;
DWORD lastFrameTime = 0;

// Ǯ��ũ�� ������ Ű ����
BOOL isFScreen;

// ������ Ƚ�� ī��Ʈ�� ����
int iRenderCnt = 0;
DWORD lastRenderTime = 0;

// �� �ڵ� ��⿡ ���Ե� �Լ��� ������ �����մϴ�:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// ���� ũ�� ����
void SetWorldSize(bool isLarge);

// ȭ�鿡 �������ϱ�
void Rendering();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DONTSTARVECOPY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���ø����̼� �ʱ�ȭ�� �����մϴ�:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DONTSTARVECOPY));

    MSG msg;

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.
    // Ŀ���� �������� �ʱ�ȭ
    SetWorldSize(true);  // true�� Large, false�� Regular
    GetWindowRect(hWnd, &windowInfo);   // ���ø����̼� ����� ������ ������ ��������
    charWinPos = { (windowInfo.bottom - windowInfo.top) / 2, (windowInfo.right - windowInfo.left) / 2 };              // �⺻ ��ǥ : ȭ�����߾�
    charWorldPos = { (worldSize.bottom - worldSize.top) / 2, (worldSize.right - worldSize.left) / 2 };            // �⺻ ��ǥ : ���� ���߾�

    // Ǯ��ũ�� ���� �ʱ�ȭ
    isFScreen = true;


    // �޼��� ����
    while (1)
    {
        // ����� �Է� ó��
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        // �б⿡ �ɸ��� �ʴ� ��׶��� ���μ��� �ۼ�
        else
        {
            Rendering();

            // ���� �ð� ������
            DWORD currentTime = GetTickCount64();

            // ������ ����Ʈ ���
            if (currentTime - lastRenderTime >= 1000)
            {
                HDC hdc = GetDC(hWnd);
                TCHAR fps[10];
                wsprintf(fps, L"FPS: %d", iRenderCnt);
                TextOut(hdc, 10, 10, fps, lstrlen(fps));
                ReleaseDC(hWnd, hdc);

                iRenderCnt = 0;
                lastRenderTime = currentTime;
            }
        }
    }

    return (int)msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  �뵵: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DONTSTARVECOPY));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(NULL); // �޴� ON : IDC_DONTSTARVECOPY
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   �뵵: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   �ּ�:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }
    // Ǯ��ũ�� ���� ����
    SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    SetWindowPos(hWnd, HWND_TOP, 0, 0, screenWidth, screenHeight, SWP_NOZORDER | SWP_FRAMECHANGED);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �뵵: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���ø����̼� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // �޴� ������ ���� �м��մϴ�:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_KEYDOWN:
        if (wParam == VK_F11)
        {
            if (isFScreen) {
                // â ���� ��ȯ (HD : 1280 X 720)
                SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
                SetWindowPos(hWnd, HWND_TOP, 100, 100, 800, 600, SWP_NOZORDER | SWP_FRAMECHANGED);
                isFScreen = false;
            }
            else {
                // Ǯ��ũ�� ���� ��ȯ
                SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
                int screenWidth = GetSystemMetrics(SM_CXSCREEN);
                int screenHeight = GetSystemMetrics(SM_CYSCREEN);
                SetWindowPos(hWnd, HWND_TOP, 0, 0, screenWidth, screenHeight, SWP_NOZORDER | SWP_FRAMECHANGED);
                isFScreen = true;
            }
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


void SetWorldSize(bool isLarge) {
    if (isLarge) {
        worldSize = { 0, 0, WSLARGE, WSLARGE };
    }
    else {
        worldSize = { 0, 0, WSREGULAR, WSREGULAR };
    }
}

void Rendering()
{
    ++iRenderCnt;
}
