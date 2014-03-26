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

		HDC		hDC,		//контекст окна
				hCopyDC,	//контекст в памяти - все отрисовывается на нём
				hBackGrDC;	//контекст с фоном

		RECT	rcClient,	//прямоугольник с клиентской областью
				rcWindow,	//со всем окном
				rcCurrent,	//текущий прямоугольник, куда рисуем
				rcBackGr;	//с фоном

		HBITMAP hBmpBackGr,		//хэндл картинки-фона
				hBmpMask,		//маска
				//пустой битмап для хранения всй картины
				hBmpCopyClient, //копия клиентской области
				hBmpTemp;		//временный

		PAINTSTRUCT PS;

		INT ResX, ResY;

		std::vector<HDC> hDCs;
		/// <summary> Добавить объект поверх фона </summary>
		/// <param name = "xDest"> координата объекта на DC назначения </param>
		/// <param name = "yDest"> координата объекта на DC назначения </param>
		/// <param name = "widthDest"> ширина объекта на DC назначения </param>
		/// <param name = "heightDest"> высота объекта на DC назначения </param>
		/// <param name = "hSrcDC"> DC содержащий объект </param>
		/// <param name = "xSrc"> координата объекта на исходном DC </param>
		/// <param name = "ySrc"> координата объекта на исходном DC </param>
		/// <param name = "widthSrc"> ширина объекта на исходном DC </param>
		/// <param name = "heightSrc"> высота объекта на исходном DC </param>
		/// <param name = "crTransparent"> цвет, который будет считаться прозрачным </param>
		void AddObject(int xDest, int yDest, int widthDest, int heightDest,
			HDC hSrcDC, int xSrc, int ySrc, int widthSrc, int heightSrc, UINT crTransparent);
	public:
		/// <summary> Конструктор </summary>
		/// <param name = "hDC">  </param>
		/// <param name = "cx"> размер области для отрисовки целиком </param>
		/// <param name = "cy"> размер области для отрисовки целиком </param>
		DoubleBuffering(HWND hWindow, int cx, int cy);
		/// <summary> Добавить к отрисовке фон </summary>
		/// <param name = "hBmp" картинка фона </summary>
		void AddBackground(HBITMAP hBmp);
		/// <summary> Освободить контекст </summary>
		void Release();
		/// <summary> Обновить фон </summary>
		void BackgroundUpdate();
		/// <summary> Добавить объект поверх фона </summary>
		/// <param name = "xDest"> координата объекта на DC назначения </param>
		/// <param name = "yDest"> координата объекта на DC назначения </param>
		/// <param name = "widthDest"> ширина объекта на DC назначения </param>
		/// <param name = "heightDest"> высота объекта на DC назначения </param>
		/// <param name = "hSrcDC"> DC содержащий объект </param>
		/// <param name = "widthSrc"> ширина объекта на исходном DC </param>
		/// <param name = "heightSrc"> высота объекта на исходном DC </param>
		/// <param name = "crTransparent"> цвет, который будет считаться прозрачным </param>
		void AddObject(int xDest, int yDest, int widthDest, int heightDest,
			HDC hSrcDC, int widthSrc, int heightSrc, UINT crTransparent);
		/// <summary> Посылает сообщение WM_PAINT </summary>
		void Invalidate();
		void Paint();
		~DoubleBuffering();
	};
}


