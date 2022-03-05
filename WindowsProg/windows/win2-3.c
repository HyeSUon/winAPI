#define _CRT_SECURE_NO_WARNINGS
#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
#include <stdio.h>
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows program 1-1";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	static TCHAR str[100];
	static int count = 0;
	static int yPos;
	static int x, y, n, scnt = 0;
	static SIZE size;
	// 메시지 처리하기
	switch (uMsg) {
	case WM_CREATE:
		CreateCaret(hWnd, NULL, 5, 15);
		ShowCaret(hWnd);
		count = 0;
		scanf("%d %d %d", &x, &y, &n);
		break;
	case WM_KEYDOWN:
		hDC = GetDC(hWnd);
		if (wParam == VK_BACK)
			count--;
		else if (wParam == VK_RETURN) {
			count = 0;
			yPos += 20;
			scnt = 0;
		}
		else
			str[count++] = wParam;
		str[count] = '\0';
		
		InvalidateRect(hWnd, NULL, TRUE);
		ReleaseDC(hWnd, hDC);
		break;
	case WM_CHAR:
		hDC = GetDC(hWnd);
		ReleaseDC(hWnd, hDC);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		GetTextExtentPoint32(hDC, str, wcslen(str), &size);
		TextOut(hDC, 0, yPos, str, wcslen(str));
		SetCaretPos(size.cx, yPos);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); // 위의 세 메시지 외의 나머지 메시지는 OS로
}