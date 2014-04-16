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
		//��������
		int apoapsis;
		//���������
		int periapsis;
		//������� �������
		int origA, a;
		//�����
		int origB, b;
		//��������
		float l;
		//��������������
		float origE, e;
		//����������� ��������
		int speed;
		//������� ������ �������
		int radius;

		//������ ��������������
		int projectionRadius;
		//�������������� ��������/ ��� ������
		float isometricRatio;
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
		SolarSystemObject() :x(0), y(0), apoapsis(0), periapsis(0), a(0), e(0.1), speed(0), radius(0),
			sizeRatio(1), speedRatio(1), distanceRatio(1), isometricRatio(1){};
		/// <summary> ����������� </summary>
		/// <param name = "apoapsis"> �������� </param>
		/// <param name = "periapsis"> ��������� </param>
		/// <param name = "a"> ������� ������� </param>
		/// <param name = "e"> �������������� </param>
		/// <param name = "t0"> ��������� ��������� (������-������ [0, 2*pi]) </param> 
		/// <param name = "radius"> ������� ������ ������� </param>
		/// <param name = "sizeRatio"> ��������� �������� � ��������� </param>
		/// <param name = "speedRatio"> ����������� �������� � ��������� </param>
		/// <param name = "distanceRatio"> ��������� ���������� � ��������� </param>
		/// <param name = "isometric"> ����������� �������� ��������� � �������� � �������������� �������� </param>
		SolarSystemObject(int apoapsis, int periapsis, int a, float e, int speed, int radius,
						  float isometricRatio, float sizeRatio, float speedRatio, float distanceRatio, float t0);
		/// <summary> ����������� </summary>
		/// <param name = "apoapsis"> �������� </param>
		/// <param name = "periapsis"> ��������� </param>
		/// <param name = "a"> ������� ������� </param>
		/// <param name = "e"> �������������� </param>
		/// <param name = "t0"> ��������� ��������� (������-������ [0, 2*pi]) </param> 
		/// <param name = "radius"> ������� ������ ������� </param>
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
		/// <param name = "ratio"> ����������� </param>
		void SetIsometricRatio(float ratio);
		/// <summary> ��������� �������� </summary>
		/// <param name = "speedRatio"> ����������� �������� � ��������� </param>
		/// <param name = "sizeRatio"> ����������� ������� � ��������� </param>
		/// <param name = "distanceRatio"> ����������� ���������� � ��������� </param>
		/// <param name = "isometricRatio"> ����������� </param>
		void Settings(float speedRatio, float sizeRatio, float distanceRatio, float isometricRatio);

		/// <summary> �������� ��������� </summary>
		float Next();
		virtual ~SolarSystemObject();
	};
}