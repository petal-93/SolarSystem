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
		// класс объединяет физические характеристики и работу с потоками
		class SolarSystemObject;
	private:
		/// <summary> Callback функция для потока </summary>
		static DWORD WINAPI SolarSystemThread(LPVOID lpParam);
		/// <summary> Callback функция для синхронизирущего потока </summary>
		static DWORD WINAPI SolarSystemThreadSync(LPVOID lpParam);
		// семафор объявляет начало пересчета 
		HANDLE hSemaphoreRecalc;
		// семафор  
		HANDLE hSemaphoreStop;
		// хендл окна, нужен для отправки в него сообщения о завершении пересчета
		HWND hWindow;
		// ИД синхронизирующего потока, нужен для отправки в него сообщений из остальных потоков
		DWORD SyncThreadId;
		// общие натройки для всех объектов
		float sizeRatio, speedRatio, distanceRatio, isometricRatio;
		// ассоциативный массив с объектами
		std::map<std::string, SS::SolarSystem::SolarSystemObject> theSystem;
	public:
		/// <summary> Конструктор </summary>
		SolarSystem();
		/// <summary> Конструктор </summary>
		/// <param name = "sizeRatio"> пропорции размеров в процентах </param>
		/// <param name = "speedRatio"> коэффициент скоростей в процентах </param>
		/// <param name = "distanceRatio"> пропорции расстояний в процентах </param>
		/// <param name = "isometricRatio"> пересчет координат и размеров в изометрической проекции </param>
		SolarSystem(HWND hWindow, float isometricRatio, float sizeRatio, float speedRatio, float distanceRatio);
		/// <summary> Добавить объект </summary>
		/// <param name = "name"> название </param>
		/// <param name = "nameFocusBody"> центральное тело орбиты (фокусное) </param>
		/// <param name = "apoapsis"> апоцентр </param>
		/// <param name = "periapsis"> перицентр </param>
		/// <param name = "a"> большая полуось </param>
		/// <param name = "e"> эксцентриситет </param>
		/// <param name = "t0"> начальное положение (радиус-вектор [0, 2*pi]) </param> 
		/// <param name = "radius"> средний радиус объекта </param>
		/// <param name = "hBmp"> изображение </param>
		void Add(std::string name, std::string nameFocusBody, int apoapsis, int periapsis,
			int a, float e, int speed, int radius, float t0, HBITMAP hBmp);
		/// <summary> Добавить объект </summary>
		/// <param name = "name"> название </param>
		/// <param name = "nameFocusBody"> центральное тело орбиты (фокусное) </param>
		/// <param name = "apoapsis"> апоцентр </param>
		/// <param name = "periapsis"> перицентр </param>
		/// <param name = "a"> большая полуось </param>
		/// <param name = "e"> эксцентриситет </param>
		/// <param name = "t0"> начальное положение (радиус-вектор [0, 2*pi]) </param> 
		/// <param name = "radius"> средний радиус объекта </param>
		/// <param name = "hBmp"> изображение </param>
		void Add(std::string name, std::string nameFocusBody, int apoapsis, int periapsis,
			int a, float e, int speed, int radius, float t0, std::vector<HBITMAP> hBmps);
		/// <summary> Добавить объект </summary>
		/// <param name = "name"> название </param>
		/// <param name = "object"> объект </param>
		void Add(std::string name, SS::SolarSystem::SolarSystemObject object);
		/// <summary> Получить объект </summary>
		/// <param name = "name" имя объекта </param>
		SS::SolarSystem::SolarSystemObject* Get(std::string name);
		/// <summary> Старт потоков </summary>
		void Go();
		/// <summary> Изменение скорости </summary>
		/// <param name = "ratioInc" коэффициент </param>
		void SpeedInc(float ratioInc);
		/// <summary> Изменение размеров </summary>
		/// <param name = "ratioInc" коэффициент </param>
		void SizeInc(float ratioInc);
		/// <summary> Изменение расстояний </summary>
		/// <param name = "ratioInc" коэффициент </param>
		void DistanceInc(float ratioInc);
		/// <summary> Изменение наклона проекции </summary>
		/// <param name = "ratioInc" коэффициент </param>
		void IsometricAngleInc(float ratioInc);
		~SolarSystem();
	};

	class SS::SolarSystem::SolarSystemObject : public SSO::SolarSystemObject
	{
	private:
		// имя объекта
		std::string name;
		// имя фокусного тела орбиты
		std::string nameFocusBody;
		// указатель на породивший класс, используется для взаимодействия потоков через объкты
		SS::SolarSystem *instance;
		// хендл потока связанного с объектом
		HANDLE hThread;
		// картинки + контексты устройства
		std::vector<std::pair<HBITMAP, HDC>> hBmps;
		// текущая картинка
		int curr;
		friend SS::SolarSystem;
	public:
		/// <summary> Конструктор </summary>
		SolarSystemObject(){ SSO::SolarSystemObject(); };
		/// <summary> Конструктор </summary>
		/// <param name = "nameFocusBody"> центральное тело орбиты (фокусное) </param>
		/// <param name = "apoapsis"> апоцентр </param>
		/// <param name = "periapsis"> перицентр </param>
		/// <param name = "a"> большая полуось </param>
		/// <param name = "e"> эксцентриситет </param>
		/// <param name = "t0"> начальное положение (радиус-вектор [0, 2*pi]) </param> 
		/// <param name = "radius"> средний радиус объекта </param>
		/// <param name = "hBmp"> изображение </param>
		SolarSystemObject(SS::SolarSystem &instance, std::string name,
			std::string nameFocusBody, int apoapsis, int periapsis,
			int a, float e, int speed, int radius, float t0, HBITMAP hBmp);
		/// <summary> Конструктор </summary>
		/// <param name = "nameFocusBody"> центральное тело орбиты (фокусное) </param>
		/// <param name = "apoapsis"> апоцентр </param>
		/// <param name = "periapsis"> перицентр </param>
		/// <param name = "a"> большая полуось </param>
		/// <param name = "e"> эксцентриситет </param>
		/// <param name = "t0"> начальное положение (радиус-вектор [0, 2*pi]) </param> 
		/// <param name = "radius"> средний радиус объекта </param>
		/// <param name = "hBmp"> изображение </param>
		SolarSystemObject(SS::SolarSystem &instance, std::string name,
			std::string nameFocusBody, int apoapsis, int periapsis,
			int a, float e, int speed, int radius, float t0, std::vector<HBITMAP> hBmp);
		/// <summary> Получить координату по Ox </summary>
		int GetX() const;
		/// <summary> Получить координату по Oy </summary>
		int GetY() const;
		/// <summary> Получить хендл потока </summary>
		HANDLE GetHThread() const;
		/// <summary> Записать хендл потока </summary>
		/// <param name = "hTread" хендл потока </summary>
		void SetHThread(HANDLE hThread);
		/// <summary> Добавить картинку </summary>
		/// <param name = "hBmp"> изображение </param>
		void AddHBmp(HBITMAP hBmp);
		/// <summary> Получить следующий контекст устройства </summary>
		HDC GetHDC();
		/// <summary> Получить следующую картинку </summary>
		HBITMAP GetHBmp() const;
		/// <summary> Получить ширину картинки </summary>
		int GetBmpWidth() const;
		/// <summary> Получить высоту картинки </summary>
		int GetBmpHeight() const;
		//~SolarSystemObject();
	};
};


