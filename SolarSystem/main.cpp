/*
Смоделировать солнечную систему (Солнце, планеты и Луну).
Пропорции расстояний от Солнца до планет и от Земли до Луны можно не соблюдать.
Главное - чтобы все было отчетливо нарисовано.
Каждая планета (и Луна тоже) управляется отдельной нитью.
Одна из планет (Земля) движется самостоятельно с заранее заданной скоростью,
движение остальных планет осуществляется через синхронизацию с движением Земли.
Все орбиты считать круговыми.
*/
#include <windows.h>
#include <WindowsX.h>
#include <TChar.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <map>
#include <string>
#include <processthreadsapi.h>

#include "resource1.h"


#define WM_RECALC (WM_USER + 0x0001)
#define IDC_MAIN_BUTTON_A 201 
#define IDC_MAIN_BUTTON_V 202
#define IDC_MAIN_BUTTON_S 203
#define IDC_MAIN_BUTTON_E 204
#define IDC_MAIN_BUTTON_Moon 205
#define IDC_MAIN_BUTTON_MARS 206
#define IDC_MAIN_BUTTON_M 207
#define IDC_MAIN_BUTTON_EXIT 300
#define IDC_MAIN_BUTTON_SPEEDUP 301
#define IDC_MAIN_BUTTON_SPEEDDOWN 302

const TCHAR strWindowTitle[] = TEXT("Лабораторная №2");
HINSTANCE hInstance;
HWND hWindow;

struct SolarSystemObj
{
	HANDLE hThreadProc = 0;
	HWND hWnd = 0;
	float x = 0, y = 0, r = 0, s = 0, v = 0;
	bool f = 0;

	void SetCoords(float x, float y)
	{
		this->x = x;
		this->y = y;
	};
	void SetAttr(float r, float s, float v)
	{
		this->v = v;
		this->r = r;
		this->s = s;
	};
	void RandCoords(int xCenter, int yCenter)
	{
		// Generate random numbers in the half-closed interval
		// [range_min, range_max). In other words,
		// range_min <= random number < range_max
		int range_max = xCenter + r;
		int range_min = xCenter - r;
		srand((unsigned)time(NULL));
		x = (double)rand() / (RAND_MAX + 1) * (range_max - range_min) + range_min;
		y = yCenter + sqrt(r * r - 1.0*(x - xCenter)*(x - xCenter));
	};
	void RecalcCoords(int xCenter, int yCenter)
	{
		float a = (xCenter + r) / x, b = (xCenter - r) / x;
		int coeff = (((a) < (b)) ? (a) : (b));

		if (f == 0)
		{
			x += v;// -2 * coeff;

			if (x >= xCenter + r)
			{
				f = 1;
				//y = yCenter + sqrt ( r * r - 1.0*( x - xCenter )*( x - xCenter ) );
				x = xCenter + r;
			}
			//else
			y = yCenter + sqrt(r * r - 1.0*(x - xCenter)*(x - xCenter));
		}
		if (f == 1)
		{
			x -= v;// +2 * coeff;
			if (x <= xCenter - r)
			{
				f = 0;
				//y = yCenter - sqrt ( r * r - 1.0*( x - xCenter )*( x - xCenter ) );
				x = xCenter - r;
			}
			//else
			y = yCenter - sqrt(r * r - 1.0*(x - xCenter)*(x - xCenter));
		}
	};
} NoObj;
int xCenter, yCenter;
int frequency = 100;
std::map<std::string, SolarSystemObj> SolarSystem = {
	{ "Sun", NoObj },
	{ "Mercury", NoObj },
	{ "Venus", NoObj },
	{ "Earth", NoObj },
	{ "Moon", NoObj },
	{ "Mars", NoObj },
	{ "Asteroids", NoObj } };
//{ "Jupiter", NoObj },
//{ "Saturn", NoObj },
//{ "Uranus", NoObj },
//{ "Neptune", NoObj },
//{ "Centaurs", NoObj },
//{ "Pluto", NoObj },
//{ "Charon", NoObj },
//{ "Makemake", NoObj },
//{ "Haumea", NoObj } };
HBITMAP hBitmapSource = NULL;
HDC hdcSource = NULL;
HBITMAP g_hBmp_sun = NULL, g_hBmp_mercury = NULL, g_hBmp_venus = NULL,
g_hBmp_earth = NULL, g_hBmp_moon = NULL, g_hBmp_mars = NULL, g_hBmp_asteroids = NULL;
CRITICAL_SECTION cs;
HWND hButtonExit, hButtonSpeedUp, hButtonSpeedDown;

