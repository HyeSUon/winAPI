#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
#include "object.h"




HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";
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
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 100, 0, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);
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

	static ObjectManager om;
	static int mx, my;
	static BOOL Selection;
	static BOOL running;
	static BOOL Text;
	static TCHAR lpOut[2][30];
	//--- 메시지 처리하기
	switch (uMsg) {
	case WM_CREATE:
		GetClientRect(hWnd, &om.rClient);
		InitObject(&om);
		SetTimer(hWnd, 1, 10, NULL);
		break;
	case WM_TIMER:
		hDC = GetDC(hWnd);
		switch (wParam) {
		case 1:
			if (running) {
				if(InRectangle(om.rClient, om.ball.x, om.ball.y))
					InFingerSnap(&om.ball, &om.rClient, TRUE, TRUE, TRUE, FALSE);
				if (InRectangle(om.stick.r, om.ball.x, om.ball.y))
					;
				else {
					OutFingerSnap(&om.ball, &om.stick, FALSE, TRUE, FALSE, FALSE);
				}
				running = FallBall(&om.ball, &om.stick, om.rClient); // fall > ball stop
				CollideBlock(om.block, &om.ball, &om.brokenBlock, &om.smallBlock);
				MoveBall(&om.ball);
				MoveBlocks(&om.block, &om.isLeftBlock, om.rClient);
			}
			break;
		}
		ReleaseDC(hWnd, hDC);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &om.rClient);
		DrawStick(hDC, &om.stick);
		DrawBlocks(hDC, &om.block);

		DrawBall(hDC, &om.ball);

		if (Text) {
			wsprintf(lpOut[0], L"색이 변한 벽돌의 개수: %d", om.smallBlock);
			wsprintf(lpOut[1], L"깨진 벽돌의 개수: %d", om.brokenBlock);
			TextOut(hDC, 300, 300, lpOut[0], wcslen(lpOut[0]));
			TextOut(hDC, 300, 320, lpOut[1], wcslen(lpOut[1]));
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		switch (wParam) {
		case 's':
			running = TRUE;
			Text = FALSE;
			break;
		case 'p':
			running = !running;
			Text = FALSE;
			if (!running)
				Text = TRUE;
			break;
		case '+':
			om.ball.a++;
			break;
		case '-':
			if(om.ball.a > 1)
				om.ball.a--;
			break;
		case 'n':
			memset(&om, 0, sizeof(ObjectManager));
			GetClientRect(hWnd, &om.rClient);
			InitObject(&om);
			running = FALSE;
			break;
		case 'q':
			PostQuitMessage(0);
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (InRectangle(om.stick.r, mx, my)) {
			Selection = TRUE;
		}
		break;
	case WM_LBUTTONUP:
		Selection = FALSE;
		break;
	case WM_MOUSEMOVE:
		hDC = GetDC(hWnd);
		if (running) {
			if (Selection) {
				DrawROPStick(hDC,lParam, &om.stick);
			}
		}
		ReleaseDC(hWnd, hDC);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}