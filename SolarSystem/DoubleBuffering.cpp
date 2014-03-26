#include "DoubleBuffering.h"


DB::DoubleBuffering::DoubleBuffering(HWND hWindow, int cx, int cy)
{
	ResX = cx;
	ResY = cy;
	//что перерисовать
	rcCurrent.left = 0;
	rcCurrent.top = 0;
	rcCurrent.right = ResX;
	rcCurrent.bottom = ResY;

	this->hWindow = hWindow;
	hDC = GetDC(hWindow);
	hCopyDC = CreateCompatibleDC(hDC);		
	hBackGrDC = CreateCompatibleDC(hDC);	

	hBmpCopyClient = CreateCompatibleBitmap(hDC, ResX, ResY);
}

void DB::DoubleBuffering::AddBackground(HBITMAP hBmp)
{
	hBmpBackGr = hBmp;
	
	hBmpTemp = (HBITMAP)SelectObject(hBackGrDC, hBmpBackGr);
	if (hBmpTemp) DeleteObject(hBmpTemp);
	hBmpTemp = (HBITMAP)SelectObject(hCopyDC, hBmpCopyClient);
	if (hBmpTemp) DeleteObject(hBmpTemp);
}

void DB::DoubleBuffering::Release()
{
	ReleaseDC(this->hWindow, this->hDC);
}

void DB::DoubleBuffering::BackgroundUpdate()
{
	StretchBlt(hCopyDC, 0, 0, ResX, ResY, hBackGrDC, 0, 0, ResX, ResY, SRCCOPY);
}

void DB::DoubleBuffering::AddObject(int xDest, int yDest, int widthDest, int heightDest,
	HDC hSrcDC, int xSrc, int ySrc, int widthSrc, int heightSrc, UINT crTransparent)
{
	TransparentBlt(hCopyDC, xDest, yDest, widthDest, heightDest, hSrcDC, xSrc, ySrc, widthSrc, heightSrc, crTransparent);
}

void DB::DoubleBuffering::AddObject(int xDest, int yDest, int widthDest, int heightDest,
	HDC hSrcDC, int widthSrc, int heightSrc, UINT crTransparent)
{
	DB::DoubleBuffering::AddObject(xDest, yDest, widthDest, heightDest, hSrcDC, 0, 0, widthSrc, heightSrc, crTransparent);
}

void DB::DoubleBuffering::Invalidate()
{
	InvalidateRect(hWindow, &rcCurrent, false);
}

void DB::DoubleBuffering::Paint()
{

	hDC = BeginPaint(hWindow, &PS);
	StretchBlt(hDC, PS.rcPaint.left, PS.rcPaint.top, PS.rcPaint.right, PS.rcPaint.bottom,
		hCopyDC, PS.rcPaint.left, PS.rcPaint.top, PS.rcPaint.right, PS.rcPaint.bottom,
		//SRCPAINT);
		SRCCOPY);
	EndPaint(hWindow, &PS);
}

DB::DoubleBuffering::~DoubleBuffering()
{
}
