#define  WINVER  0x0500
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <SDKDDKVer.h>
#include <wingdi.h>


#define BTNRED  1001
#define BTNBLUE 1002

HWND hRed, hBlue;
HINSTANCE hInst;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
VOID drawButtonRed(DRAWITEMSTRUCT *dis, HWND hwnd);
VOID drawButtonBlue(DRAWITEMSTRUCT *dis, HWND hwnd);

WCHAR szClassName[] = _T("ColoredButtons");

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nFunsterStil)
{
	HWND hwnd;
	MSG messages;
	WNDCLASSEX wincl;

	hInst = hThisInstance;

	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = WindowProcedure;
	wincl.style = CS_DBLCLKS;
	wincl.cbSize = sizeof(WNDCLASSEX);

	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;
	wincl.cbClsExtra = 0;
	wincl.cbWndExtra = 0;
	wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

	if (!RegisterClassEx(&wincl)) { return 0; }

	hwnd = CreateWindowEx(0, szClassName, _T("Colored Buttons"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 544, 375, HWND_DESKTOP, NULL, hThisInstance, NULL);

	ShowWindow(hwnd, nFunsterStil);

	while (GetMessage(&messages, NULL, 0, 0))
	{
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}

	return messages.wParam;
}


LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		hRed = CreateWindowEx(0, _T("BUTTON"), _T("RED"), WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 0, 100, 100, 50, hwnd, (HMENU)BTNRED, hInst, NULL);
		hBlue = CreateWindowEx(0, _T("BUTTON"), _T("BLUE"), WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 0, 200, 100, 50, hwnd, (HMENU)BTNBLUE, hInst, NULL);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case BTNRED: MessageBox(hwnd, _T("Red Button Pressed"), _T("Red"), MB_OK); break;
		case BTNBLUE: MessageBox(hwnd, _T("Blue Button Pressed"), _T("Blue"), MB_OK); break;
		}
		break;
	case WM_DRAWITEM:
	{
		DRAWITEMSTRUCT *dis = (DRAWITEMSTRUCT*)lParam;
		switch (dis->CtlID) {
		case BTNRED: drawButtonRed(dis, hRed); break;
		case BTNBLUE: drawButtonBlue(dis, hBlue); break;
		}
		return TRUE;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

VOID drawButtonRed(DRAWITEMSTRUCT *dis, HWND hwnd) {
	if (dis->itemState & ODS_SELECTED) {
		TRIVERTEX vertex[2];
		vertex[0].x = 0;
		vertex[0].y = 0;
		vertex[0].Red = 65535;
		vertex[0].Green = 0;
		vertex[0].Blue = 0;

		vertex[1].x = 100;
		vertex[1].y = 50;
		vertex[1].Red = 52428;
		vertex[1].Green = 0;
		vertex[1].Blue = 0;


		GRADIENT_RECT gRect[1];
		gRect[0].UpperLeft = 0;
		gRect[0].LowerRight = 1;

		GradientFill(dis->hDC, vertex, 2, gRect, 1, GRADIENT_FILL_RECT_H);
	}
	else {
		TRIVERTEX vertex[2];
		vertex[0].x = 0;
		vertex[0].y = 0;
		vertex[0].Red = 39321;
		vertex[0].Green = 0;
		vertex[0].Blue = 0;

		vertex[1].x = 100;
		vertex[1].y = 50;
		vertex[1].Red = 56797;
		vertex[1].Green = 0;
		vertex[1].Blue = 0;

		GRADIENT_RECT gRect[1];
		gRect[0].UpperLeft = 0;
		gRect[0].LowerRight = 1;

		GradientFill(dis->hDC, vertex, 2, gRect, 1, GRADIENT_FILL_RECT_H);
	}

	RECT rc;
	GetClientRect(hwnd, &rc);

	SetBkMode(dis->hDC, TRANSPARENT);

	char buf[255];
	GetWindowText(hwnd, (LPTSTR)buf, 255);
	DrawText(dis->hDC, (LPTSTR)buf, strlen(buf), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

VOID drawButtonBlue(DRAWITEMSTRUCT *dis, HWND hwnd) {
	RECT rc;
	GetClientRect(hwnd, &rc);

	if (dis->itemState & ODS_SELECTED) {
		TRIVERTEX vertex[2];
		vertex[0].x = 0;
		vertex[0].y = 0;
		vertex[0].Red = 0x0000;
		vertex[0].Green = 0x0000;
		vertex[0].Blue = 0xffff;

		vertex[1].x = 100;
		vertex[1].y = 50;
		vertex[1].Red = 0x0000;
		vertex[1].Green = 0x0000;
		vertex[1].Blue = 0xcccc;


		GRADIENT_RECT gRect;
		gRect.UpperLeft = 0;
		gRect.LowerRight = 1;

		GradientFill(dis->hDC, vertex, 2, &gRect, 1, GRADIENT_FILL_RECT_H);
	}
	else {
		TRIVERTEX vertex[2];
		vertex[0].x = 0;
		vertex[0].y = 0;
		vertex[0].Red = 0x0000;
		vertex[0].Green = 0x0000;
		vertex[0].Blue = 0x9999;

		vertex[1].x = 100;
		vertex[1].y = 50;
		vertex[1].Red = 0x0000;
		vertex[1].Green = 0x0000;
		vertex[1].Blue = 0xdddd;

		GRADIENT_RECT gRect;
		gRect.UpperLeft = 0;
		gRect.LowerRight = 1;

		GradientFill(dis->hDC, vertex, 2, &gRect, 1, GRADIENT_FILL_RECT_H);
	}

	SetBkMode(dis->hDC, TRANSPARENT);

	char buf[255];
	GetWindowText(hwnd, (LPTSTR)buf, 255);
	DrawText(dis->hDC, (LPTSTR)buf, strlen(buf), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
