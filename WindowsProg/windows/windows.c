#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
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
	TCHAR text[30];
	int x[5] = { 0, 700, 0, 700, 350 };
	int y[5] = { 0, 0, 650, 650, 300 };
	
	// 메시지 처리하기
	switch (uMsg) {
	case WM_CREATE:
		
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		SetTextColor(hDC, RGB(200, 0, 0));
		SetBkColor(hDC, RGB(0, 200, 0));
		wsprintf(text, L"Left-top(%d, %d)", x[0], y[0]);
		DrawText(hDC, text, wcslen(text), &(RECT){ x[0], y[0], x[0]+100, x[0]+30 }, DT_LEFT);

		SetTextColor(hDC, RGB(0, 0, 200));
		SetBkColor(hDC, RGB(0, 200, 200));
		wsprintf(text, L"Right-top(%d, %d)", x[1], y[1]);
		DrawText(hDC, text, wcslen(text), &(RECT){ x[1]-20, y[1], x[1]+100, y[1]+30}, DT_LEFT);

		SetTextColor(hDC, RGB(200, 200, 0));
		SetBkColor(hDC, RGB(100, 100, 100));
		wsprintf(text, L"Left-bottom(%d, %d)", x[2], y[2]);
		DrawText(hDC, text, wcslen(text), &(RECT){ x[2], y[2]-105, x[2]+150, y[2]+30}, DT_LEFT);

		SetTextColor(hDC, RGB(200, 0, 0));
		SetBkColor(hDC, RGB(200, 200, 0));
		wsprintf(text, L"Right-bottom(%d, %d)", x[3], y[3]);
		DrawText(hDC, text, wcslen(text), &(RECT){ x[3]-60, y[3]-105, x[3]+100, y[3]+30}, DT_LEFT);
		
		SetTextColor(hDC, RGB(200, 0, 200));
		SetBkColor(hDC, RGB(100, 100, 100));
		wsprintf(text, L"Center(%d, %d)", x[4], y[4]);
		DrawText(hDC, text, wcslen(text), &(RECT){ 0, 0, 800, 600}, DT_SINGLELINE|DT_CENTER|DT_VCENTER);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); // 위의 세 메시지 외의 나머지 메시지는 OS로
}