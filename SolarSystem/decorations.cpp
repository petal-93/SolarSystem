#include "decorations.h"

HWND hButtonExit, hButtonSpeedUp, hButtonSpeedDown, hButtonSizeUp,
hButtonSizeDown, hButtonDistanceUp, hButtonDistanceDown, hButtonIsometricUp,
hButtonIsometricDown;

void ButtonsInit(HWND hWindow, HINSTANCE hInstance, int ResX, int RexY)
{
	hButtonExit = CreateWindowEx(NULL, L"BUTTON", L"�����", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		ResX - 150, 10, 100, 20, hWindow, (HMENU)IDC_MAIN_BUTTON_EXIT, GetModuleHandle(NULL), NULL);
	hButtonSpeedUp = CreateWindowEx(NULL, L"BUTTON", L"+", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		ResX - 100, 60, 20, 20, hWindow, (HMENU)IDC_MAIN_BUTTON_SPEEDUP, GetModuleHandle(NULL), NULL);
	hButtonSpeedDown = CreateWindowEx(NULL, L"BUTTON", L"-", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		ResX - 150, 60, 20, 20, hWindow, (HMENU)IDC_MAIN_BUTTON_SPEEDDOWN, GetModuleHandle(NULL), NULL);
	hButtonSizeUp = CreateWindowEx(NULL, L"BUTTON", L"+", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		ResX - 100, 110, 20, 20, hWindow, (HMENU)IDC_MAIN_BUTTON_SPEEDUP, GetModuleHandle(NULL), NULL);
	hButtonSizeDown = CreateWindowEx(NULL, L"BUTTON", L"-", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		ResX - 150, 110, 20, 20, hWindow, (HMENU)IDC_MAIN_BUTTON_SPEEDDOWN, GetModuleHandle(NULL), NULL);
	hButtonDistanceUp = CreateWindowEx(NULL, L"BUTTON", L"+", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		ResX - 100, 160, 20, 20, hWindow, (HMENU)IDC_MAIN_BUTTON_SPEEDUP, GetModuleHandle(NULL), NULL);
	hButtonDistanceDown = CreateWindowEx(NULL, L"BUTTON", L"-", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		ResX - 150, 190, 20, 20, hWindow, (HMENU)IDC_MAIN_BUTTON_SPEEDDOWN, GetModuleHandle(NULL), NULL);
	hButtonIsometricUp = CreateWindowEx(NULL, L"BUTTON", L"+", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		ResX - 100, 210, 20, 20, hWindow, (HMENU)IDC_MAIN_BUTTON_SPEEDUP, GetModuleHandle(NULL), NULL);
	hButtonIsometricDown = CreateWindowEx(NULL, L"BUTTON", L"-", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		ResX - 150, 240, 20, 20, hWindow, (HMENU)IDC_MAIN_BUTTON_SPEEDDOWN, GetModuleHandle(NULL), NULL);
	
	HWND hEdit = CreateWindow(L"Edit", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | NULL | NULL, 10, 10, 200, 20, hWindow, NULL, hInstance, 0);
}
// �� ����� �� ������ �����
void OnClickButtonExit(HWND hWindow)
{
	PostMessage(hWindow, WM_QUIT, 0, 0);
}
// ������ WM_COMMAND (�������� �� ������ �� �������)
void OnCommand(HWND hWnd, HWND hWndCtl, UINT codeNotify, SS::SolarSystem *SolarSystem)
{
	if (hWndCtl == hButtonExit)
	{
		if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED))
		{
			OnClickButtonExit(hWnd);
		}
	}
	else if (hWndCtl == hButtonSpeedUp)
	{
		if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED))
		{
			SolarSystem->SpeedInc(0.5);
		}
	}
	else if (hWndCtl == hButtonSpeedDown)
	{
		if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED))
		{
			SolarSystem->SpeedInc(-0.5);
		}
	}
	else if (hWndCtl == hButtonSizeUp)
	{
		if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED))
		{
			SolarSystem->SizeInc(0.1);
		}
	}
	else if (hWndCtl == hButtonSizeDown)
	{
		if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED))
		{
			SolarSystem->SizeInc(-0.1);
		}
	}
	else if (hWndCtl == hButtonDistanceUp)
	{
		if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED))
		{
			SolarSystem->DistanceInc(0.01);
		}
	}
	else if (hWndCtl == hButtonDistanceDown)
	{
		if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED))
		{
			SolarSystem->DistanceInc(-0.01);
		}
	}
	else if (hWndCtl == hButtonIsometricUp)
	{
		if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED))
		{
			SolarSystem->IsometricAngleInc(0.05);
		}
	}
	else if (hWndCtl == hButtonIsometricDown)
	{
		if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED))
		{
			SolarSystem->IsometricAngleInc(-0.05);
		}
	}
}