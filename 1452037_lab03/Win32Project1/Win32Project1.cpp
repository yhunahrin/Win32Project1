// Win32Project1.cpp : Defines the entry point for the application.
/*
Trong chương trình này, ta sẽ cố gắng lưu lại bản vẽ sau khi bản vẽ.
Việc vẽ các đường thẳng tạm sẽ không được vẽ trực tiếp lên bản vẽ này.
Ý tưởng: sử dụng một bitmap để lưu bản vẽ hiện hành. Việc vẽ các đường thẳng tạm sẽ được vẽ trực tiếp lên màn hình.
khi kết thúc việc vẽ thì ta mới vẽ đường thẳng lên bitmap
Đối với kỹ thuật này thì khi vẽ đường thẳng ta không cần vẽ đè đường
*/

#include "stdafx.h"
#include "Win32Project1.h"
#include "Windowsx.h"


#define MAX_LOADSTRING 100
#define ID_TIMER1	123123
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
POINT p1, p2, p2_ancien;
HBITMAP hBitmap;
BOOL enTrainDessin;
int o = 0;
int mode; //mode de dessin==> 0 pour ligne, 1 pour libre
bool save;
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
VOID				DrawLineProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
VOID				FreeDrawProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
VOID				DrawRectangleProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
VOID				DrawEllipseProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
VOID				DrawSquareProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
VOID				DrawRoundProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
VOID				errhandler(LPCTSTR, HWND);
VOID				CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi,
	HBITMAP hBMP, HDC hDC);
