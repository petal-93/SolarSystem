﻿// ChildWin.cpp : Defines the entry point for the application.
//

#include "windows.h"
#include "windowsx.h"
#include <tchar.h>
#include <math.h>
#include <wingdi.h>
#include <vector>
#include <map>
#include <stdio.h>
#include "SolarSystem.h"
#include "DoubleBuffering.h"
#include "decorations.h"

HINSTANCE hinst;
HWND hWindow;
int ResX = GetSystemMetrics(SM_CXSCREEN);
int ResY = GetSystemMetrics(SM_CYSCREEN);

SS::SolarSystem *SolarSystem;
DB::DoubleBuffering *DoubleBuffering;

/// <summary> Создание вектора с изображениями </summary>
/// <param name = "path" Путь к папке с изображениями </param>
std::vector<HBITMAP> SetOfPictures(std::wstring path);

HDC GetEllipse(HWND hWindow);

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_COMMAND:
		OnCommand(hWindow, (HWND)lParam, HIWORD(wParam), SolarSystem);
		break;
	case WM_CREATE:
	{


		
	}
		break;
	case WM_SHOWWINDOW:
	{

		DoubleBuffering = new DB::DoubleBuffering(hWindow, ResX, ResY);
		DoubleBuffering->AddBackground((HBITMAP)LoadImage(NULL, L"space.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));

		SolarSystem = new SS::SolarSystem(hWindow, -1, 1, 1, 1);

		SolarSystem->Add("Sun", "Sun", 0, 0, 0, 0, 0, 100, 0, SetOfPictures(_T(".\\Images\\Sun\\*.bmp")));
		SolarSystem->Add("Mercury", "Sun", 69, 46, 100, 0.8, 47, 24, 0, SetOfPictures(_T(".\\Images\\Mercury\\*.bmp")));
		SolarSystem->Add("Venus", "Sun", 107, 108, 200, 0.8, 35, 60, 0, SetOfPictures(_T(".\\Images\\Venus\\*.bmp")));
		SolarSystem->Add("Earth", "Sun", 152, 147, 270, 0.8, 29, 63, 0, SetOfPictures(_T(".\\Images\\Earth\\*.bmp")));
		SolarSystem->Add("Moon", "Earth", 30, 40, 40, 0.8, 80, 8, 0, SetOfPictures(_T(".\\Images\\Moon\\*.bmp")));
		//SolarSystem->Add("Mars", "Sun", 172, 172, 380, 0.8, 20, 63, 0, SetOfPictures(_T(".\\Images\\_Sun\\*.bmp")));

		DoubleBuffering->Release();
		SolarSystem->Go();
	}
		break;
	case WM_REPAINT:
	{
		DoubleBuffering->BackgroundUpdate();

		SS::SolarSystem::SolarSystemObject *temp;
		temp = SolarSystem->Get("Sun");
		DoubleBuffering->AddObject(temp->GetX() + ResX / 2, temp->GetY() + ResY / 2, temp->GetRadius(),
			temp->GetRadius(), temp->GetHDC(), temp->GetBmpWidth(), temp->GetBmpHeight(), RGB(0, 255, 0));
		
		temp = SolarSystem->Get("Mercury");
		DoubleBuffering->AddObject(ResX/2-temp->GetA(), ResY/2-temp->GetB(), temp->GetA()*2, temp->GetB()*2, GetEllipse(hWindow), ResX, ResY, RGB(0, 255, 0));
		DoubleBuffering->AddObject(temp->GetX() + ResX / 2, temp->GetY() + ResY / 2, temp->GetRadius(),
			temp->GetRadius(), temp->GetHDC(), temp->GetBmpWidth(), temp->GetBmpHeight(), RGB(0, 255, 0));

		temp = SolarSystem->Get("Venus");
		DoubleBuffering->AddObject(ResX / 2 - temp->GetA(), ResY / 2 - temp->GetB(), temp->GetA() * 2, temp->GetB() * 2, GetEllipse(hWindow), ResX, ResY, RGB(0, 255, 0));
		DoubleBuffering->AddObject(temp->GetX() + ResX / 2, temp->GetY() + ResY / 2, temp->GetRadius(),
			temp->GetRadius(), temp->GetHDC(), temp->GetBmpWidth(), temp->GetBmpHeight(), RGB(0, 255, 0));

		temp = SolarSystem->Get("Earth");
		DoubleBuffering->AddObject(ResX / 2 - temp->GetA(), ResY / 2 - temp->GetB(), temp->GetA() * 2, temp->GetB() * 2, GetEllipse(hWindow), ResX, ResY, RGB(0, 255, 0));
		DoubleBuffering->AddObject(temp->GetX() + ResX / 2, temp->GetY() + ResY / 2, temp->GetRadius(),
			temp->GetRadius(), temp->GetHDC(), temp->GetBmpWidth(), temp->GetBmpHeight(), RGB(0, 255, 0));

		temp = SolarSystem->Get("Moon");
		DoubleBuffering->AddObject(ResX / 2 - temp->GetA(), ResY / 2 - temp->GetB(), temp->GetA() * 2, temp->GetB() * 2, GetEllipse(hWindow), ResX, ResY, RGB(0, 255, 0));
		DoubleBuffering->AddObject(temp->GetX() + ResX / 2, temp->GetY() + ResY / 2, temp->GetRadius(),
			temp->GetRadius(), temp->GetHDC(), temp->GetBmpWidth(), temp->GetBmpHeight(), RGB(0, 255, 0));
		
		
		DoubleBuffering->Invalidate();
	}
		break;
	case WM_PAINT:
	{
		DoubleBuffering->Paint();
	}
		break;
	case WM_DESTROY:
		delete SolarSystem;
		delete DoubleBuffering;

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
	ButtonsInit(hWindow, ResX, ResY);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

std::vector<HBITMAP> SetOfPictures(std::wstring path)
{
	std::vector<HBITMAP> pictures;
	std::wstring p;
	//TCHAR buffer[MAX_PATH];
	WIN32_FIND_DATA wfd;
	HANDLE const hFind = FindFirstFileW(path.c_str(), &wfd);
	//GetFullPathName(path.c_str(), MAX_PATH, buffer, 0);
	if (INVALID_HANDLE_VALUE != hFind)
	{
		do
		{
			p = path.substr(0, path.find_last_of(L"/\\")+1) + std::wstring(wfd.cFileName);
			pictures.push_back((HBITMAP)LoadImage(NULL, p.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
		} while (FindNextFile(hFind, &wfd));
	}
	FindClose(hFind);
	return pictures;
}

HDC GetEllipse(HWND hWindow)
{
	HDC hDC;
	HBITMAP bmp1;
	RECT rc;
	GetClientRect(hWindow, &rc);
	hDC = CreateCompatibleDC(GetDC(hWindow));
	bmp1 = CreateCompatibleBitmap(hDC, rc.right - rc.left, rc.bottom - rc.top);
	(HBITMAP)SelectObject(hDC, bmp1);
	(HBRUSH)SelectObject(hDC, CreateSolidBrush(RGB(0, 0, 0)));
	(HPEN)SelectObject(hDC, CreatePen(PS_SOLID, 8, RGB(255, 255, 255)));
	Ellipse(hDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top);
	return hDC;
}
