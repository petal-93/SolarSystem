#ifdef _MSC_VER
#pragma once
#endif
#ifndef _SOLAR_SYSTEM_OBJECT_
#define _SOLAR_SYSTEM_OBJECT_
#endif
#ifndef _VECTOR_
#include <vector>
#endif
namespace SSO
{
	class SolarSystemObject
	{
	private:
		int x, y;
		float t;
		//апоцентр
		int apoapsis;
		//перицентр
		int periapsis;
		//большая полуось
		int a;
		//малая
		int b;
		//периметр
		float l;
		//эксцентриситет
		float e;
		//орбитальная скорость
		int speed;
		//средний радиус объекта
		int radius;

		//радиус изометрический
		int projectionRadius;
		//флаг изометрическая проекция/ вид сверху
		bool isometric;

		//коэффициент размера
		float sizeRatio;
		//коэффициент скорости
		float speedRatio;
		//коэффициент расстояния
		float distanceRatio;

		/// <summary> Пересчет координат (уравнение в параметрической форме) </summary>
		///	<param name = "t"> радиус вектор [0, 2*pi] </param>
		int Recalc(float t);

	public:
		SolarSystemObject() :x(0), y(0), apoapsis(0), periapsis(0), a(0), e(0.1), speed(0), radius(0), isometric(true){};
		/// <summary> Конструктор </summary>
		/// <param name = "apoapsis"> апоцентр </param>
		/// <param name = "periapsis"> перицентр </param>
		/// <param name = "a"> большая полуось </param>
		/// <param name = "e"> эксцентриситет </param>
		/// <param name = "t0"> начальное положение (радиус-вектор [0, 2*pi]) </param> 
		/// <param name = "radius"> средний радиус объекта </param>
		/// <param name = "sizeRatio"> пропорции размеров в процентах </param>
		/// <param name = "speedRatio"> коэффициент скорости в процентах </param>
		/// <param name = "distanceRatio" пропорции расстояний в процентах </param>
		/// <param name = "isometric"> производить пересчет координат и размеров в изометрической проекции </param>
		SolarSystemObject(int apoapsis, int periapsis, int a, float e, int speed, int radius,
						  bool isometric, float sizeRatio, float speedRatio, float distanceRatio, float t0);
		/// <summary> Конструктор </summary>
		/// <param name = "apoapsis"> апоцентр </param>
		/// <param name = "periapsis"> перицентр </param>
		/// <param name = "a"> большая полуось </param>
		/// <param name = "e"> эксцентриситет </param>
		/// <param name = "t0"> начальное положение (радиус-вектор [0, 2*pi]) </param> 
		/// <param name = "radius"> средний радиус объекта </param>
		SolarSystemObject(int apoapsis, int periapsis, int a, float e, int speed, int radius, float t0);
		int GetX() const;
		int GetY() const;
		int GetRadius() const;
		float GetSpeedRatio() const;
		/// <summary> Установить коэффицент скорости </summary>
		/// <param name = "ratio"> коэффициент в процентах </param>
		void SetSpeedRatio(float ratio);
		/// <summary> Установить пропорции размеров </summary>
		/// <param name = "ratio"> коэффициент в процентах </param>
		void SetSizeRatio(float ratio);
		/// <summary> Установить пропорции расстояний </summary>
		/// <param name = "ratio"> коэффициент в процентах </param>
		void SetDistanceRatio(float ratio);
		/// <Summary> Настройка проекции </summary>
		/// <param name = "isometric"> производить пересчет координат и размеров в изометрической проекции </param>
		void SetIsometric(bool isometric);
		/// <summary> Изменение настроек </summary>
		/// <param name = "speedRatio"> коэффициент скорости в процентах </param>
		/// <param name = "sizeRatio"> коэффициент размера в процентах </param>
		/// <param name = "distanceRatio"> коэффициент расстояния в процентах </param>
		/// <param name = "isometric"> производить пересчет координат и размеров в изометрической проекции </param>
		void Settings(float speedRatio, float sizeRatio, float distanceRatio, bool isometric);

		/// <summary> Пересчет координат </summary>
		float Next();
		virtual ~SolarSystemObject();
	};
}