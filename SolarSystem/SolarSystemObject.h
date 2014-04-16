#pragma once
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
		int origA, a;
		//малая
		int origB, b;
		//периметр
		float l;
		//эксцентриситет
		float origE, e;
		//орбитальная скорость
		int speed;
		//средний радиус объекта
		int radius;

		//радиус изометрический
		int projectionRadius;
		//изометрическая проекция/ вид сверху
		float isometricRatio;
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
		SolarSystemObject() :x(0), y(0), apoapsis(0), periapsis(0), a(0), e(0.1), speed(0), radius(0),
			sizeRatio(1), speedRatio(1), distanceRatio(1), isometricRatio(1){};
		/// <summary> Конструктор </summary>
		/// <param name = "apoapsis"> апоцентр </param>
		/// <param name = "periapsis"> перицентр </param>
		/// <param name = "a"> большая полуось </param>
		/// <param name = "e"> эксцентриситет </param>
		/// <param name = "t0"> начальное положение (радиус-вектор [0, 2*pi]) </param> 
		/// <param name = "radius"> средний радиус объекта </param>
		/// <param name = "sizeRatio"> пропорции размеров в процентах </param>
		/// <param name = "speedRatio"> коэффициент скорости в процентах </param>
		/// <param name = "distanceRatio"> пропорции расстояний в процентах </param>
		/// <param name = "isometric"> производить пересчет координат и размеров в изометрической проекции </param>
		SolarSystemObject(int apoapsis, int periapsis, int a, float e, int speed, int radius,
						  float isometricRatio, float sizeRatio, float speedRatio, float distanceRatio, float t0);
		/// <summary> Конструктор </summary>
		/// <param name = "apoapsis"> апоцентр </param>
		/// <param name = "periapsis"> перицентр </param>
		/// <param name = "a"> большая полуось </param>
		/// <param name = "e"> эксцентриситет </param>
		/// <param name = "t0"> начальное положение (радиус-вектор [0, 2*pi]) </param> 
		/// <param name = "radius"> средний радиус объекта </param>
		SolarSystemObject(int apoapsis, int periapsis, int a, float e, int speed, int radius, float t0);
		virtual int GetX() const;
		virtual int GetY() const;
		virtual int GetA() const;
		virtual int GetB() const;
		virtual int GetApoapsis() const;
		int GetRadius() const;
		float GetSpeedRatio() const;
		float GetSizeRatio() const;
		float GetDistanceRatio() const;
		float GetIsometricRatio() const;
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
		/// <param name = "ratio"> коэффициент </param>
		void SetIsometricRatio(float ratio);
		/// <summary> Изменение настроек </summary>
		/// <param name = "speedRatio"> коэффициент скорости в процентах </param>
		/// <param name = "sizeRatio"> коэффициент размера в процентах </param>
		/// <param name = "distanceRatio"> коэффициент расстояния в процентах </param>
		/// <param name = "isometricRatio"> коэффициент </param>
		void Settings(float speedRatio, float sizeRatio, float distanceRatio, float isometricRatio);

		/// <summary> Пересчет координат </summary>
		float Next();
		virtual ~SolarSystemObject();
	};
}