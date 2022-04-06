#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>

#define STR_SIZE 30
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
	static TCHAR str[10][STR_SIZE+1];
	static int row, col;
	static BOOL insert;
	static SIZE size;
	//--- 메시지 처리하기
	switch (uMsg) {
	case WM_CREATE:
		CreateCaret(hWnd, NULL, 1, 15);
		ShowCaret(hWnd);
		insert = FALSE;
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		if(insert)
			GetTextExtentPoint32(hDC, str[row], col, &size);
		else
			GetTextExtentPoint32(hDC, str[row], wcslen(str[row]), &size);
		for (int i = 0; i < 10; ++i) {
			TextOut(hDC, 0, 20 * i, str[i], wcslen(str[i]));
		}
		SetCaretPos(size.cx, row*20);
		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		if (wParam == VK_RETURN) {
			if (row < 9) {
				row++;
				col = 0;
			}
		}
		else if (wParam == VK_BACK) {
			if (col > 0) {
				str[row][--col] = '\0';
			}
			else if (row > 0) {
				col = wcslen(str[--row]);
			}
		}
		else if(col < STR_SIZE) {
			str[row][col++] = wParam;
			if (insert == FALSE || wcslen(str[row]) <= col ) {
				str[row][col] = '\0';
			}
		}
		else if (row < 9) {
			col = 0;
			str[++row][col++] = wParam;
			str[row][col] = '\0';
		}
		else {
			row = 0;
			col = 0;
			insert = TRUE;
		}
		
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}