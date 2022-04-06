#define _CRT_SECURE_NO_WARNINGS
#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>

#define SIZE 20

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
	return (int)Message.wParam;
}


typedef struct {
	int shape; int x1; int y1; int x2; int y2; int t; // t 펜의 두께
	COLORREF cBrush; COLORREF cPen;
}Data;
void DrawShape(HDC hDC, const Data* d);
void ControlShape(TCHAR, Data* data);


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	static RECT r;
	static TCHAR str[SIZE + 1] = { 0 };
	static int cntStr;
	static int spaceCnt;
	static Data data[10];
	static int dataCnt, maxDataCnt;
	static int a;
	srand(time(NULL));
	//--- 메시지 처리하기
	switch (uMsg) {
	case WM_CREATE:
		cntStr = 0;
		dataCnt = 0;
		maxDataCnt = 0;
		a = 5;
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &r);
		TextOut(hDC, 0, r.bottom - 20, str, (int)wcslen(str));
		FrameRect(hDC, &(RECT){ 0, r.bottom - 20, 400, r.bottom }, GetStockObject(BLACK_BRUSH));

		DrawShape(hDC, &data[dataCnt-1]);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		if (VK_LEFT == wParam) {
			data[dataCnt - 1].x1 -= a;
			data[dataCnt - 1].x2 -= a;
		}
		else if (VK_UP == wParam) {
			data[dataCnt - 1].y1 -= a;
			data[dataCnt - 1].y2 -= a;
		}
		else if (VK_RIGHT == wParam) {
			data[dataCnt - 1].x1 += a;
			data[dataCnt - 1].x2 += a;
		}
		else if (VK_DOWN == wParam) {
			data[dataCnt - 1].y1 += a;
			data[dataCnt - 1].y2 += a;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_CHAR:
		if (VK_BACK == wParam) {
			if (cntStr > 0) {
				if (str[--cntStr] == ' ')
					spaceCnt--;
				str[cntStr] = '\0';
			}
		}
		else if (VK_RETURN == wParam) {
			if (spaceCnt == 0 && cntStr == 1) {
				if(dataCnt == 0)
					MessageBox(hWnd, L"먼저 데이터를 입력해주세요.",
						L"오류", MB_ICONWARNING | MB_OK);
				else
					ControlShape(str[0], &data[dataCnt - 1]);
				if (str[0] == 'p') {
					if (dataCnt > 1)
						dataCnt--;
					else
						MessageBox(hWnd, L"prior: 이전의 데이터가 없습니다.",
							L"오류", MB_ICONWARNING | MB_OK);
				}
				else if (str[0] == 'n') {
					if (dataCnt < maxDataCnt)
						dataCnt++;
					else
						MessageBox(hWnd, L"next: 이후의 데이터가 없습니다.",
							L"오류", MB_ICONWARNING | MB_OK);
				}

				cntStr = 0;
				str[cntStr] = '\0';
			}
			else if (str[cntStr - 1] == ' ')
				MessageBox(hWnd, L"입력값 형식이 올바르지 않습니다. (마지막 입력값에 공백이 들어가면 안됩니다.)",
					L"오류", MB_ICONWARNING | MB_OK);
			else if(spaceCnt != 5)
				MessageBox(hWnd, L"입력값 형식이 올바르지 않습니다.",
					L"오류", MB_ICONWARNING | MB_OK);
			else if (dataCnt < 10) {
				TCHAR* ptr;
				ptr = wcstok(str, L" \r", NULL);
				data[dataCnt].shape = _wtoi(ptr);
				ptr = wcstok(NULL, L" \r", NULL);
				data[dataCnt].x1 = _wtoi(ptr);
				ptr = wcstok(NULL, L" \r", NULL);
				data[dataCnt].y1 = _wtoi(ptr);
				ptr = wcstok(NULL, L" \r", NULL);
				data[dataCnt].x2 = _wtoi(ptr);
				ptr = wcstok(NULL, L" \r", NULL);
				data[dataCnt].y2 = _wtoi(ptr);
				ptr = wcstok(NULL, L" ", NULL);
				data[dataCnt].t = _wtoi(ptr);
				if (data[dataCnt].shape <= 4 && data[dataCnt].shape >= 1
					&& data[dataCnt].t <= 10 && data[dataCnt].t >= 1
					&& data[dataCnt].x1 >= 0 && data[dataCnt].y1 >= 0
					&& data[dataCnt].x2 < r.right && data[dataCnt].y2 < r.bottom) {
					data[dataCnt].cBrush = RGB(rand() % 256, rand() % 256, rand() % 256);
					data[dataCnt].cPen = RGB(rand() % 256, rand() % 256, rand() % 256);
					dataCnt++;
					maxDataCnt++;
				}
				else {
					MessageBox(hWnd, L"입력값이 범위를 벗어났습니다.",
						L"오류", MB_ICONWARNING | MB_OK);
				}
			}
			else {
				MessageBox(hWnd, L"데이터가 모두 찼습니다. (최대 10개)",
					L"오류", MB_ICONWARNING | MB_OK);
			}
			spaceCnt = 0;
			cntStr = 0;
			str[cntStr] = '\0';
		}
		else {
			if (cntStr < SIZE) {
				str[cntStr++] = (TCHAR)wParam;
				str[cntStr] = '\0';
			}
		}
		if (VK_SPACE == wParam)
			spaceCnt++;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}
void ControlShape(TCHAR t, Data* data)
{
	if (t == 'k') {
		data[0].cBrush = RGB(rand() % 256, rand() % 256, rand() % 256);
		data[0].cPen = RGB(rand() % 256, rand() % 256, rand() % 256);
	}
	else if (t == '+') {
		if (data[0].t == 10) {
			data[0].x1--; data[0].x2++; data[0].y1--; data[0].y2++;
		}
		else
			data[0].t++;
	}
	else if (t == '-') {
		if (data[0].t == 1) {
			data[0].x1++; data[0].x2--; data[0].y1++; data[0].y2--;
		}
		else
			data[0].t--;
	}
}
void DrawShape(HDC hDC, const Data* d)
{

	HBRUSH hBrush = CreateSolidBrush(d[0].cBrush);
	HPEN hPen = CreatePen(PS_SOLID, d[0].t, d[0].cPen);

	SelectObject(hDC, hPen);
	SelectObject(hDC, hBrush);
	if (d[0].shape == 1) {
		MoveToEx(hDC, d[0].x1, d[0].y1, NULL);
		LineTo(hDC, d[0].x2, d[0].y2);
	}
	else if (d[0].shape == 2) {
		POINT point[3] = { {(d[0].x1 + d[0].x2) / 2, d[0].y1}, {d[0].x1, d[0].y2}, {d[0].x2, d[0].y2} };
		Polygon(hDC, point, 3);
	}
	else if (d[0].shape == 3) {
		Rectangle(hDC, d[0].x1, d[0].y1, d[0].x2, d[0].y2);
	}
	else if (d[0].shape == 4) {
		Ellipse(hDC, d[0].x1, d[0].y1, d[0].x2, d[0].y2);
	}

	DeleteObject(hPen);
	DeleteObject(hBrush);
}