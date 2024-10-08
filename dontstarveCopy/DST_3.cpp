// dontstarveCopy.cpp : ���ø����̼ǿ� ���� �������� �����մϴ�.
//

#include "framework.h"
#include "dontstarveCopy.h"
#include "Gdiplus.h"  
#pragma comment (lib, "gdiplus")

using namespace Gdiplus;

static HWND hWnd;

// GDIPlus ��ū �ʱ�ȭ
ULONG_PTR gdiplusToken;


// ��ǥ ���� 
static POINT point;
static BOOL reverse;
static POINT endPos;

static int scaledWidth;
static int scaledHeight;
#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ���Ե� �Լ��� ������ �����մϴ�:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HDC MemDC;
HBITMAP hBitmap, oldBitmap;
HBITMAP hBitmaps[5];

BOOL changedhWndSize;

static RECT hWndInfo;

static unsigned int iFrame = 1;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // GDI+ �ʱ�ȭ
    GdiplusStartupInput gpsi;
    GdiplusStartup(&gdiplusToken, &gpsi, nullptr);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    // TODO: ���⿡ �ڵ带 �Է��մϴ�.
    // 
    // 

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DONTSTARVECOPY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���ø����̼� �ʱ�ȭ�� �����մϴ�:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }
    // �⺻ ��ǥ ����
    point = { 0, 0 };

    // �̹��� ���� ����
    reverse = true;

    changedhWndSize = false;

    // ��Ʈ�� �ε�
    // 

    /*hBitmaps[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
    hBitmaps[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
    hBitmaps[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
    hBitmaps[3] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));
    hBitmaps[4] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5));*/


    /*for (int i = 0; i < 5; ++i)
    {
        if (hBitmaps[i] == NULL) {
            MessageBox(NULL, L"��Ʈ���� �ε��� �� �����ϴ�.", L"Error", MB_OK);
        }
    }*/
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DONTSTARVECOPY));

    MSG msg;


    // �޼��� ����
    while (1)
    {
        // ����� �Է� ó��
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
            {
                break;
            }
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        // �б⿡ �ɸ��� �ʴ� ��׶��� ���μ��� �ۼ�
        else
        {
            if (reverse)
            {
                if (point.x + scaledWidth < endPos.x)
                {
                    ++point.x;
                }
                if (point.y + scaledHeight < endPos.y - (scaledHeight / 2))
                {
                    ++point.y;
                }
            }
            else if (!reverse)
            {
                if (point.x > 0)
                    --point.x;
                if (point.y > 0)
                    --point.y;
            }
            InvalidateRect(hWnd, NULL, FALSE);
        }
    }

    // GDI Plus ����
    GdiplusShutdown(gdiplusToken);

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
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DONTSTARVECOPY);
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
    case WM_SIZE:
        // ������ ũ�� ���� �� RECT �� ������Ʈ
        GetWindowRect(hWnd, &hWndInfo);
        endPos.x = hWndInfo.right - hWndInfo.left;
        endPos.y = hWndInfo.bottom - hWndInfo.top;
        changedhWndSize = true;
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // MemDC�� ó���� ���� ����
        if (MemDC == NULL) {
            MemDC = CreateCompatibleDC(hdc);
            HBITMAP hMemBitmap = CreateCompatibleBitmap(hdc, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top);
            oldBitmap = (HBITMAP)SelectObject(MemDC, hMemBitmap);
        }
        else if (changedhWndSize)
        {
            MemDC = CreateCompatibleDC(hdc);
            HBITMAP hMemBitmap = CreateCompatibleBitmap(hdc, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top);
            oldBitmap = (HBITMAP)SelectObject(MemDC, hMemBitmap);
            changedhWndSize = false;
        }

        // �� ����(MemDC)�� ��� �׸���
        FillRect(MemDC, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        // �̸� �ε�� ��Ʈ���� �� ���ۿ� �׸���
        hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1 + (iFrame / 5)));
        HDC hTempDC = CreateCompatibleDC(MemDC);
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hTempDC, hBitmap);

        BITMAP bitmap;
        GetObject(hBitmap, sizeof(BITMAP), &bitmap);

        int originalWidth = bitmap.bmWidth;
        int originalHeight = bitmap.bmHeight;
        scaledWidth = originalWidth / 10;
        scaledHeight = originalHeight / 10;

        // ��Ʈ���� �� ���ۿ� �׸���
        if (reverse) {
            StretchBlt(MemDC, point.x + scaledWidth, point.y, -scaledWidth, scaledHeight, hTempDC, 0, 0, originalWidth, originalHeight, SRCCOPY);
        }
        else {
            StretchBlt(MemDC, point.x, point.y, scaledWidth, scaledHeight, hTempDC, 0, 0, originalWidth, originalHeight, SRCCOPY);
        }

        // �� ����(MemDC) ������ ȭ��(hdc)�� ����
        BitBlt(hdc, 0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, MemDC, 0, 0, SRCCOPY);

        // �ڿ� ����
        SelectObject(hTempDC, hOldBitmap);
        DeleteDC(hTempDC);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_LEFT:
        {
            reverse = false;
            point.x -= 5;
            if (iFrame > 1)
            {
                --iFrame;
            }
            break;
        }
        case VK_RIGHT:
        {
            reverse = true;
            point.x += 5;
            if (iFrame < 24)
            {
                ++iFrame;
            }
            break;
        }
        case VK_UP:
            point.y -= 5;
            break;
        case VK_DOWN:
            point.y += 5;
            break;
        }
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_DESTROY:
        SelectObject(MemDC, oldBitmap);
        for (int i = 0; i < 5; ++i) {
            DeleteObject(hBitmaps[i]);
        }
        DeleteDC(MemDC);
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