// обработчик сообщения WM_DRAWITEM
SIZE SizeBitmap(HBITMAP hBmp)
{
	SIZE resSize = { 0, 0 };
	BITMAP bmp = { 0, 0, 0, 0, 0, 0, NULL };
	int iRes = ::GetObject(hBmp, sizeof(BITMAP), &bmp);
	if (iRes == sizeof(BITMAP))
	{
		resSize.cx = bmp.bmWidth;
		resSize.cy = bmp.bmHeight;
	}
	return resSize;
}
void OnDrawItem(HWND hWnd, const DRAWITEMSTRUCT *lpDrawItem)
{
	//RECT rc = lpDrawItem->rcItem;
	//::DrawFrameControl(lpDrawItem->hDC, &rc, DFC_BUTTON, DFCS_PUSHED|DFCS_BUTTONPUSH);

	HDC hCDC = ::CreateCompatibleDC(lpDrawItem->hDC);
	HBITMAP hBmpOld;
	if (lpDrawItem->hwndItem == SolarSystem["Sun"].hWnd)
	{
		HBITMAP &hBmp = g_hBmp_sun;
		hBmpOld = (HBITMAP)::SelectObject(hCDC, hBmp);
		::StretchBlt(
			lpDrawItem->hDC,
			0, 0, lpDrawItem->rcItem.right - lpDrawItem->rcItem.left, lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top,
			hCDC,
			0, 0, SizeBitmap(hBmp).cx, SizeBitmap(hBmp).cy,
			SRCCOPY
			);

		::SelectObject(hCDC, hBmpOld);
		::DeleteDC(hCDC);
	}
	else
	if (lpDrawItem->hwndItem == SolarSystem["Mercury"].hWnd)
	{
		HBITMAP &hBmp = g_hBmp_mercury;
		hBmpOld = (HBITMAP)::SelectObject(hCDC, hBmp);
		::StretchBlt(
			lpDrawItem->hDC,
			0, 0, lpDrawItem->rcItem.right - lpDrawItem->rcItem.left, lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top,
			hCDC,
			0, 0, SizeBitmap(hBmp).cx, SizeBitmap(hBmp).cy,
			SRCCOPY
			);

		::SelectObject(hCDC, hBmpOld);
		::DeleteDC(hCDC);
	}
	else
	if (lpDrawItem->hwndItem == SolarSystem["Venus"].hWnd)
	{
		HBITMAP &hBmp = g_hBmp_venus;
		hBmpOld = (HBITMAP)::SelectObject(hCDC, hBmp);
		::StretchBlt(
			lpDrawItem->hDC,
			0, 0, lpDrawItem->rcItem.right - lpDrawItem->rcItem.left, lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top,
			hCDC,
			0, 0, SizeBitmap(hBmp).cx, SizeBitmap(hBmp).cy,
			SRCCOPY
			);

		::SelectObject(hCDC, hBmpOld);
		::DeleteDC(hCDC);
	}
	else
	if (lpDrawItem->hwndItem == SolarSystem["Earth"].hWnd)
	{
		HBITMAP &hBmp = g_hBmp_earth;
		hBmpOld = (HBITMAP)::SelectObject(hCDC, hBmp);
		::StretchBlt(
			lpDrawItem->hDC,
			0, 0, lpDrawItem->rcItem.right - lpDrawItem->rcItem.left, lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top,
			hCDC,
			0, 0, SizeBitmap(hBmp).cx, SizeBitmap(hBmp).cy,
			SRCCOPY
			);

		::SelectObject(hCDC, hBmpOld);
		::DeleteDC(hCDC);
	}
	else
	if (lpDrawItem->hwndItem == SolarSystem["Moon"].hWnd)
	{
		HBITMAP &hBmp = g_hBmp_moon;
		hBmpOld = (HBITMAP)::SelectObject(hCDC, hBmp);
		::StretchBlt(
			lpDrawItem->hDC,
			0, 0, lpDrawItem->rcItem.right - lpDrawItem->rcItem.left, lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top,
			hCDC,
			0, 0, SizeBitmap(hBmp).cx, SizeBitmap(hBmp).cy,
			SRCCOPY
			);

		::SelectObject(hCDC, hBmpOld);
		::DeleteDC(hCDC);
	}
	else
	if (lpDrawItem->hwndItem == SolarSystem["Mars"].hWnd)
	{
		HBITMAP &hBmp = g_hBmp_mars;
		hBmpOld = (HBITMAP)::SelectObject(hCDC, hBmp);
		::StretchBlt(
			lpDrawItem->hDC,
			0, 0, lpDrawItem->rcItem.right - lpDrawItem->rcItem.left, lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top,
			hCDC,
			0, 0, SizeBitmap(hBmp).cx, SizeBitmap(hBmp).cy,
			SRCCOPY
			);

		::SelectObject(hCDC, hBmpOld);
		::DeleteDC(hCDC);
	}
	else
	if (lpDrawItem->hwndItem == SolarSystem["Asteroids"].hWnd)
	{
		HBITMAP &hBmp = g_hBmp_asteroids;
		hBmpOld = (HBITMAP)::SelectObject(hCDC, hBmp);
		::StretchBlt(
			lpDrawItem->hDC,
			0, 0, lpDrawItem->rcItem.right - lpDrawItem->rcItem.left, lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top,
			hCDC,
			0, 0, SizeBitmap(hBmp).cx, SizeBitmap(hBmp).cy,
			SRCCOPY
			);

		::SelectObject(hCDC, hBmpOld);
		::DeleteDC(hCDC);
	}
}
// обработчик клика на кнопку
void OnClickButtonExit()
{
	PostMessage(hWindow, WM_QUIT, 0, 0);
}
// обработчик сообщения WM_COMMAND
void OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify)
{
	if (hWndCtl == hButtonExit)
	{
		if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED))
		{
			OnClickButtonExit();
		}
	}
	else
	if (hWndCtl == hButtonSpeedUp)
	{
		if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED))
		{
			if (frequency >= 1)
			{
				frequency = 1;
			}
			else
				frequency += 0.1;

		}
	}
	else
	if (hWndCtl == hButtonSpeedDown)
	{
		if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED))
		{
			if (frequency <= 1)
			{
				frequency = 1;
			}
			else
				frequency -= 0.1;
		}
	}
}
// главная функция обработки сообщений
LRESULT APIENTRY WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		HANDLE_MSG(hWnd, WM_DRAWITEM, OnDrawItem);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
	case WM_CREATE:
	{
					  g_hBmp_sun = (HBITMAP) ::LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP_SUN2),
						  IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_COPYFROMRESOURCE);
					  g_hBmp_mercury = (HBITMAP) ::LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP_MERCURY1),
						  IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_COPYFROMRESOURCE);
					  g_hBmp_venus = (HBITMAP) ::LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP_VENUS1),
						  IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_COPYFROMRESOURCE);
					  g_hBmp_earth = (HBITMAP) ::LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP_EARTH1),
						  IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_COPYFROMRESOURCE);
					  g_hBmp_moon = (HBITMAP) ::LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP_MOON1),
						  IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_COPYFROMRESOURCE);
					  g_hBmp_mars = (HBITMAP) ::LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP_MARS1),
						  IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_COPYFROMRESOURCE);
					  g_hBmp_asteroids = (HBITMAP) ::LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP_DEATH),
						  IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_COPYFROMRESOURCE);
					  xCenter = GetSystemMetrics(SM_CXSCREEN) / 2;
					  yCenter = GetSystemMetrics(SM_CYSCREEN) / 2;
					  // начальные координаты объектов солнечной системы
					  SolarSystem["Sun"].SetAttr(0, 50, 0);
					  SolarSystem["Sun"].SetCoords(xCenter, yCenter);
					  SolarSystem["Mercury"].SetAttr(40 * 2, 20, 4);
					  SolarSystem["Mercury"].RandCoords(xCenter, yCenter);
					  SolarSystem["Venus"].SetAttr(100 * 2, 30, 3);
					  SolarSystem["Venus"].RandCoords(xCenter, yCenter);
					  SolarSystem["Earth"].SetAttr(140 * 2, 30, 3);
					  SolarSystem["Earth"].RandCoords(xCenter, yCenter);
					  SolarSystem["Moon"].SetAttr(31, 10, 10);
					  SolarSystem["Moon"].RandCoords(SolarSystem["Earth"].x, SolarSystem["Earth"].y);
					  SolarSystem["Mars"].SetAttr(200 * 2, 33, 1);
					  SolarSystem["Mars"].RandCoords(xCenter, yCenter);
					  SolarSystem["Asteroids"].SetAttr(250 * 2, 50, 5);
					  SolarSystem["Asteroids"].RandCoords(xCenter, yCenter);
					  //InvalidateRect ( hWindow, 0, true );
					  break;
	}
	case WM_RECALC:
	{
					  //RedrawWindow ( hWnd, 0, 0, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW );
					  std::map<std::string, SolarSystemObj>::iterator it = SolarSystem.begin(), it_end = SolarSystem.end();
					  for (; it != it_end; ++it)
					  {
						  //UpdateWindow ( hWindow );
						  //RedrawWindow ( it->second.hWnd, 0, 0, RDW_UPDATENOW );
						  //InvalidateRect ( it->second.hWnd, 0, true );
						  //SendMessage ( it->second.hWnd, , 0, 0 );
						  //PostThreadMessage ( GetThreadId ( SolarSystem [ it->first ].hThreadProc ), WM_RECALC, 0, 0 );
					  }
					  break;
	}
	case WM_PAINT:
	{
					 PAINTSTRUCT ps;
					 HDC hdc = BeginPaint(hWnd, &ps);
					 //Фон
					 hBitmapSource = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP2), IMAGE_BITMAP, 0, 0, 0);
					 hdcSource = CreateCompatibleDC(hdc);
					 SelectObject(hdcSource, hBitmapSource);
					 BITMAP bitmap;
					 GetObject(hBitmapSource, sizeof(bitmap), &bitmap);
					 BitBlt(hdc,
						 ((int)GetSystemMetrics(SM_CXSCREEN) - bitmap.bmWidth) / 2,
						 0, bitmap.bmWidth, bitmap.bmHeight, hdcSource, 0, 0, SRCCOPY);
					 DeleteObject(hBitmapSource);
					 DeleteDC(hdcSource);

					 /*RECT rcClient, rcWnd;
					 GetClientRect ( hWnd, &rcClient );
					 SetTextColor ( hdc, RGB ( 0, 0, 0 ) );
					 SetBkColor ( hdc, RGB ( 255, 255, 255 ) );
					 TCHAR coords [ 120 ] = { 0};
					 wsprintf ( coords, TEXT ("скорость: %d"),frequency);
					 rcClient.left = 10;
					 rcClient.bottom = 36;
					 DrawText ( hdc, coords, wcslen ( coords ), &rcClient, DT_LEFT | DT_EXTERNALLEADING | DT_WORDBREAK );
					 */
					 EndPaint(hWnd, &ps);
	}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// ф-ция потока
