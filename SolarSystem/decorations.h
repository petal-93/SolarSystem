#pragma once
#ifndef _DECORATIONS_
#define _DECORATIONS_
#ifndef _WINDOWS_
#include <Windows.h>
#endif
#ifndef _SOLAR_SYSTEM_
#include "SolarSystem.h"
#endif

#define IDC_MAIN_BUTTON_EXIT 300+WM_USER
#define IDC_MAIN_BUTTON_SPEEDUP 205+WM_USER
#define IDC_MAIN_BUTTON_SPEEDDOWN 206+WM_USER
#define IDC_MAIN_BUTTON_SIZEUP 207+WM_USER
#define IDC_MAIN_BUTTON_SIZEDOWN 208+WM_USER
#define IDC_MAIN_BUTTON_DISTANCEUP 209+WM_USER
#define IDC_MAIN_BUTTON_DISTANCEDOWN 210+WM_USER
#define IDC_MAIN_BUTTON_ISOMETRICUP 211+WM_USER
#define IDC_MAIN_BUTTON_ISOMETRICDOWN 212+WM_USER

extern HWND hButtonExit, hButtonSpeedUp, hButtonSpeedDown, hButtonSizeUp,
hButtonSizeDown, hButtonDistanceUp, hButtonDistanceDown, hButtonIsometricUp,
hButtonIsometricDown;
// инициализация кнопок управления
void ButtonsInit(HWND hWindow, int ResX, int ResY);
// по клику на кнопку Выход
void OnClickButtonExit(HWND hWindow);
// разбор WM_COMMAND (действия на щелчки по кнопкам)
void OnCommand(HWND hWnd, HWND hWndCtl, UINT codeNotify, SS::SolarSystem* SolarSystem);
#endif