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
	static TCHAR str[10][81];
	static int row, col;
	static int a;
	static SIZE size;
	static BOOL operStr, insertKey;
	int len; TCHAR temp;
	//--- 메시지 처리하기
	switch (uMsg) {
	case WM_CREATE:
		CreateCaret(hWnd, NULL, 1, 15);
		ShowCaret(hWnd);
		operStr = FALSE; insertKey = FALSE;
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		GetTextExtentPoint32(hDC, str[row], col, &size);
		for (int i = 0; i < 10; ++i) {
			TextOut(hDC, 0, i*20, str[i], wcslen(str[i]));
		}
		SetCaretPos(size.cx, row*20);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		len = wcslen(str[row]);
		if (wParam == VK_F1)
			operStr = !operStr;
		else if (wParam == VK_RIGHT) {
			if(len > col)
				col++;
			if (len == col) {
				if (row < 9) {
					row++;
					col = 0;
				}
			}
		}
		else if (wParam == VK_LEFT) {
			if(col > 0)
				col--;
			if (col == 0) {
				if (row > 0) {
					row--;
					col = wcslen(str[row]);
				}
			}
		}
		else if (wParam == VK_UP) {
			if (row > 0) {
				row--;
				len = wcslen(str[row]);
				if (col > len)
					col = len;
			}
		}
		else if (wParam == VK_DOWN) {
			if (row < 9) {
				row++;
				len = wcslen(str[row]);
				if (col > len)
					col = len;
			}
		}
		else if (wParam == VK_DELETE) {
			// 단어 삭제
		}
		else if (wParam == VK_HOME) {
			insertKey = TRUE;
			col = 0;
		}
		else if (wParam == VK_END) {
			col = len;
			
		}
		else if (wParam == VK_INSERT) {
			insertKey = !insertKey;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_CHAR:
		
		len = wcslen(str[row]);
		if (wParam == VK_RETURN) {
			if (row < 9) {
				row++; col = 0;
			}
		}
		else if (wParam == VK_BACK) {
			if (col > 0) {
				if (col-- != len) { // len이 더 큰경우
					for (int i = col; i <= len; ++i) {
						str[row][i] = str[row][i + 1];
					}
				}
				else
					str[row][col] = '\0';
			}
			else {
				if (row > 0) {
					row--;
					col = wcslen(str[row]);
				}
			}
		}
		else if (wParam == VK_TAB) {
			if (len < 80-4) {
				if (insertKey) {
					for (int i = 0; i < 4; ++i)
						str[row][col++] = ' ';
					if (col >= len)
						str[row][col] = '\0';
				}
				else {
					len = wcslen(str[row]);
					for (int i = len+4; i >= col; --i) {
						str[row][i] = str[row][i - 4];
					}
					for (int i = 0; i < 4; ++i)
						str[row][col++] = ' ';
				}
			}
			else {
				if (row < 9)
					row++;
				col = wcslen(str[row]);
			}
		}
		else if (wParam == VK_ESCAPE) {
			for (int i = 0; i < 10; ++i) {
				str[i][0] = '\0';
				row = 0; col = 0;
			}
		}
		else {
			if (len < 80) {
				if (insertKey) {
					;
				}
				else {
					if (col != len) { // len이 더 큰경우
						for (int i = len; i >= col; --i) {
							str[row][i] = str[row][i - 1];
						}
					}
				}

				if (operStr) {
					if (wParam >= 'a' && wParam <= 'z')
						str[row][col++] = wParam - 32;
				}
				else {
					str[row][col++] = wParam;
					if(col-1 == len)
						str[row][col] = '\0';
				}
			}
			else {
				if(row < 9)
					row++;
				col = wcslen(str[row]);
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}

