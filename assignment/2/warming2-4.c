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
	static TCHAR str[100];
	static SIZE size, size2;
	static int count;
	static int spaceCnt;
	static LPTSTR token, ptr1;
	static int x, y, n;
	static TCHAR strN[3];
	static TCHAR result[150];
	static TCHAR Out[30];
	static int cnt;
	static RECT rect;
	//--- 메시지 처리하기
	switch (uMsg) {
	case WM_CREATE:
		CreateCaret(hWnd, NULL, 5, 15);
		ShowCaret(hWnd);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		GetTextExtentPoint32(hDC, str, wcslen(str), &size);
		TextOut(hDC, 0, 0, str, wcslen(str));
		SetCaretPos(size.cx, 0);
		

		if (spaceCnt == 3) {
			GetTextExtentPoint32(hDC, result, 40, &size2);
			rect.right = x + size2.cx;
			DrawText(hDC, result, -1, &rect, DT_WORDBREAK | DT_EDITCONTROL);
			spaceCnt = 0;
			result[0] = '\0';
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		if (wParam == VK_SPACE || wParam == VK_RETURN)
			spaceCnt++;
		if (wParam == VK_BACK) {
			if(count > 0)
				count--;
			if (str[count] == ' ')
				spaceCnt--;
			str[count] = '\0';
		}
		else {
			str[count++] = wParam;
			str[count] = '\0';
		}

		if (spaceCnt == 3) {
			token = wcstok(str, L" \r", ptr1);
			x = _wtoi(token);
			token = wcstok(NULL, L" \r", ptr1);
			y = _wtoi(token);
			token = wcstok(NULL, L" \r", ptr1);
			wcscpy(strN, token);
			n = _wtoi(token);

			if (n < 0 || n > 19)
				PostQuitMessage(0);

			for (int i = 1; i <= 9; ++i) {
				wsprintf(Out, L"%d*%d=%d ", n, i, n * i);
				wcscat(result, Out);
			}
			rect.left = x;
			rect.top = y;
			rect.bottom = y + 100;
			str[0] = '\0';
			count = 0;
		}
		else if(wParam == VK_RETURN) {
			if (str[0] == '0')
				PostQuitMessage(0);
			spaceCnt = 0;
			count = 0;
			str[count] = '\0';
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