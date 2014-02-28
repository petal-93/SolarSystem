// ChildWin.cpp : Defines the entry point for the application.
//

#include "windows.h"
#include "windowsx.h"
#include <math.h>
#include <wingdi.h>
#include <vector>
#include <map>
#include "SolarSystemObject.h"

#include <stdio.h>
/**/
#define WM_MY_MSG 200
#define WM_RECALC_OK 201


#define IDC_MAIN_BUTTON_EXIT 300
#define IDC_MAIN_BUTTON_SPEEDUP 205
#define IDC_MAIN_BUTTON_SPEEDDOWN 206

HINSTANCE hinst;
HWND hWindow;
int ResX = GetSystemMetrics(SM_CXSCREEN);
int ResY = GetSystemMetrics(SM_CYSCREEN);

HWND hButtonExit, hButtonSpeedUp, hButtonSpeedDown;

static PAINTSTRUCT PS;

HDC		hDC,		//контекст окна
		hCopyDC,	//контекст в памяти - все отрисовывается на нём
		hBackGrDC;	//контекст с фоном

RECT	rcClient,	//прямоугольник с клиентской областью
		rcWindow,	//со всем окном
		rcCurrent,	//текущий прямоугольник, куда рисуем
		rcBackGr;	//с фоном

HBITMAP hBmpBackGr,		//хэндл картинки-фона
		hBmpMask,		//маска
		hBmpCopyClient, //копия клиентской области
		hBmpTemp;		//временный

namespace SS
{
	struct Object
	{
		HANDLE hTread;
		HDC hObjDC;
		HBITMAP hBmpObj;
		RECT rcObj;
		SSO::SolarSystemObject Obj;
	};
}

std::map<std::string, SS::Object> SolarSystem;


HANDLE hSemaphoreRecalc;
// функция синхронизующего потока
DWORD WINAPI SolarSystemTreadSync(LPVOID lpParam)
{
	MSG msg;
	std::string objName((const char*)lpParam);
	hSemaphoreRecalc = CreateSemaphore(NULL, 0, SolarSystem.size() - 1, TEXT("SemaphoreForRecalc"));
	while (1)
	{
		Sleep(60);
		ReleaseSemaphore(hSemaphoreRecalc, SolarSystem.size() - 1, NULL);
		SolarSystem[objName].Obj.Next();
		int i = 0;
		while (1)
		{
			GetMessage(&msg, NULL, 0, 0);
			if (msg.message == WM_RECALC_OK)
				i++;
			if (i >= SolarSystem.size() - 1)
			{
				i = 0;
				break;
			}
		}
		PostMessage(hWindow, WM_MY_MSG, 0, 0);
	}
	return 0;
}
// функция потока
DWORD WINAPI SolarSystemTread(LPVOID lpParam)
{
	std::string objName((const char*)lpParam);
	while (1)
	{
		WaitForSingleObject(hSemaphoreRecalc, INFINITE);
		SolarSystem[objName].Obj.Next();
		std::map<std::string, SS::Object>::iterator it = SolarSystem.begin(), it_end = SolarSystem.end();
		PostThreadMessage(GetThreadId(SolarSystem["Mercury"].hTread), WM_RECALC_OK, 0, 0);
	}
	return 0;
}

