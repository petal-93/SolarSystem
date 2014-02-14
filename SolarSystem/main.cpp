// ChildWin.cpp : Defines the entry point for the application.
//

#include "windows.h"
#include "windowsx.h"
#include "resource.h"

#define ID_FIRSTCHILD	100
#define ID_SECONDCHILD	101
#define ID_THIRDCHILD	102


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hinst;
HWND hWindow, hWindowMercury;
HANDLE hThreadMercury;
int x = 0, y = 0, h = 100, w = 100;
// ф-ция потока
DWORD WINAPI MercuryThread(LPVOID lpParam)
{
	while (1)
	{
		//SendMessage(hWi, WM_RECALC, 0, 0);
		Sleep(100);
		//SolarSystem["Mercury"].RecalcCoords(xCenter, yCenter);
		x += 10;
		y += 10;
		MoveWindow(hWindowMercury, x, y, h, w, true);
	}
	return 0;
}


int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS w;
	hinst = hInstance;
	memset(&w, 0, sizeof(WNDCLASS));
	w.style = CS_HREDRAW | CS_VREDRAW;
	w.lpfnWndProc = WndProc;
	w.hInstance = hInstance;
	w.hbrBackground = GetStockBrush(BLACK_BRUSH);
	w.lpszClassName = L"C Windows";
	w.hCursor = LoadCursor(NULL, IDC_ARROW);
	RegisterClass(&w);
	hwnd = CreateWindow(L"C Windows", L"C Windows", WS_EX_TOPMOST | WS_POPUP | WS_CLIPCHILDREN,
		0, 0,
		(int)GetSystemMetrics(SM_CXSCREEN),
		(int)GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hInstance, NULL);
	hWindow = hwnd;
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wparam, LPARAM lparam)
{
	switch (Message)
	{
	case WM_CREATE:
		WNDCLASS w;
		memset(&w, 0, sizeof(WNDCLASS));
		w.lpfnWndProc = ChildProc;
		w.hInstance = hinst;
		w.hbrBackground = GetStockBrush(WHITE_BRUSH);
		w.lpszClassName = L"ChildWClass";
		w.hCursor = LoadCursor(NULL, IDC_CROSS);
		RegisterClass(&w);
		HWND child;
		child = CreateWindowEx(0, L"ChildWClass", (LPCTSTR)NULL,
			WS_CHILD | WS_BORDER | WS_VISIBLE, x, y,
			50, 50, hwnd, (HMENU)(int)(ID_FIRSTCHILD), hinst, NULL);
		hWindowMercury = child;
		ShowWindow(child, SW_NORMAL);
		UpdateWindow(child);

		/*потоки*/
		hThreadMercury = CreateThread(0, 0, MercuryThread, 0, 0, 0);

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, Message, wparam, lparam);
}


LRESULT CALLBACK ChildProc(HWND hwnd, UINT Message, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT PS;
	BITMAP bm;
	HDC hDC;
	HBITMAP hBitmap;
	HDC hMemDC;
	switch (Message)
	{
	case WM_CREATE:
		HRGN hRegionRect;
		hRegionRect = CreateEllipticRgn(0, 0, 50, 50);
		SetWindowRgn(hwnd, hRegionRect, TRUE);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hwnd, &PS);
		hBitmap = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_MERCURY));
		hMemDC = CreateCompatibleDC(hDC);
		SelectObject(hMemDC, hBitmap);
		GetObject(hBitmap, sizeof(bm), &bm);
		BitBlt(hDC, -2, -2, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0, SRCCOPY);
		DeleteDC(hMemDC);
		DeleteObject(hBitmap);
		EndPaint(hwnd, &PS);
		break;
	case WM_DESTROY:
		break;
	}
	return DefWindowProc(hwnd, Message, wparam, lparam);
}