DWORD WINAPI MercuryThread(LPVOID lpParam)
{
	while (1)
	{
		SendMessage(hWindow, WM_RECALC, 0, 0);
		Sleep(100);
		SolarSystem["Mercury"].RecalcCoords(xCenter, yCenter);
		MoveWindow(SolarSystem["Mercury"].hWnd, SolarSystem["Mercury"].x - SolarSystem["Mercury"].s / 2,
			SolarSystem["Mercury"].y - SolarSystem["Mercury"].s / 2,
			SolarSystem["Mercury"].s, SolarSystem["Mercury"].s, true);
		if (NULL != hWindow)
		{
			PostThreadMessage(GetThreadId(SolarSystem["Sun"].hThreadProc), WM_RECALC, 0, 0);
			PostThreadMessage(GetThreadId(SolarSystem["Venus"].hThreadProc), WM_RECALC, 0, 0);
			PostThreadMessage(GetThreadId(SolarSystem["Earth"].hThreadProc), WM_RECALC, 0, 0);
			PostThreadMessage(GetThreadId(SolarSystem["Moon"].hThreadProc), WM_RECALC, 0, 0);
			PostThreadMessage(GetThreadId(SolarSystem["Mars"].hThreadProc), WM_RECALC, 0, 0);
			PostThreadMessage(GetThreadId(SolarSystem["Asteroids"].hThreadProc), WM_RECALC, 0, 0);
		}

	}
	return 0;
}

