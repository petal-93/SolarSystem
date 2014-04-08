#include "decorations.h"

HWND hButtonExit, hButtonSpeedUp, hButtonSpeedDown, hButtonSizeUp,
hButtonSizeDown, hButtonDistanceUp, hButtonDistanceDown, hButtonIsometricUp,
hButtonIsometricDown;
HWND hEditSpeed, hEditSize, hEditDistance, hEditIsometric;

int bWidth = 100, bHeigh = 20, bIntervalX = 5, bIntervalY = 5;

void ButtonsInit(HWND hWindow, HINSTANCE hInstance, int ResX, int RexY)
{
	hButtonExit = CreateWindowEx(NULL, L"BUTTON", L"Выход", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		ResX - 1 * (bWidth + bIntervalX), bIntervalY + 0 * (bHeigh + bIntervalY), bWidth, bHeigh,
		hWindow, (HMENU)IDC_MAIN_BUTTON_EXIT, GetModuleHandle(NULL), NULL);

	hEditSpeed = CreateWindow(L"Edit", L"speed ratio", WS_BORDER | WS_CHILD | WS_VISIBLE,
		ResX - 2 * (bWidth + bIntervalX), bIntervalY + 1 * (bHeigh + bIntervalY), bWidth*2 + bIntervalX, bHeigh,
		hWindow, (HMENU)IDC_SPEED_EDIT, GetModuleHandle(NULL), NULL);
	hButtonSpeedUp = CreateWindowEx(NULL, L"BUTTON", L"+", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		ResX - 1 * (bWidth + bIntervalX), bIntervalY + 2 * (bHeigh + bIntervalY), bWidth, bHeigh,
		hWindow, (HMENU)IDC_MAIN_BUTTON_SPEEDUP, GetModuleHandle(NULL), NULL);
	hButtonSpeedDown = CreateWindowEx(NULL, L"BUTTON", L"-", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		ResX - 2 * (bWidth + bIntervalX), bIntervalY + 2 * (bHeigh + bIntervalY), bWidth, bHeigh,
		hWindow, (HMENU)IDC_MAIN_BUTTON_SPEEDDOWN, GetModuleHandle(NULL), NULL);

	hEditSize = CreateWindow(L"Edit", L"size ratio", WS_BORDER | WS_CHILD | WS_VISIBLE,
		ResX - 2 * (bWidth + bIntervalX), bIntervalY + 3 * (bHeigh + bIntervalY), bWidth * 2 + bIntervalX, bHeigh,
		hWindow, (HMENU)IDC_SIZE_EDIT, GetModuleHandle(NULL), NULL);
	hButtonSizeUp = CreateWindowEx(NULL, L"BUTTON", L"+", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		ResX - 1 * (bWidth + bIntervalX), bIntervalY + 4 * (bHeigh + bIntervalY), bWidth, bHeigh,
		hWindow, (HMENU)IDC_MAIN_BUTTON_SIZEUP, GetModuleHandle(NULL), NULL);
	hButtonSizeDown = CreateWindowEx(NULL, L"BUTTON", L"-", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		ResX - 2 * (bWidth + bIntervalX), bIntervalY + 4 * (bHeigh + bIntervalY), bWidth, bHeigh,
		hWindow, (HMENU)IDC_MAIN_BUTTON_SIZEDOWN, GetModuleHandle(NULL), NULL);

	hEditDistance = CreateWindow(L"Edit", L"distance ratio", WS_BORDER | WS_CHILD | WS_VISIBLE,
		ResX - 2 * (bWidth + bIntervalX), bIntervalY + 5 * (bHeigh + bIntervalY), bWidth * 2 + bIntervalX, bHeigh,
		hWindow, (HMENU)IDC_DISTANCE_EDIT, GetModuleHandle(NULL), NULL);
	hButtonDistanceUp = CreateWindowEx(NULL, L"BUTTON", L"+", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		ResX - 1 * (bWidth + bIntervalX), bIntervalY + 6 * (bHeigh + bIntervalY), bWidth, bHeigh,
		hWindow, (HMENU)IDC_MAIN_BUTTON_DISTANCEUP, GetModuleHandle(NULL), NULL);
	hButtonDistanceDown = CreateWindowEx(NULL, L"BUTTON", L"-", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		ResX - 2 * (bWidth + bIntervalX), bIntervalY + 6 * (bHeigh + bIntervalY), bWidth, bHeigh,
		hWindow, (HMENU)IDC_MAIN_BUTTON_DISTANCEDOWN, GetModuleHandle(NULL), NULL);

	hEditIsometric = CreateWindow(L"Edit", L"isometric ratio", WS_BORDER | WS_CHILD | WS_VISIBLE,
		ResX - 2 * (bWidth + bIntervalX), bIntervalY + 7 * (bHeigh + bIntervalY), bWidth * 2 + bIntervalX, bHeigh,
		hWindow, (HMENU)IDC_SPEED_EDIT, GetModuleHandle(NULL), NULL);
	hButtonIsometricUp = CreateWindowEx(NULL, L"BUTTON", L"+", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		ResX - 1 * (bWidth + bIntervalX), bIntervalY + 8 * (bHeigh + bIntervalY), bWidth, bHeigh,
		hWindow, (HMENU)IDC_MAIN_BUTTON_ISOMETRICUP, GetModuleHandle(NULL), NULL);
	hButtonIsometricDown = CreateWindowEx(NULL, L"BUTTON", L"-", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		ResX - 2 * (bWidth + bIntervalX), bIntervalY + 8 * (bHeigh + bIntervalY), bWidth, bHeigh,
		hWindow, (HMENU)IDC_MAIN_BUTTON_ISOMETRICDOWN, GetModuleHandle(NULL), NULL);
}
// по клику на кнопку Выход
void OnClickButtonExit(HWND hWindow)
{
	PostMessage(hWindow, WM_QUIT, 0, 0);
}
//обновить значения текстбоксов
void UpdateValues(SS::SolarSystem *SolarSystem)
{
	wchar_t buffer[30];
	swprintf(buffer, 30, L"speed ratio: %f\0", SolarSystem->GetSpeedRatio());
	SetWindowText(hEditSpeed, buffer);

	swprintf(buffer, 30, L"size ratio: %f\0", SolarSystem->GetSizeRatio());
	SetWindowText(hEditSize, buffer);

	swprintf(buffer, 30, L"distance ratio: %f\0", SolarSystem->GetDistanceRatio());
	SetWindowText(hEditDistance, buffer);

	swprintf(buffer, 30, L"isometric ratio: %f\0", SolarSystem->GetIsometricRatio());
	SetWindowText(hEditIsometric, buffer);
}
// разбор WM_COMMAND (действия на щелчки по кнопкам)
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
			SolarSystem->SpeedInc(0.05);
		}
	}
	else if (hWndCtl == hButtonSpeedDown)
	{
		if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED))
		{
			SolarSystem->SpeedInc(-0.05);
		}
	}
	else if (hWndCtl == hButtonSizeUp)
	{
		if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED))
		{
			SolarSystem->SizeInc(0.01);
		}
	}
	else if (hWndCtl == hButtonSizeDown)
	{
		if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED))
		{
			SolarSystem->SizeInc(-0.01);
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
			SolarSystem->IsometricAngleInc(0.01);
		}
	}
	else if (hWndCtl == hButtonIsometricDown)
	{
		if ((codeNotify == BN_CLICKED) || (codeNotify == BN_DOUBLECLICKED))
		{
			SolarSystem->IsometricAngleInc(-0.01);
		}
	}
}