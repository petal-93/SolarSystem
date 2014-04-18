// ChildWin.cpp : Defines the entry point for the application.
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
DB::DoubleBuffering *DoubleBuffering = NULL;

/// <summary> Создание вектора с изображениями </summary>
std::vector<HBITMAP> SetOfPictures(std::wstring path);
void AddTrack(DB::DoubleBuffering*, SS::SolarSystem::SolarSystemObject*);
void AddObject(DB::DoubleBuffering*, SS::SolarSystem::SolarSystemObject*);

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
		SolarSystem = new SS::SolarSystem(hWindow, 0.16, 0.8, 10.0, 2.0);
		SolarSystem->Add("Sun", "Sun", 0, 0, 0, 0, 1, 100, 1, SetOfPictures(_T(".\\Images\\Sun\\*.bmp")));
		SolarSystem->Add("Mercury", "Sun", 69, 46, 100, 0.8, 47, 24, 0, SetOfPictures(_T(".\\Images\\Mercury\\*.bmp")));
		SolarSystem->Add("Venus", "Sun", 107, 108, 200, 0.8, 35, 60, 0, SetOfPictures(_T(".\\Images\\Venus\\*.bmp")));
		SolarSystem->Add("Earth", "Sun", 152, 147, 270, 0.8, 29, 63, 0, SetOfPictures(_T(".\\Images\\Earth\\*.bmp")));
		SolarSystem->Add("Moon", "Earth", 30, 40, 40, 0.8, 80, 8, 0, SetOfPictures(_T(".\\Images\\Moon\\*.bmp")));
		DoubleBuffering->Release();
		SolarSystem->Go();
	}
		break;
	case WM_REPAINT:
	{
		DoubleBuffering->BackgroundUpdate();
		SS::SolarSystem::SolarSystemObject *temp;
		while (temp = SolarSystem->Get())
		{
			AddTrack(DoubleBuffering, temp);
			AddObject(DoubleBuffering, temp);
		}
		DoubleBuffering->Invalidate();
		delete temp;
	}
		break;
	case WM_PAINT:
	{
		DoubleBuffering->Paint();
		UpdateValues(SolarSystem);
		if 	((fabs(((3.141592653 * 2 - SolarSystem->Get("Earth")->GetT()) - (3.141592653 - SolarSystem->Get("Moon")->GetT())))<0.03)||
			(fabs(((3.141592653 * 2 - SolarSystem->Get("Moon")->GetT()) - (3.141592653 - SolarSystem->Get("Earth")->GetT())))<0.03))
			{
				SolarSystem->Stop();
				// окно паузы
				int msgboxID = MessageBox(NULL, (LPCWSTR)L"Затмение!", (LPCWSTR)L"Затмение", MB_ICONWARNING);
			
			}
	}
		break;
	case WM_DESTROY:
		delete SolarSystem;
		//delete DoubleBuffering;

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
	ButtonsInit(hWindow, hInstance, ResX, ResY);
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

void AddTrack(DB::DoubleBuffering *DoubleBuffering, SS::SolarSystem::SolarSystemObject *temp)
{
	RECT rc;
	GetClientRect(hWindow, &rc);
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	HDC hDC = CreateCompatibleDC(GetDC(hWindow));
	HBITMAP bmp1 = CreateCompatibleBitmap(hDC, rc.right - rc.left, rc.bottom - rc.top);

	(HBITMAP)SelectObject(hDC, bmp1);
	(HBRUSH)SelectObject(hDC, brush);
	(HPEN)SelectObject(hDC, pen);

	Ellipse(hDC, 0, 0, temp->::SSO::SolarSystemObject::GetA() * 2, temp->::SSO::SolarSystemObject::GetB() * 2);
	DoubleBuffering->AddObject(
			temp->GetA() + temp->::SSO::SolarSystemObject::GetApoapsis() + ResX / 2,
			temp->GetB() + ResY / 2,
			temp->::SSO::SolarSystemObject::GetA() * 2, temp->::SSO::SolarSystemObject::GetB() * 2, hDC,
			temp->::SSO::SolarSystemObject::GetA() * 2, temp->::SSO::SolarSystemObject::GetB() * 2, RGB(0, 255, 0));

	DeleteDC(hDC);
	DeleteBitmap(bmp1);
	DeletePen(pen);
	DeleteBrush(brush);
}

void AddObject(DB::DoubleBuffering *DoubleBuffering, SS::SolarSystem::SolarSystemObject *temp)
{
	DoubleBuffering->AddObject(temp->GetX() + ResX / 2, temp->GetY() + ResY / 2, temp->GetRadius(),
		temp->GetRadius(), temp->GetHDC(), temp->GetBmpWidth(), temp->GetBmpHeight(), RGB(0, 255, 0));
}