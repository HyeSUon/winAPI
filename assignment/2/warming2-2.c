#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
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
	static TCHAR str[1001];
	static TCHAR str2[1001];
	//--- 메시지 처리하기
	static RECT r;
	int centerX;
	int centerY;
	srand(time(NULL));

	switch (uMsg) {
	case WM_CREATE:
		for (int i = 0; i < 1000; ++i) {
			str[i] = 'a' + i % ('z' - 'a' + 1);
			str2[i] = 'A' + i % ('Z' - 'A' + 1);
		}
		str[1000] = '\0';
		str2[1000] = '\0';
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &r);
		centerX = r.right / 2;
		centerY = r.bottom / 2;

		//top l-r
		RECT rect = { 0, 0, centerX, centerY };
		SetBkColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
		SetTextColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
		DrawText(hDC, str, -1, &rect, DT_LEFT|DT_WORDBREAK | DT_EDITCONTROL);

		RECT rect2 = { centerX, 0, r.right, centerY }; 
		SetBkColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
		SetTextColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
		DrawText(hDC, str2, -1, &rect2, DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL);

		//bottom l-r
		RECT rect3 = { 0, centerY, centerX, r.bottom };
		SetBkColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
		SetTextColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
		DrawText(hDC, str2, -1, &rect3, DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL);

		RECT rect4 = { centerX, centerY, r.right, r.bottom };
		SetBkColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
		SetTextColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
		DrawText(hDC, str, -1, &rect4, DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL);
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