PBITMAPINFO			CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
VOID DrawLine(HDC hdc);
VOID FreeDraw(HDC hdc);
VOID PenBrushDemo(HDC hdc);
VOID DrawRectangle(HDC hdc);
VOID DrawEllipse(HDC hdc);
VOID DrawSquare(HDC hdc);
VOID DrawRound(HDC hdc);
VOID Pen(HDC hdc, int size, int R, int G, int B);
int size, R, G, B, Size, color;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
	wcex.hCursor = LoadCursor(nullptr, IDC_CROSS);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDR_MENU1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU,
		0, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = GetDC(hWnd);
	HBITMAP image = NULL;
	TCHAR szFile[260];
	switch (mode)
	{
	case 0:
		FreeDrawProc(hWnd, message, wParam, lParam);
		break;
	case 1:
		DrawLineProc(hWnd, message, wParam, lParam);
	case 3:
		DrawRectangleProc(hWnd, message, wParam, lParam);
	case 4:
		DrawEllipseProc(hWnd, message, wParam, lParam);
	case 5:
		DrawSquareProc(hWnd, message, wParam, lParam);
	case 6:
		DrawRoundProc(hWnd, message, wParam, lParam);
	}

	switch (message)
	{
	case WM_CREATE:
	{
					  p1.x = p1.y = 0;
					  p2 = p1;
					  mode = 1;
					  size = PS_SOLID;
					  R = 0;
					  G = 0;
					  B = 0;
					  enTrainDessin = FALSE;

					  //Tạo bitmap

					  RECT rect;
					  GetClientRect(hWnd, &rect);
					  //Tạo ra một bitmap tương thích với DC màn hình
					  hBitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
					  image = (HBITMAP)LoadImage(NULL, szFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

					  //Tô background cho bitmap
					  //Tạo memory dc để tương tác với Bitmap
					  HDC memDC = CreateCompatibleDC(hdc);
					  SelectObject(memDC, hBitmap);
					  HBRUSH hBrush = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
					  FillRect(memDC, &rect, hBrush);
					  DeleteObject(hBrush);
					  DeleteDC(memDC);
					  ReleaseDC(hWnd, hdc);

					  break;
	}
	case WM_COMMAND:
	{
					   save = 0;
					   int wmId = LOWORD(wParam);
					   // Parse the menu selections:
					   switch (wmId)
					   {
					   case ID_DRAW_PEN:
						   mode = 2;
						   RECT rect;
						   GetWindowRect(hWnd, &rect);
						   InvalidateRect(hWnd, &rect, true);
						   UpdateWindow(hWnd);
						   break;

					   case ID_SHAPE_RECTANGLE:
					   {
												  mode = 3;
												  break;
					   }
					   case ID_SHAPE_SQUARE:
					   {
											   mode = 5;
											   break;
					   }
					   case ID_SHAPE_ROUND:
					   {
											  mode = 6;
											  break;
					   }
					   case ID_SHAPE_ELIPSE:
					   {
											   mode = 4;
											   break;
					   }
					   case ID_LINE_STRAIGHTLINE:
					   {
													mode = 1;
													break;
					   }
					   case ID_LINE_FREESTYLE:
					   {
												 mode = 0;
												 break;
					   }
					   case ID_LINE_CURVE:
					   {
											 break;
					   }
					   case ID_FILE_SAVE:
					   {
											//OPENFILENAME ofn;

											//o = 1;
											//ZeroMemory(&ofn, sizeof(ofn));
											//ofn.lStructSize = sizeof(ofn);
											//ofn.hwndOwner = hWnd;
											//ofn.lpstrFile = szFile;
											//ofn.lpstrFile[0] = '\0';
											//ofn.nMaxFile = sizeof(szFile);
											//ofn.lpstrFilter = _T("All Files(*.*)\0 * .*\0Bmp Files(*.bmp)\0 * .bmp\0Text Files(*.txt)\0 * .txt\0");
											//ofn.nFilterIndex = 1;
											//ofn.lpstrInitialDir = _T("C:\\");
											//ofn.lpstrTitle = _T("My Application - Open file");
											//ofn.lpstrDefExt = _T("bmp");
											//ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
											//// Display the Open dialog box. 
											//if (GetOpenFileName(&ofn) == TRUE)
											//{
											//	PAINTSTRUCT ps;
											//	// TODO: Add any drawing code that uses hdc here...
											//	HDC hdc = GetDC(hWnd);
											//	//HDC hdc = BeginPaint(hWnd, &ps);
											//	hBitmap = (HBITMAP)LoadImage(NULL, ofn.lpstrFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
											//	//Vẽ bitmap ra màn hình trước
											//	HDC memDC = CreateCompatibleDC(hdc);
											//	SelectObject(memDC, hBitmap);
											//	RECT rect;
											//	GetClientRect(hWnd, &rect);
											//	BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, memDC, 0, 0, SRCCOPY);
											//	DeleteObject(memDC);
											//	//break;
											//	//DeleteDC(memDC);
											//	//EndPaint(hWnd, &ps);
											//}
											OPENFILENAME ofn;
											ZeroMemory(&ofn, sizeof(ofn));
											char szFileName[MAX_LOADSTRING];
											ZeroMemory(szFileName, MAX_LOADSTRING);
											ofn.lStructSize = sizeof(ofn);
											ofn.hwndOwner = NULL;
											ofn.lpstrFilter = _T("All Files(*.*)\0 * .*\0Bmp Files(*.bmp)\0 * .bmp\0Text Files(*.txt)\0 * .txt\0");
											ofn.lpstrFile = (LPWSTR)szFileName;
											ofn.nMaxFile = MAX_LOADSTRING;
											ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
											ofn.lpstrDefExt = (LPCWSTR)L"bmp";
											GetSaveFileName(&ofn);
											PBITMAPINFO PBi = CreateBitmapInfoStruct(hWnd, hBitmap);
											CreateBMPFile(hWnd, ofn.lpstrFile, PBi, hBitmap, hdc);
											/*save = 1;*/
											break;
					   }
					   case ID_WIDTH_1X:
					   {
										   Size = 1;
										   break;
					   }
					   case ID_WIDTH_3X:
					   {
										   Size = 2;
										   break;
					   }
					   case ID_WIDTH_5X:
					   {
										   Size = 3;
										   break;
					   }
					   case ID_COLOR_BLACK:
					   {
											  color = 1;
											  break;
					   }
					   case ID_COLOR_RED:
					   {
											color = 2;
											break;
					   }
					   case ID_COLOR_BLUE:
					   {
											 color = 3;
											 break;
					   }
					   case ID_COLOR_GREEN:
					   {
											  color = 4;
											  break;
					   }
					   case ID_FILE_EXIT:
					   {
											if (save == 0)
											{
												if (MessageBox(hWnd, _T("Are you want save?"), _T("Confirm close"), MB_OKCANCEL | MB_ICONQUESTION) == 1)
												{
													PBITMAPINFO PBi = CreateBitmapInfoStruct(hWnd, hBitmap);
													CreateBMPFile(hWnd, _T("D:\\a.bmp"), PBi, hBitmap, hdc);
													DestroyWindow(hWnd);
												}
												else
												{
													DestroyWindow(hWnd);
												}
											}
											else
											{
												DestroyWindow(hWnd);
											}
											break;
					   }
					   case IDM_ABOUT:
						   DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
						   break;
					   case IDM_EXIT:
						   if (MessageBox(hWnd, _T("Are you sure to close?"), _T("Confirm close"), MB_OKCANCEL | MB_ICONQUESTION) == 1)
						   {

							   DestroyWindow(hWnd);
						   }
						   break;
					   default:
						   return DefWindowProc(hWnd, message, wParam, lParam);
					   }
	}
		break;


		//case WM_RBUTTONDOWN:
		//	SetTimer(hWnd, ID_TIMER1, 1000, NULL);
		//	
		//	break;
		//case WM_RBUTTONDBLCLK:
		//	KillTimer(hWnd, ID_TIMER1);
		//	//effacer l'ecran
		//	SendMessage(hWnd, WM_ERASEBKGND, (WPARAM)GetDC(hWnd), (LPARAM) nullptr);
		//	break;
		//case WM_TIMER:
		//	switch (wParam)
		//	{
		//	case ID_TIMER1:
		//		KillTimer(hWnd, ID_TIMER1);
		//		mode = (mode + 1) % 2;
		//		MessageBox(hWnd, _T("Mode de dessin change"), _T("Notifcation"), MB_OK);
		//		break;
		//	}
	case WM_PAINT:
	{
					 /*
					 }*/
					 PAINTSTRUCT ps;

					 // TODO: Add any drawing code that uses hdc here...
					 HDC hdc = BeginPaint(hWnd, &ps);
					 //HDC hdc = GetDC(hWnd);
					 //Vẽ bitmap ra màn hình trước
					 HDC memDC = CreateCompatibleDC(hdc);
					 //break;
					 SelectObject(memDC, hBitmap);
					 RECT rect1;
					 GetClientRect(hWnd, &rect1);
					 BitBlt(hdc, 0, 0, rect1.right - rect1.left, rect1.bottom - rect1.top, memDC, 0, 0, SRCCOPY);
					 switch (mode)
					 {
					 case 0:
						 //Vẽ tự do thì ta vẽ luôn trên màn hình và bitmap
						 FreeDraw(memDC);
						 FreeDraw(hdc);
						 break;
					 case 1:
						 //Vẽ đường thẳng thì ta chỉ vẽ lên màn hình
						 DrawLine(hdc);
						 break;
					 case 2:
						 PenBrushDemo(memDC);
						 PenBrushDemo(hdc);
						 break;
					 case 3:
						 DrawRectangle(hdc);
						 break;
					 case 4:
						 DrawEllipse(hdc);
						 break;
					 case 5:
						 DrawSquare(hdc);
						 break;
					 case 6:
						 DrawRound(hdc);
						 /*case 7:*/
					 }

					 DeleteObject(memDC);
					 //SelectObject(hdc, oldPen);
					 EndPaint(hWnd, &ps);
	}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

VOID DrawLine(HDC hdc)
{
	Pen(hdc, size, R, G, B);
	if (enTrainDessin == TRUE)
	{

		MoveToEx(hdc, p1.x, p1.y, NULL);
		LineTo(hdc, p2.x, p2.y);
	}
}

VOID FreeDraw(HDC hdc)
{
	Pen(hdc, size, R, G, B);
	if (enTrainDessin == TRUE)
	{
		MoveToEx(hdc, p1.x, p1.y, NULL);
		LineTo(hdc, p2.x, p2.y);
		p1 = p2;
	}
}

VOID DrawRectangle(HDC hdc)
{
	Pen(hdc, size, R, G, B);
	if (enTrainDessin == TRUE)
	{
		HGDIOBJ I = GetStockBrush(NULL_BRUSH);
		SelectObject(hdc, I);
		MoveToEx(hdc, p1.x, p1.y, NULL);
		Rectangle(hdc, p1.x, p1.y, p2.x, p2.y);

	}
}
VOID DrawSquare(HDC hdc)
{
	Pen(hdc, size, R, G, B);
	if (enTrainDessin == TRUE)
	{
		HGDIOBJ I = GetStockBrush(NULL_BRUSH);
		SelectObject(hdc, I);
		MoveToEx(hdc, p1.x, p1.y, NULL);
		if ((p2.x - p1.x)*(p2.y - p1.y) >= 0)
			Rectangle(hdc, p1.x, p1.y, p2.x, p1.y - p1.x + p2.x);
		else
			Rectangle(hdc, p1.x, p1.y, p2.x, p1.y + p1.x - p2.x);
	}
}
VOID DrawRound(HDC hdc)
{
	Pen(hdc, size, R, G, B);
	if (enTrainDessin == TRUE)
	{
		MoveToEx(hdc, p1.x, p1.y, NULL);
		if ((p2.x - p1.x)*(p2.y - p1.y) >= 0)
			Arc(hdc, p1.x, p1.y, p2.x, p1.y - p1.x + p2.x, 0, 0, 0, 0);
		else
			Arc(hdc, p1.x, p1.y, p2.x, p1.y + p1.x - p2.x, 0, 0, 0, 0);
	}
}
VOID DrawEllipse(HDC hdc)
{
	Pen(hdc, size, R, G, B);
	if (enTrainDessin == TRUE)
	{

		MoveToEx(hdc, p1.x, p1.y, NULL);
		Arc(hdc, p1.x, p1.y, p2.x, p2.y, 0, 0, 0, 0);
	}
}
VOID DrawLineProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT windowRect;
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
						   p1.x = GET_X_LPARAM(lParam);
						   p1.y = GET_Y_LPARAM(lParam);
						   p2_ancien = p2 = p1;
						   enTrainDessin = TRUE;
						   break;
	}
	case WM_MOUSEMOVE:
	{
						 if (wParam&MK_LBUTTON)
						 {
							 p2.x = GET_X_LPARAM(lParam);
							 p2.y = GET_Y_LPARAM(lParam);
							 GetWindowRect(hWnd, &windowRect);
							 InvalidateRect(hWnd, &windowRect, FALSE);
							 UpdateWindow(hWnd);
						 }
						 break;
	}
	case WM_LBUTTONUP:

		//Tiến hành cho vẽ lên bitmap
		HDC hdc = GetDC(hWnd);
		HDC memDC = CreateCompatibleDC(hdc);
		SelectObject(memDC, hBitmap);
		DrawLine(memDC);
		DeleteObject(memDC);
		ReleaseDC(hWnd, hdc);

		enTrainDessin = FALSE;
		break;
	}
}
VOID DrawRoundProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT windowRect;
	HDC hdc = GetDC(hWnd);
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
						   p1.x = GET_X_LPARAM(lParam);
						   p1.y = GET_Y_LPARAM(lParam);
						   p2_ancien = p2 = p1;
						   enTrainDessin = TRUE;
						   break;
	}
	case WM_MOUSEMOVE:
	{
						 if (wParam&MK_LBUTTON)
						 {
							 p2.x = GET_X_LPARAM(lParam);
							 p2.y = GET_Y_LPARAM(lParam);
							 GetWindowRect(hWnd, &windowRect);
							 InvalidateRect(hWnd, &windowRect, FALSE);
							 UpdateWindow(hWnd);
						 }
						 break;
	}
	case WM_LBUTTONUP:

		//Tiến hành cho vẽ lên bitmap
		HDC hdc = GetDC(hWnd);
		HDC memDC = CreateCompatibleDC(hdc);
		SelectObject(memDC, hBitmap);
		DrawRound(memDC);
		DeleteObject(memDC);
		ReleaseDC(hWnd, hdc);

		enTrainDessin = FALSE;
		break;
	}
}
VOID DrawRectangleProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT windowRect;
	HDC hdc = GetDC(hWnd);
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
						   p1.x = GET_X_LPARAM(lParam);
						   p1.y = GET_Y_LPARAM(lParam);
						   p2_ancien = p2 = p1;
						   enTrainDessin = TRUE;
						   break;
	}
	case WM_MOUSEMOVE:
	{
						 if (wParam&MK_LBUTTON)
						 {
							 p2.x = GET_X_LPARAM(lParam);
							 p2.y = GET_Y_LPARAM(lParam);
							 GetWindowRect(hWnd, &windowRect);

							 // GetObject(hWnd,)
							 InvalidateRect(hWnd, &windowRect, FALSE);

							 UpdateWindow(hWnd);
						 }
						 break;
	}
	case WM_LBUTTONUP:

		//Tiến hành cho vẽ lên bitmap
		HDC hdc = GetDC(hWnd);
		HDC memDC = CreateCompatibleDC(hdc);
		SelectObject(memDC, hBitmap);
		/*HGDIOBJ I = GetStockBrush(NULL_BRUSH);
		SelectObject(hdc, I);*/
		DrawRectangle(memDC);
		DeleteObject(memDC);
		ReleaseDC(hWnd, hdc);

		enTrainDessin = FALSE;
		break;
	}
}
VOID DrawEllipseProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT windowRect;
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
						   p1.x = GET_X_LPARAM(lParam);
						   p1.y = GET_Y_LPARAM(lParam);
						   p2_ancien = p2 = p1;
						   enTrainDessin = TRUE;
						   break;
	}
	case WM_MOUSEMOVE:
	{
						 if (wParam&MK_LBUTTON)
						 {
							 p2.x = GET_X_LPARAM(lParam);
							 p2.y = GET_Y_LPARAM(lParam);
							 GetWindowRect(hWnd, &windowRect);
							 InvalidateRect(hWnd, &windowRect, FALSE);
							 UpdateWindow(hWnd);
						 }
						 break;
	}
	case WM_LBUTTONUP:

		//Tiến hành cho vẽ lên bitmap
		HDC hdc = GetDC(hWnd);
		HDC memDC = CreateCompatibleDC(hdc);
		SelectObject(memDC, hBitmap);
		DrawEllipse(memDC);
		DeleteObject(memDC);
		ReleaseDC(hWnd, hdc);

		enTrainDessin = FALSE;
		break;
	}
}
VOID DrawSquareProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT windowRect;
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
						   p1.x = GET_X_LPARAM(lParam);
						   p1.y = GET_Y_LPARAM(lParam);
						   p2_ancien = p2 = p1;
						   enTrainDessin = TRUE;
						   break;
	}
	case WM_MOUSEMOVE:
	{
						 if (wParam&MK_LBUTTON)
						 {
							 p2.x = GET_X_LPARAM(lParam);
							 p2.y = GET_Y_LPARAM(lParam);
							 GetWindowRect(hWnd, &windowRect);
							 InvalidateRect(hWnd, &windowRect, FALSE);
							 UpdateWindow(hWnd);
						 }
						 break;
	}
	case WM_LBUTTONUP:

		//Tiến hành cho vẽ lên bitmap
		HDC hdc = GetDC(hWnd);
		HDC memDC = CreateCompatibleDC(hdc);
		SelectObject(memDC, hBitmap);
		DrawSquare(memDC);
		DeleteObject(memDC);
		ReleaseDC(hWnd, hdc);

		enTrainDessin = FALSE;
		break;
	}
}
VOID FreeDrawProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT windowRect;
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
						   p1.x = GET_X_LPARAM(lParam);
						   p1.y = GET_Y_LPARAM(lParam);
						   p2 = p1;
						   enTrainDessin = TRUE;
						   break;
	}
	case WM_MOUSEMOVE:
	{
						 if (wParam&MK_LBUTTON)
						 {

							 p2.x = GET_X_LPARAM(lParam);
							 p2.y = GET_Y_LPARAM(lParam);
							 GetWindowRect(hWnd, &windowRect);
							 InvalidateRect(hWnd, &windowRect, FALSE);
							 UpdateWindow(hWnd);
						 }
						 break;
	}
	case WM_LBUTTONUP:
		enTrainDessin = FALSE;
		break;


	}
}
VOID Pen(HDC hdc, int size, int R, int G, int B)
{
	switch (Size)
	{
	case 1:
		size = PS_DASH;
		break;
	case 2:
		size = 3;
		break;
	case 3:
		size = 5;
		break;
	}
	switch (color)
	{
	case 1:
		R = 0;
		G = 0;
		B = 0;
		break;
	case 2:
		R = 255;
		G = 0;
		B = 0;
		break;
	case 3:
		R = 0;
		G = 0;
		B = 255;
		break;
	case 4:
		R = 0;
		G = 255;
		B = 0;
		break;
	}
	HPEN hPen = CreatePen(PS_SOLID, size, RGB(R, G, B));
	SelectObject(hdc, hPen);
}
VOID PenBrushDemo(HDC hdc)
{
	HPEN hPen = CreatePen(PS_DASHDOT, 3, RGB(255, 0, 255));
	SelectObject(hdc, hPen);
	MoveToEx(hdc, 100, 100, NULL);
	LineTo(hdc, 200, 200);
	Rectangle(hdc, 100, 200, 300, 300);
	Ellipse(hdc, 100, 200, 300, 300);
	HRGN hRegion = CreateEllipticRgn(100, 200, 300, 300);
	HBRUSH hbrush = CreateSolidBrush(RGB(23, 100, 40));
	FillRgn(hdc, hRegion, hbrush);
	Ellipse(hdc, 100, 200, 300, 300);
}


// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{
	BITMAP bmp;
	PBITMAPINFO pbmi;
	WORD    cClrBits;
	// Retrieve the bitmap color format, width, and height.  
	if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
		errhandler(_T("GetObject"), hwnd);
	// Convert the color format to a count of bits.  
	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
	else if (cClrBits <= 8)
		cClrBits = 8;
	else if (cClrBits <= 16)
		cClrBits = 16;
	else if (cClrBits <= 24)
		cClrBits = 24;
	else cClrBits = 32;
	// Allocate memory for the BITMAPINFO structure. (This structure  
	// contains a BITMAPINFOHEADER structure and an array of RGBQUAD  
	// data structures.)  
	if (cClrBits < 24)
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
		sizeof(BITMAPINFOHEADER)+
		sizeof(RGBQUAD)* (1 << cClrBits));
	// There is no RGBQUAD array for these formats: 24-bit-per-pixel or 32-bit-per-pixel 
	else
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
		sizeof(BITMAPINFOHEADER));
	// Initialize the fields in the BITMAPINFO structure.  
	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = bmp.bmWidth;
	pbmi->bmiHeader.biHeight = bmp.bmHeight;
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
	if (cClrBits < 24)
		pbmi->bmiHeader.biClrUsed = (1 << cClrBits);
	// If the bitmap is not compressed, set the BI_RGB flag.  
	pbmi->bmiHeader.biCompression = BI_RGB;
	// Compute the number of bytes in the array of color  
	// indices and store the result in biSizeImage.  
	// The width must be DWORD aligned unless the bitmap is RLE 
	// compressed. 
	pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8
		* pbmi->bmiHeader.biHeight;
	// Set biClrImportant to 0, indicating that all of the  
	// device colors are important.  
	pbmi->bmiHeader.biClrImportant = 0;
	return pbmi;
}
void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi,
	HBITMAP hBMP, HDC hDC)
{
	HANDLE hf;                 // file handle  
	BITMAPFILEHEADER hdr;       // bitmap file-header  
	PBITMAPINFOHEADER pbih;     // bitmap info-header  
	LPBYTE lpBits;              // memory pointer  
	DWORD dwTotal;              // total count of bytes  
	DWORD cb;                   // incremental count of bytes  
	BYTE *hp;                   // byte pointer  
	DWORD dwTmp;
	pbih = (PBITMAPINFOHEADER)pbi;
	lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);
	if (!lpBits)
		errhandler(_T("GlobalAlloc"), hwnd);
	// Retrieve the color table (RGBQUAD array) and the bits  
	// (array of palette indices) from the DIB.  
	if (!GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi,
		DIB_RGB_COLORS))
	{
		errhandler(_T("GetDIBits"), hwnd);
	}
	// Create the .BMP file.  
	hf = CreateFile(pszFile,
		GENERIC_READ | GENERIC_WRITE,
		(DWORD)0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);
	if (hf == INVALID_HANDLE_VALUE)
		errhandler(_T("CreateFile"), hwnd);
	hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"  
	// Compute the size of the entire file.  
	hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER)+
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD)+pbih->biSizeImage);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;
	// Compute the offset to the array of color indices.  
	hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER)+
		pbih->biSize + pbih->biClrUsed
		* sizeof (RGBQUAD);
	// Copy the BITMAPFILEHEADER into the .BMP file.  
	if (!WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER),
		(LPDWORD)&dwTmp, NULL))
	{
		errhandler(_T("WriteFile"), hwnd);
	}
	// Copy the BITMAPINFOHEADER and RGBQUAD array into the file.  
	if (!WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER)
		+pbih->biClrUsed * sizeof (RGBQUAD),
		(LPDWORD)&dwTmp, (NULL)))
		errhandler(_T("WriteFile"), hwnd);
	// Copy the array of color indices into the .BMP file.  
	dwTotal = cb = pbih->biSizeImage;
	hp = lpBits;
	if (!WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL))
		errhandler(_T("WriteFile"), hwnd);
	// Close the .BMP file.  
	if (!CloseHandle(hf))
		errhandler(_T("CloseHandle"), hwnd);
	// Free memory.  
	GlobalFree((HGLOBAL)lpBits);
}
void errhandler(LPCTSTR sz, HWND hwnd)
{
	MessageBox(hwnd, sz, _T("Error"), MB_ICONERROR);
	PostQuitMessage(0);
}