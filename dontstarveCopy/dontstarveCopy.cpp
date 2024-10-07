// dontstarveCopy.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "dontstarveCopy.h"
#include "Gdiplus.h"  
#pragma comment (lib, "gdiplus")

using namespace Gdiplus;

// GDIPlus 토큰 초기화
ULONG_PTR gdiplusToken;


// 좌표 변수 
static POINT point;
static BOOL reverse;
#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HDC MemDC;
HBITMAP hBitmap, oldBitmap;
HBITMAP hBitmaps[5];

static unsigned int iFrame = 1;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    // GDI+ 초기화
    GdiplusStartupInput gpsi;
    GdiplusStartup(&gdiplusToken, &gpsi, nullptr);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    // TODO: 여기에 코드를 입력합니다.
    // 
    // 
    // 기본 좌표 설정
    point = { 0, 0 };

    // 이미지 역전 설정
    reverse = true;

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DONTSTARVECOPY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    // 비트맵 로드
    // 

    /*hBitmaps[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
    hBitmaps[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
    hBitmaps[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
    hBitmaps[3] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));
    hBitmaps[4] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5));*/


    /*for (int i = 0; i < 5; ++i)
    {
        if (hBitmaps[i] == NULL) {
            MessageBox(NULL, L"비트맵을 로드할 수 없습니다.", L"Error", MB_OK);
        }
    }*/
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DONTSTARVECOPY));

    MSG msg;


    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // GDI Plus 해제
    GdiplusShutdown(gdiplusToken);

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DONTSTARVECOPY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DONTSTARVECOPY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
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
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            // MemDC가 처음일 때만 생성
            if (MemDC == NULL) {
                MemDC = CreateCompatibleDC(hdc);
                HBITMAP hMemBitmap = CreateCompatibleBitmap(hdc, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top);
                oldBitmap = (HBITMAP)SelectObject(MemDC, hMemBitmap);
            }

            // 백 버퍼(MemDC)에 배경 그리기
            FillRect(MemDC, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

            // 미리 로드된 비트맵을 백 버퍼에 그리기
            hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1 + (iFrame / 5)));
            HDC hTempDC = CreateCompatibleDC(MemDC);
            HBITMAP hOldBitmap = (HBITMAP)SelectObject(hTempDC, hBitmap);

            BITMAP bitmap;
            GetObject(hBitmap, sizeof(BITMAP), &bitmap);

            int originalWidth = bitmap.bmWidth;
            int originalHeight = bitmap.bmHeight;
            int scaledWidth = originalWidth / 10;
            int scaledHeight = originalHeight / 10;

            // 비트맵을 백 버퍼에 그리기
            if (reverse) {
                StretchBlt(MemDC, point.x + scaledWidth, point.y, -scaledWidth, scaledHeight, hTempDC, 0, 0, originalWidth, originalHeight, SRCCOPY);
            }
            else {
                StretchBlt(MemDC, point.x, point.y, scaledWidth, scaledHeight, hTempDC, 0, 0, originalWidth, originalHeight, SRCCOPY);
            }

            // 백 버퍼(MemDC) 내용을 화면(hdc)에 복사
            BitBlt(hdc, 0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, MemDC, 0, 0, SRCCOPY);

            // 자원 해제
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

// 정보 대화 상자의 메시지 처리기입니다.
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
