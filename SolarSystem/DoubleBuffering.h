#pragma once
#ifndef _DOUBLE_BUFFERING_
#define _DOUBLE_BUFFERING_
#endif
#ifndef _WINDOWS_
#include <Windows.h>
#endif
#ifndef _VECTOR_
#include <vector>
#endif
namespace DB
{
	class DoubleBuffering
	{
	private:
		HWND	hWindow;

		HDC		hDC,		//�������� ����
				hCopyDC,	//�������� � ������ - ��� �������������� �� ��
				hBackGrDC;	//�������� � �����

		RECT	rcClient,	//������������� � ���������� ��������
				rcWindow,	//�� ���� �����
				rcCurrent,	//������� �������������, ���� ������
				rcBackGr;	//� �����

		HBITMAP hBmpBackGr,		//����� ��������-����
				hBmpMask,		//�����
				//������ ������ ��� �������� ��� �������
				hBmpCopyClient, //����� ���������� �������
				hBmpTemp;		//���������

		PAINTSTRUCT PS;

		INT ResX, ResY;

		std::vector<HDC> hDCs;
		/// <summary> �������� ������ ������ ���� </summary>
		/// <param name = "xDest"> ���������� ������� �� DC ���������� </param>
		/// <param name = "yDest"> ���������� ������� �� DC ���������� </param>
		/// <param name = "widthDest"> ������ ������� �� DC ���������� </param>
		/// <param name = "heightDest"> ������ ������� �� DC ���������� </param>
		/// <param name = "hSrcDC"> DC ���������� ������ </param>
		/// <param name = "xSrc"> ���������� ������� �� �������� DC </param>
		/// <param name = "ySrc"> ���������� ������� �� �������� DC </param>
		/// <param name = "widthSrc"> ������ ������� �� �������� DC </param>
		/// <param name = "heightSrc"> ������ ������� �� �������� DC </param>
		/// <param name = "crTransparent"> ����, ������� ����� ��������� ���������� </param>
		void AddObject(int xDest, int yDest, int widthDest, int heightDest,
			HDC hSrcDC, int xSrc, int ySrc, int widthSrc, int heightSrc, UINT crTransparent);
	public:
		/// <summary> ����������� </summary>
		/// <param name = "hDC">  </param>
		/// <param name = "cx"> ������ ������� ��� ��������� ������� </param>
		/// <param name = "cy"> ������ ������� ��� ��������� ������� </param>
		DoubleBuffering(HWND hWindow, int cx, int cy);
		/// <summary> �������� � ��������� ��� </summary>
		/// <param name = "hBmp" �������� ���� </summary>
		void AddBackground(HBITMAP hBmp);
		/// <summary> ���������� �������� </summary>
		void Release();
		/// <summary> �������� ��� </summary>
		void BackgroundUpdate();
		/// <summary> �������� ������ ������ ���� </summary>
		/// <param name = "xDest"> ���������� ������� �� DC ���������� </param>
		/// <param name = "yDest"> ���������� ������� �� DC ���������� </param>
		/// <param name = "widthDest"> ������ ������� �� DC ���������� </param>
		/// <param name = "heightDest"> ������ ������� �� DC ���������� </param>
		/// <param name = "hSrcDC"> DC ���������� ������ </param>
		/// <param name = "widthSrc"> ������ ������� �� �������� DC </param>
		/// <param name = "heightSrc"> ������ ������� �� �������� DC </param>
		/// <param name = "crTransparent"> ����, ������� ����� ��������� ���������� </param>
		void AddObject(int xDest, int yDest, int widthDest, int heightDest,
			HDC hSrcDC, int widthSrc, int heightSrc, UINT crTransparent);
		/// <summary> �������� ��������� WM_PAINT </summary>
		void Invalidate();
		void Paint();
		~DoubleBuffering();
	};
}