// по клику на кнопку Выход
void OnClickButtonExit()
{
	PostMessage(hWindow, WM_QUIT, 0, 0);
}
// разбор WM_COMMAND (действия на щелчки по кнопкам)
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
			std::map<std::string, SS::Object>::iterator it = SolarSystem.begin(), it_end = SolarSystem.end();
			for (; it != it_end; ++it)
			{
				it->second.Obj.setSpeedRatio(it->second.Obj.getSpeedRatio() + 5);
			}
		}
	}
	else
	if (hWndCtl == hButtonSpeedDown)
	{
		if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED))
		{
			std::map<std::string, SS::Object>::iterator it = SolarSystem.begin(), it_end = SolarSystem.end();
			for (; it != it_end; ++it)
			{
				it->second.Obj.setSpeedRatio(it->second.Obj.getSpeedRatio() - 5);
				if (it->second.Obj.getSpeedRatio() < 0) it->second.Obj.setSpeedRatio(0);
			}
		}
	}
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	HANDLE_MSG(hWindow, WM_COMMAND, OnCommand);
	case WM_CREATE:
		;
		{
			HDC hTempDC = GetDC(hWindow);				//контекст окна
			hCopyDC = CreateCompatibleDC(hTempDC);		//контекст-прослойка
			hBackGrDC = CreateCompatibleDC(hTempDC);	//для фона

			hBmpBackGr = (HBITMAP)LoadImage(NULL, L"space.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

			SS::Object Mercury;
			Mercury.Obj = SSO::SolarSystemObject(69, 46, 100, 0.8, 47, 24, true, 0.5, 10, 0.5);
			Mercury.hObjDC = CreateCompatibleDC(hTempDC);
			Mercury.hTread = CreateThread(0, 0, SolarSystemTreadSync, "Mercury", CREATE_SUSPENDED, 0);
			Mercury.hBmpObj = (HBITMAP)LoadImage(NULL, L"mercury1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			Mercury.rcObj = { 0 };
			SolarSystem.insert(std::pair<std::string, SS::Object>("Mercury", Mercury));

			SS::Object Venus;
			Venus.Obj = SSO::SolarSystemObject(107, 108, 200, 0.8, 35, 60, true, 0.5, 10, 0.0);
			Venus.hObjDC = CreateCompatibleDC(hTempDC);
			Venus.hTread = CreateThread(0, 0, SolarSystemTread, "Venus", CREATE_SUSPENDED, 0);
			Venus.hBmpObj = (HBITMAP)LoadImage(NULL, L"venus1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			Venus.rcObj = { 0 };
			SolarSystem.insert(std::pair<std::string, SS::Object>("Venus", Venus));

			SS::Object Jupiter;
			Jupiter.Obj = SSO::SolarSystemObject(389, 340, 500, 0.8, 13, 130, true, 0.5, 10, 0.0);
			Jupiter.hObjDC = CreateCompatibleDC(hTempDC);
			Jupiter.hTread = CreateThread(0, 0, SolarSystemTread, "Jupiter", CREATE_SUSPENDED, 0);
			Jupiter.hBmpObj = (HBITMAP)LoadImage(NULL, L"Jupiter1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			Jupiter.rcObj = { 0 };
			SolarSystem.insert(std::pair<std::string, SS::Object>("Jupiter", Jupiter));


			SS::Object Earth;
			Earth.Obj = SSO::SolarSystemObject(152, 147, 270, 0.8, 29, 63, true, 0.5, 10, 1.9);
			Earth.hObjDC = CreateCompatibleDC(hTempDC);
			Earth.hTread = CreateThread(0, 0, SolarSystemTread, "Earth", CREATE_SUSPENDED, 0);
			Earth.hBmpObj = (HBITMAP)LoadImage(NULL, L"earth1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			Earth.rcObj = { 0 };
			SolarSystem.insert(std::pair<std::string, SS::Object>("Earth", Earth));

			SS::Object Moon;
			Earth.Obj = SSO::SolarSystemObject(30, 40, 40, 0.8, 80, 8, true, 1, 10, 3.14);
			Earth.hObjDC = CreateCompatibleDC(hTempDC);
			Earth.hTread = CreateThread(0, 0, SolarSystemTread, "Moon", CREATE_SUSPENDED, 0);
			Earth.hBmpObj = (HBITMAP)LoadImage(NULL, L"moon1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			Earth.rcObj = { 0 };
			SolarSystem.insert(std::pair<std::string, SS::Object>("Moon", Earth));

			SS::Object Mars;
			Earth.Obj = SSO::SolarSystemObject(172, 172, 380, 0.8, 20, 63, true, 0.5, 10, 6.0);
			Earth.hObjDC = CreateCompatibleDC(hTempDC);
			Earth.hTread = CreateThread(0, 0, SolarSystemTread, "Mars", CREATE_SUSPENDED, 0);
			Earth.hBmpObj = (HBITMAP)LoadImage(NULL, L"mars1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			Earth.rcObj = { 0 };
			SolarSystem.insert(std::pair<std::string, SS::Object>("Mars", Earth));

			SS::Object Sun;
			Earth.Obj = SSO::SolarSystemObject(172, 172, 0, 0.016, 0, 200, false, 0.5, 10, 6.0);
			Earth.hObjDC = CreateCompatibleDC(hTempDC);
			Earth.hTread = CreateThread(0, 0, SolarSystemTread, "Sun", CREATE_SUSPENDED, 0);
			Earth.hBmpObj = (HBITMAP)LoadImage(NULL, L"sun2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			Earth.rcObj = { 0 };
			SolarSystem.insert(std::pair<std::string, SS::Object>("Sun", Earth));

			//пустой битмап для хранения всй картины
			hBmpCopyClient = CreateCompatibleBitmap(hTempDC, ResX, ResY);

			ReleaseDC(hWindow, hTempDC);//отпускаем контекст

			//привязываем картинки к контекстам
			std::map<std::string, SS::Object>::iterator it = SolarSystem.begin(), it_end = SolarSystem.end();
			for (; it != it_end; ++it)
			{
				hBmpTemp = (HBITMAP)SelectObject(it->second.hObjDC, it->second.hBmpObj);
				if (hBmpTemp) DeleteObject(hBmpTemp);
			}
			hBmpTemp = (HBITMAP)SelectObject(hBackGrDC, hBmpBackGr);
			if (hBmpTemp) DeleteObject(hBmpTemp);
			hBmpTemp = (HBITMAP)SelectObject(hCopyDC, hBmpCopyClient);
			if (hBmpTemp) DeleteObject(hBmpTemp);
			//...

			/*std::map<std::string, HeavenlyBody>::iterator */it = SolarSystem.begin(), it_end = SolarSystem.end();
			for (; it != it_end; ++it)
			{
				ResumeThread(it->second.hTread);
			}

			/**/
		}
		break;
	case WM_MY_MSG:
		/**/
		//что перерисовать
		rcCurrent.left = 0;
		rcCurrent.top = 0;
		rcCurrent.right = ResX;
		rcCurrent.bottom = ResY;
		//обновить фон
		StretchBlt(hCopyDC, 0, 0, ResX, ResY,
				   hBackGrDC, 0, 0, ResX, ResY,
				   SRCCOPY);
		//копировать Меркурий
		TransparentBlt(hCopyDC,
					   SolarSystem["Mercury"].Obj.getX() + ResX / 2 - SolarSystem["Mercury"].Obj.getRadius()/2,
					   SolarSystem["Mercury"].Obj.getY() + ResY / 2 - SolarSystem["Mercury"].Obj.getRadius()/2,
					   SolarSystem["Mercury"].Obj.getRadius(),
					   SolarSystem["Mercury"].Obj.getRadius(),
					   SolarSystem["Mercury"].hObjDC, 0, 0, 400, 400, RGB(0, 255, 0));

		//копировать Венеру
		TransparentBlt(hCopyDC,
					   SolarSystem["Venus"].Obj.getX() + ResX / 2 - SolarSystem["Venus"].Obj.getRadius()/2,
					   SolarSystem["Venus"].Obj.getY() + ResY / 2 - SolarSystem["Venus"].Obj.getRadius()/2,
					   SolarSystem["Venus"].Obj.getRadius(),
					   SolarSystem["Venus"].Obj.getRadius(),
					   SolarSystem["Venus"].hObjDC, 0, 0, 500, 500, RGB(0, 255, 0));


		//копировать Юпитер
		TransparentBlt(hCopyDC,
					   SolarSystem["Jupiter"].Obj.getX() + ResX / 2 - SolarSystem["Jupiter"].Obj.getRadius() / 2,
					   SolarSystem["Jupiter"].Obj.getY() + ResY / 2 - SolarSystem["Jupiter"].Obj.getRadius() / 2,
					   SolarSystem["Jupiter"].Obj.getRadius(),
					   SolarSystem["Jupiter"].Obj.getRadius(),
					   SolarSystem["Jupiter"].hObjDC, 0, 0, 500, 500, RGB(0, 255, 0));



		//копировать Землю
		TransparentBlt(hCopyDC,
					   SolarSystem["Earth"].Obj.getX() + ResX / 2 - SolarSystem["Earth"].Obj.getRadius()/2,
					   SolarSystem["Earth"].Obj.getY() + ResY / 2 - SolarSystem["Earth"].Obj.getRadius()/2,
					   SolarSystem["Earth"].Obj.getRadius(),
					   SolarSystem["Earth"].Obj.getRadius(),
					   SolarSystem["Earth"].hObjDC, 0, 0, 500, 500, RGB(0, 255, 0));


		//копировать Луну
		TransparentBlt(hCopyDC,
					   SolarSystem["Moon"].Obj.getX() + SolarSystem["Earth"].Obj.getX() + ResX / 2 - SolarSystem["Moon"].Obj.getRadius() / 2,
					   SolarSystem["Moon"].Obj.getY() + SolarSystem["Earth"].Obj.getY() + ResY / 2 - SolarSystem["Moon"].Obj.getRadius() / 2,
					   SolarSystem["Moon"].Obj.getRadius(),
					   SolarSystem["Moon"].Obj.getRadius(),
					   SolarSystem["Moon"].hObjDC, 0, 0, 500, 500, RGB(0, 255, 0));



		//копировать Марс
		TransparentBlt(hCopyDC,
					   SolarSystem["Mars"].Obj.getX() + ResX / 2 - SolarSystem["Mars"].Obj.getRadius()/2,
					   SolarSystem["Mars"].Obj.getY() + ResY / 2 - SolarSystem["Mars"].Obj.getRadius()/2,
					   SolarSystem["Mars"].Obj.getRadius(),
					   SolarSystem["Mars"].Obj.getRadius(),
					   SolarSystem["Mars"].hObjDC, 0, 0, 500, 500, RGB(0, 255, 0));


		//копировать Солнце
		TransparentBlt(hCopyDC,
					   SolarSystem["Sun"].Obj.getX() + ResX / 2 - SolarSystem["Sun"].Obj.getRadius() / 2,
					   SolarSystem["Sun"].Obj.getY() + ResY / 2 - SolarSystem["Sun"].Obj.getRadius() / 2,
					   SolarSystem["Sun"].Obj.getRadius(),
					   SolarSystem["Sun"].Obj.getRadius(),
					   SolarSystem["Sun"].hObjDC, 0, 0, 500, 500, RGB(0, 255, 0));


		//посылаем WM_PAINT
		InvalidateRect(hWindow, &rcCurrent, false);
		/**/
		break;
	case WM_PAINT:
		;
		{
			hDC = BeginPaint(hWindow, &PS);
			StretchBlt(hDC, PS.rcPaint.left, PS.rcPaint.top, PS.rcPaint.right, PS.rcPaint.bottom,
					   hCopyDC, PS.rcPaint.left, PS.rcPaint.top, PS.rcPaint.right, PS.rcPaint.bottom,
					  // SRCPAINT);
					   SRCCOPY);
			EndPaint(hWindow, &PS);
		}
		break;
	case WM_DESTROY:
		TerminateThread(SolarSystem["Mercury"].hTread, 0);
		if (hBmpBackGr) DeleteObject(hBmpBackGr);
		if (hBmpCopyClient) DeleteObject(hBmpCopyClient);
		if (hBmpMask) DeleteObject(hBmpMask);
		//if (hBmpMercury) DeleteObject(hBmpMercury);
		if (hBmpTemp) DeleteObject(hBmpTemp);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, Message, wParam, lParam);
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
	w.style = CS_HREDRAW | CS_VREDRAW ;
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

	hButtonExit = CreateWindowEx(NULL, L"BUTTON", L"Выход", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
								 ResX-150, 10, 100, 50, hWindow, (HMENU)IDC_MAIN_BUTTON_EXIT, GetModuleHandle(NULL), NULL);
	hButtonSpeedUp = CreateWindowEx(NULL, L"BUTTON", L"+", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
									ResX-150, 170,100, 50, hWindow, (HMENU)IDC_MAIN_BUTTON_SPEEDUP, GetModuleHandle(NULL), NULL);
	hButtonSpeedDown = CreateWindowEx(NULL, L"BUTTON", L"-", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
									  ResX-150, 130, 100, 50, hWindow, (HMENU)IDC_MAIN_BUTTON_SPEEDDOWN, GetModuleHandle(NULL), NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}