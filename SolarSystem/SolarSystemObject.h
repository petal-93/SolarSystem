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
		//��������
		int apoapsis;
		//���������
		int periapsis;
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

		//����������� �������
		float sizeRatio;
		//����������� ��������
		float speedRatio;
		//����������� ����������
		float distanceRatio;

		/// <summary> �������� ��������� (��������� � ��������������� �����) </summary>
		///	<param name = "t"> ������ ������ [0, 2*pi] </param>
		int Recalc(float t);

	public:
		SolarSystemObject() :x(0), y(0), apoapsis(0), periapsis(0), a(0), e(0.1), speed(0), radius(0), isometric(true){};
		/// <summary> ����������� </summary>
		/// <param name = "apoapsis"> �������� </param>
		/// <param name = "periapsis"> ��������� </param>
		/// <param name = "a"> ������� ������� </param>
		/// <param name = "e"> �������������� </param>
		/// <param name = "t0"> ��������� ��������� (������-������ [0, 2*pi]) </param> 
		/// <param name = "radius"> ������� ������ ������� </param>
		/// <param name = "sizeRatio"> ��������� �������� � ��������� </param>
		/// <param name = "speedRatio"> ����������� �������� � ��������� </param>
		/// <param name = "distanceRatio" ��������� ���������� � ��������� </param>
		/// <param name = "isometric"> ����������� �������� ��������� � �������� � �������������� �������� </param>
		SolarSystemObject(int apoapsis, int periapsis, int a, float e, int speed, int radius,
						  bool isometric, float sizeRatio, float speedRatio, float distanceRatio, float t0);
		/// <summary> ����������� </summary>
		/// <param name = "apoapsis"> �������� </param>
		/// <param name = "periapsis"> ��������� </param>
		/// <param name = "a"> ������� ������� </param>
		/// <param name = "e"> �������������� </param>
		/// <param name = "t0"> ��������� ��������� (������-������ [0, 2*pi]) </param> 
		/// <param name = "radius"> ������� ������ ������� </param>
		SolarSystemObject(int apoapsis, int periapsis, int a, float e, int speed, int radius, float t0);
		int GetX() const;
		int GetY() const;
		int GetRadius() const;
		float GetSpeedRatio() const;
		/// <summary> ���������� ���������� �������� </summary>
		/// <param name = "ratio"> ����������� � ��������� </param>
		void SetSpeedRatio(float ratio);
		/// <summary> ���������� ��������� �������� </summary>
		/// <param name = "ratio"> ����������� � ��������� </param>
		void SetSizeRatio(float ratio);
		/// <summary> ���������� ��������� ���������� </summary>
		/// <param name = "ratio"> ����������� � ��������� </param>
		void SetDistanceRatio(float ratio);
		/// <Summary> ��������� �������� </summary>
		/// <param name = "isometric"> ����������� �������� ��������� � �������� � �������������� �������� </param>
		void SetIsometric(bool isometric);
		/// <summary> ��������� �������� </summary>
		/// <param name = "speedRatio"> ����������� �������� � ��������� </param>
		/// <param name = "sizeRatio"> ����������� ������� � ��������� </param>
		/// <param name = "distanceRatio"> ����������� ���������� � ��������� </param>
		/// <param name = "isometric"> ����������� �������� ��������� � �������� � �������������� �������� </param>
		void Settings(float speedRatio, float sizeRatio, float distanceRatio, bool isometric);

		/// <summary> �������� ��������� </summary>
		float Next();
		virtual ~SolarSystemObject();
	};
}