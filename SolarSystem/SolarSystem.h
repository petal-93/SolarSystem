#ifdef _MSC_VER
#pragma once
#endif
#define _X86_
#ifndef _SOLAR_SYSTEM_
#define _SOLAR_SYSTEM_
#endif
#ifndef _SOLAR_SYSTEM_OBJECT_
#include "SolarSystemObject.h"
#endif
#ifndef _MAP_
#include <map>
#endif
#ifndef _STRING_
#include <string>
#endif
#ifndef _WINDEF_
#include <windef.h>
#endif
#ifndef _PROCESSTHREADSAPI_H_
#include <processthreadsapi.h>
#endif
#ifndef _SYNCHAPI_H_
#include <synchapi.h>
#endif
#ifndef _WINBASE_
#include <WinBase.h>
#endif
#ifndef _WINUSER_
#include <WinUser.h>
#endif
#ifndef _WINGDI_
#include <wingdi.h>
#endif

#define WM_REPAINT WM_USER+100
#define WM_RECALC_OK WM_USER+101
#define WM_EXIT_THREAD WM_USER+102


namespace SS
{
	class SolarSystem
	{
	private:
		static DWORD WINAPI SolarSystemThread(LPVOID lpParam);
		static DWORD WINAPI SolarSystemThreadSync(LPVOID lpParam);
		HANDLE hSemaphoreRecalc, hSemaphoreStop;
		HWND hWindow;
		DWORD SyncThreadId;
		/*class SolarSystemObject;
		std::map<std::string, SS::SolarSystem::SolarSystemObject> theSystem;
		*/
		bool isometric;
		float sizeRatio, speedRatio, distanceRatio;
	public:
		//��������!!!
		class SolarSystemObject;
		std::map<std::string, SS::SolarSystem::SolarSystemObject> theSystem;
		//!!!!!!!!!!!

		SolarSystem();
		/// <summary> ����������� </summary>
		/// <param name = "sizeRatio"> ��������� �������� � ��������� </param>
		/// <param name = "speedRatio"> ����������� ��������� � ��������� </param>
		/// <param name = "distanceRatio"> ��������� ���������� � ��������� </param>
		/// <param name = "isometric"> ����������� �������� ��������� � �������� � �������������� �������� </param>
		SolarSystem(HWND hWindow, bool isometric, float sizeRatio, float speedRatio, float distanceRatio);
		/// <summary> �������� ������ </summary>
		/// <param name = "name"> �������� </param>
		/// <param name = "nameFocusBody"> ����������� ���� ������ (��������) </param>
		/// <param name = "apoapsis"> �������� </param>
		/// <param name = "periapsis"> ��������� </param>
		/// <param name = "a"> ������� ������� </param>
		/// <param name = "e"> �������������� </param>
		/// <param name = "t0"> ��������� ��������� (������-������ [0, 2*pi]) </param> 
		/// <param name = "radius"> ������� ������ ������� </param>
		/// <param name = "hBmp"> ����������� </param>
		void Add(std::string name, std::string nameFocusBody, int apoapsis, int periapsis, 
			int a, float e, int speed, int radius, float t0, HBITMAP hBmp);
		/// <summary> �������� ������ </summary>
		/// <param name = "name"> �������� </param>
		void Add(std::string name, SS::SolarSystem::SolarSystemObject);
		void Go();
		~SolarSystem();
	};

	class SS::SolarSystem::SolarSystemObject : public SSO::SolarSystemObject
	{
	private:
		//HBITMAP hBmp;
		//HANDLE hThread;
		//std::string name;
		//std::string nameFocusBody;
		//SS::SolarSystem *instance;
	public:
		std::string name;
		std::string nameFocusBody;
		SS::SolarSystem *instance;
		//��������!!!
		HDC hObjDC;
		HBITMAP hBmp;
		HANDLE hThread;
		//!!!!!!!!!!!
		
		SolarSystemObject(){ SSO::SolarSystemObject(); };
		/// <summary> ����������� </summary>
		/// <param name = "nameFocusBody"> ����������� ���� ������ (��������) </param>
		/// <param name = "apoapsis"> �������� </param>
		/// <param name = "periapsis"> ��������� </param>
		/// <param name = "a"> ������� ������� </param>
		/// <param name = "e"> �������������� </param>
		/// <param name = "t0"> ��������� ��������� (������-������ [0, 2*pi]) </param> 
		/// <param name = "radius"> ������� ������ ������� </param>
		/// <param name = "hBmp"> ����������� </param>
		//SolarSystemObject(std::string nameFocusBody, int apoapsis, int periapsis,
		//	int a, float e, int speed, int radius, float t0, HBITMAP hBmp);
		SolarSystemObject(SS::SolarSystem &instance, HWND hWindow, std::string name,
			std::string nameFocusBody, int apoapsis, int periapsis,
			int a, float e, int speed, int radius, float t0, HBITMAP hBmp);
		HANDLE GetHThread();
		void SetHThread(HANDLE hThread);
		//~SolarSystemObject();
	};
};


