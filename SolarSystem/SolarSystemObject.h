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
		//������
		int aphelion;
		//���������
		int perihelion;
		//������� �������
		int a;
		//�����
		int b;
		//��������
		float l;
		//��������������
		float e;
		//����������� ��������
		int speed;
		//������� ������ �������
		int radius;

		//������ ��������������
		int projectionRadius;
		//���� �������������� ��������/ ��� ������
		bool isometric;
		//bitmap's
		std::vector<HBITMAP> bitmaps();

		//����������� �������
		float sizeRatio;
		//����������� ��������
		float speedRatio;

		/// <summary> �������� ��������� (��������� � ��������������� �����) </summary>
		///	<param name = "t"> ������ ������ [0, 2*pi] </param>
		int Recalc(float t);

	public:
		SolarSystemObject() :x(0), y(0), aphelion(0), perihelion(0), a(0), e(0.1), speed(0), radius(0), isometric(false){};
		/// <summary> ����������� </summary>
		/// <param name = "aphelion"> ������ </param>
		/// <param name = "perihelion"> ��������� </param>
		/// <param name = "a"> ������� ������� </param>
		/// <param name = "e"> �������������� </param>
		/// <param name = "t0"> ��������� ��������� (������-������ [0, 2*pi]) </param> 
		/// <param name = "radius"> ������� ������ ������� </param>
		/// <param name = "sizeRatio"> ��������� �������� � ��������� </param>
		/// <param name = "speedRatio"> ����������� �������� � ��������� </param>
		/// <param name = "isometric"> ����������� �������� ��������� � �������� � �������������� �������� </param>
		SolarSystemObject(int aphelion, int perihelion, int a, float e, int speed, int radius,
						  bool isometric, float sizeRatio, float speedRatio, float t0);
		int getX() const;
		int getY() const;
		int getRadius() const;
		float getSpeedRatio() const;
		/// <summary> ���������� ���������� �������� </summary>
		/// <param name = "ratio"> ����������� � ��������� </param>
		void setSpeedRatio(float ratio);
		/// <summary> ���������� ��������� </summary>
		/// <param name = "ratio"> ����������� � ��������� </param>
		void setSizeRatio(float ratio);
		/// <summary> �������� ��������� </summary>
		float Next();
		~SolarSystemObject();
	};
}