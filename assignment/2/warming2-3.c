#define _CRT_SECURE_NO_WARNINGS
#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);



int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpszCmdParam, _In_ int nCmdShow)
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
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 200, 150, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);
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
	srand(time(NULL));
	static int x, y, count;
	static TCHAR n;
	static TCHAR str[101];
	static RECT rect;
	//--- 메시지 처리하기
	switch (uMsg) {
	case WM_CREATE:
		rect.left = rand() % 701;
		rect.top = rand() % 501;
		rect.right = rect.left+100;
		rect.bottom = rect.top+200;
		n = rand() % 10 + '0';
		count = rand() % 81 + 20;

		for (int i = 0; i < count; ++i)
			str[i] = n;
		str[count] = '\0';

		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		SetBkColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
		SetTextColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
		DrawText(hDC, str, -1, &rect, DT_WORDBREAK | DT_EDITCONTROL);
		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}