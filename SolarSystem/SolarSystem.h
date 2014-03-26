#pragma once
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
#ifndef _VECTOR_
#include <vector>
#endif
#ifndef _WINDOWS_
#include <Windows.h>
#endif

#define WM_REPAINT WM_USER+100
#define WM_RECALC_OK WM_USER+101
#define WM_EXIT_THREAD WM_USER+102


namespace SS
{
	class SolarSystem
	{
	public:
		// ����� ���������� ���������� �������������� � ������ � ��������
		class SolarSystemObject;
	private:
		/// <summary> Callback ������� ��� ������ </summary>
		static DWORD WINAPI SolarSystemThread(LPVOID lpParam);
		/// <summary> Callback ������� ��� ���������������� ������ </summary>
		static DWORD WINAPI SolarSystemThreadSync(LPVOID lpParam);
		// ������� ��������� ������ ��������� 
		HANDLE hSemaphoreRecalc;
		// �������  
		HANDLE hSemaphoreStop;
		// ����� ����, ����� ��� �������� � ���� ��������� � ���������� ���������
		HWND hWindow;
		// �� ����������������� ������, ����� ��� �������� � ���� ��������� �� ��������� �������
		DWORD SyncThreadId;
		// ����� �������� ��� ���� ��������
		float sizeRatio, speedRatio, distanceRatio, isometricRatio;
		// ������������� ������ � ���������
		std::map<std::string, SS::SolarSystem::SolarSystemObject> theSystem;
	public:
		/// <summary> ����������� </summary>
		SolarSystem();
		/// <summary> ����������� </summary>
		/// <param name = "sizeRatio"> ��������� �������� � ��������� </param>
		/// <param name = "speedRatio"> ����������� ��������� � ��������� </param>
		/// <param name = "distanceRatio"> ��������� ���������� � ��������� </param>
		/// <param name = "isometricRatio"> �������� ��������� � �������� � �������������� �������� </param>
		SolarSystem(HWND hWindow, float isometricRatio, float sizeRatio, float speedRatio, float distanceRatio);
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
		/// <param name = "nameFocusBody"> ����������� ���� ������ (��������) </param>
		/// <param name = "apoapsis"> �������� </param>
		/// <param name = "periapsis"> ��������� </param>
		/// <param name = "a"> ������� ������� </param>
		/// <param name = "e"> �������������� </param>
		/// <param name = "t0"> ��������� ��������� (������-������ [0, 2*pi]) </param> 
		/// <param name = "radius"> ������� ������ ������� </param>
		/// <param name = "hBmp"> ����������� </param>
		void Add(std::string name, std::string nameFocusBody, int apoapsis, int periapsis,
			int a, float e, int speed, int radius, float t0, std::vector<HBITMAP> hBmps);
		/// <summary> �������� ������ </summary>
		/// <param name = "name"> �������� </param>
		/// <param name = "object"> ������ </param>
		void Add(std::string name, SS::SolarSystem::SolarSystemObject object);
		/// <summary> �������� ������ </summary>
		/// <param name = "name" ��� ������� </param>
		SS::SolarSystem::SolarSystemObject* Get(std::string name);
		/// <summary> ����� ������� </summary>
		void Go();
		/// <summary> ��������� �������� </summary>
		/// <param name = "ratioInc" ����������� </param>
		void SpeedInc(float ratioInc);
		/// <summary> ��������� �������� </summary>
		/// <param name = "ratioInc" ����������� </param>
		void SizeInc(float ratioInc);
		/// <summary> ��������� ���������� </summary>
		/// <param name = "ratioInc" ����������� </param>
		void DistanceInc(float ratioInc);
		/// <summary> ��������� ������� �������� </summary>
		/// <param name = "ratioInc" ����������� </param>
		void IsometricAngleInc(float ratioInc);
		~SolarSystem();
	};

	class SS::SolarSystem::SolarSystemObject : public SSO::SolarSystemObject
	{
	private:
		// ��� �������
		std::string name;
		// ��� ��������� ���� ������
		std::string nameFocusBody;
		// ��������� �� ���������� �����, ������������ ��� �������������� ������� ����� ������
		SS::SolarSystem *instance;
		// ����� ������ ���������� � ��������
		HANDLE hThread;
		// �������� + ��������� ����������
		std::vector<std::pair<HBITMAP, HDC>> hBmps;
		// ������� ��������
		int curr;
		friend SS::SolarSystem;
	public:
		/// <summary> ����������� </summary>
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
		SolarSystemObject(SS::SolarSystem &instance, std::string name,
			std::string nameFocusBody, int apoapsis, int periapsis,
			int a, float e, int speed, int radius, float t0, HBITMAP hBmp);
		/// <summary> ����������� </summary>
		/// <param name = "nameFocusBody"> ����������� ���� ������ (��������) </param>
		/// <param name = "apoapsis"> �������� </param>
		/// <param name = "periapsis"> ��������� </param>
		/// <param name = "a"> ������� ������� </param>
		/// <param name = "e"> �������������� </param>
		/// <param name = "t0"> ��������� ��������� (������-������ [0, 2*pi]) </param> 
		/// <param name = "radius"> ������� ������ ������� </param>
		/// <param name = "hBmp"> ����������� </param>
		SolarSystemObject(SS::SolarSystem &instance, std::string name,
			std::string nameFocusBody, int apoapsis, int periapsis,
			int a, float e, int speed, int radius, float t0, std::vector<HBITMAP> hBmp);
		/// <summary> �������� ���������� �� Ox </summary>
		int GetX() const;
		/// <summary> �������� ���������� �� Oy </summary>
		int GetY() const;
		/// <summary> �������� ����� ������ </summary>
		HANDLE GetHThread() const;
		/// <summary> �������� ����� ������ </summary>
		/// <param name = "hTread" ����� ������ </summary>
		void SetHThread(HANDLE hThread);
		/// <summary> �������� �������� </summary>
		/// <param name = "hBmp"> ����������� </param>
		void AddHBmp(HBITMAP hBmp);
		/// <summary> �������� ��������� �������� ���������� </summary>
		HDC GetHDC();
		/// <summary> �������� ��������� �������� </summary>
		HBITMAP GetHBmp() const;
		/// <summary> �������� ������ �������� </summary>
		int GetBmpWidth() const;
		/// <summary> �������� ������ �������� </summary>
		int GetBmpHeight() const;
		//~SolarSystemObject();
	};
};


