#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
#include <atlimage.h>
#include "object.h"
HINSTANCE hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

CImage Bullbasaur::img[BULL_SPRITES], MyBackGround::img;
int Bullbasaur::imageW[BULL_SPRITES], Bullbasaur::imageH[BULL_SPRITES];
int MyBackGround::imageW, MyBackGround::imageH;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static HDC hdc, memdc;
	static HBITMAP hBit;
	static MyObject* obj[10];
	static int cntObj;
	static RECT rectView;
	TCHAR word[] = _T("대한민국 화이팅");
	//--- 메시지 처리하기
	switch (uMsg) {
	case WM_CREATE:
		GetClientRect(hWnd, &rectView);

		MyBackGround::SetImage(L".\\bitmap2.bmp");
		Bullbasaur::SetImage(L".\\image\\O");

		obj[cntObj++] = new MyBackGround(hWnd);
		obj[cntObj++] = new Bullbasaur(100, 100, 30, 30);
		SetTimer(hWnd, 1, 100, NULL);
		break;
	case WM_TIMER:
		InvalidateRgn(hWnd, NULL, FALSE);
	case WM_PAINT:
		GetClientRect(hWnd, &rectView);
		hdc = BeginPaint(hWnd, &ps);
		hBit = CreateCompatibleBitmap(hdc, rectView.right, rectView.bottom);
		memdc = CreateCompatibleDC(hdc);
		(HBITMAP)SelectObject(memdc, hBit);
		::SetStretchBltMode(memdc, HALFTONE);
		for (int i = 0; i < cntObj; ++i)
			obj[i]->Draw(memdc);
		BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, memdc, 0, 0, SRCCOPY);
		DeleteDC(memdc);
		EndPaint(hWnd, &ps);

		break;
	case WM_DESTROY:
		if (hBit) DeleteObject(hBit);
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}