DWORD WINAPI ObjectThread(LPVOID lpParam)
{
	std::string objName((const char*)lpParam);
	MSG msg;
	while (1)
	{
		GetMessage(&msg, NULL, 0, 0);
		if (msg.message == WM_RECALC)
		{
			SolarSystem[objName].RecalcCoords(xCenter, yCenter);
			if (NULL != hWindow)
			{
				MoveWindow(SolarSystem[objName].hWnd, SolarSystem[objName].x - SolarSystem[objName].s / 2,
					SolarSystem[objName].y - SolarSystem[objName].s / 2,
					SolarSystem[objName].s, SolarSystem[objName].s, true);
				//SendMessage ( hWindow, WM_RECALC, 0, 0 );
			}
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
	return 0;
}

DWORD WINAPI MoonThread(LPVOID lpParam)
{
	MSG msg;
	while (1)
	{
		GetMessage(&msg, NULL, 0, 0);
		if (msg.message == WM_RECALC)
		{
			SolarSystem["Moon"].RecalcCoords(SolarSystem["Earth"].x, SolarSystem["Earth"].y);
			if (NULL != hWindow)
			{
				MoveWindow(SolarSystem["Moon"].hWnd, SolarSystem["Moon"].x - SolarSystem["Moon"].s / 2,
					SolarSystem["Moon"].y - SolarSystem["Moon"].s / 2,
					SolarSystem["Moon"].s, SolarSystem["Moon"].s, true);
				//SendMessage ( hWindow, WM_RECALC, 0, 0 );
			}
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
	return 0;
}

// точка входа в программу
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int nShowCmd)
{
	hInstance = hInst;
	// регистрируется класс
	WNDCLASS wc = { 0 };
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInst;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszClassName = strWindowTitle;
	if (!RegisterClass(&wc)) return(0);

	// создается окно
	HWND hWnd = CreateWindow(
		strWindowTitle,
		strWindowTitle,
		WS_EX_TOPMOST | WS_POPUP | WS_CLIPCHILDREN,
		0, 0,
		(int)GetSystemMetrics(SM_CXSCREEN),
		(int)GetSystemMetrics(SM_CYSCREEN),
		0, 0, hInst, 0);
	if (!hWnd) return 0;
	hWindow = hWnd;
	hButtonExit = CreateWindowEx(NULL, L"BUTTON", L"Выход", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		10, 10, 50, 25, hWnd, (HMENU)IDC_MAIN_BUTTON_EXIT, GetModuleHandle(NULL), NULL);
	hButtonSpeedUp = CreateWindowEx(NULL, L"BUTTON", L"+", WS_CHILD | BS_PUSHBUTTON,
		10, 60, 20, 25, hWnd, (HMENU)IDC_MAIN_BUTTON_SPEEDUP, GetModuleHandle(NULL), NULL);
	hButtonSpeedDown = CreateWindowEx(NULL, L"BUTTON", L"-", WS_CHILD | BS_PUSHBUTTON,
		40, 60, 20, 25, hWnd, (HMENU)IDC_MAIN_BUTTON_SPEEDDOWN, GetModuleHandle(NULL), NULL);
	// создание объектов солнечной системы
	std::map<std::string, SolarSystemObj>::iterator it = SolarSystem.begin(), it_end = SolarSystem.end();
	int i = 0;
	for (; it != it_end; ++it)
	{
		std::wstring temp = std::wstring(it->first.begin(), it->first.end());
		LPCWSTR planet = temp.c_str();
		it->second.hWnd = CreateWindowEx(NULL, L"BUTTON", planet, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
			0, 0, 50, 50, hWnd, (HMENU)IDC_MAIN_BUTTON_A + i, GetModuleHandle(NULL), NULL);
		it->second.hThreadProc = CreateThread(0, 0, ObjectThread, (LPVOID)it->first.c_str(), 0, 0);
		i++;
	}
	//TerminateThread ( SolarSystem [ "Mercury" ].hThreadProc, 0 );
	SolarSystem["Mercury"].hThreadProc = CreateThread(0, 0, MercuryThread, 0, 0, 0);
	SolarSystem["Moon"].hThreadProc = CreateThread(0, 0, MoonThread, 0, 0, 0);

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);



	// главный цикл обработки сообщений
	MSG msg = { 0 };
	while (GetMessage(&msg, 0, 0, 0))
	{
		DispatchMessage(&msg);
	}

	return((int)msg.wParam);
}