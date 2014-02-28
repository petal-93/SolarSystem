#pragma once
#ifndef _SOLAR_SYSTEM_OBJECT_
#define _SOLAR_SYSTEM_OBJECT_
#endif
#ifndef _VECTOR_
#include <vector>
#endif
#ifndef _WINDOWS_
#include <Windows.h>
#endif
namespace SSO
{
	class SolarSystemObject
	{
	private:
		int x, y;
		float t;
		//афелий
		int aphelion;
		//перигелий
		int perihelion;
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
		//bitmap's
		std::vector<HBITMAP> bitmaps();

		//коэффициент размера
		float sizeRatio;
		//коэффициент скорости
		float speedRatio;

		/// <summary> Пересчет координат (уравнение в параметрической форме) </summary>
		///	<param name = "t"> радиус вектор [0, 2*pi] </param>
		int Recalc(float t);

	public:
		SolarSystemObject() :x(0), y(0), aphelion(0), perihelion(0), a(0), e(0.1), speed(0), radius(0), isometric(false){};
		/// <summary> Конструктор </summary>
		/// <param name = "aphelion"> афелий </param>
		/// <param name = "perihelion"> перигелий </param>
		/// <param name = "a"> большая полуось </param>
		/// <param name = "e"> эксцентриситет </param>
		/// <param name = "t0"> начальное положение (радиус-вектор [0, 2*pi]) </param> 
		/// <param name = "radius"> средний радиус объекта </param>
		/// <param name = "sizeRatio"> пропорции размеров в процентах </param>
		/// <param name = "speedRatio"> коэффициент скорости в процентах </param>
		/// <param name = "isometric"> производить пересчет координат и размеров в изометрической проекции </param>
		SolarSystemObject(int aphelion, int perihelion, int a, float e, int speed, int radius,
						  bool isometric, float sizeRatio, float speedRatio, float t0);
		int getX() const;
		int getY() const;
		int getRadius() const;
		float getSpeedRatio() const;
		/// <summary> Установить коэффицент скорости </summary>
		/// <param name = "ratio"> коэффициент в процентах </param>
		void setSpeedRatio(float ratio);
		/// <summary> Установить пропорции </summary>
		/// <param name = "ratio"> коэффициент в процентах </param>
		void setSizeRatio(float ratio);
		/// <summary> Пересчет координат </summary>
		float Next();
		~SolarSystemObject();
	};
}