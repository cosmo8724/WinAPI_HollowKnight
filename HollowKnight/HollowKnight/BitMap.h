#pragma once
#include "Include.h"

class CBitMap
{
private:
	HDC			m_hMemDC;
	HBITMAP		m_hBitmap;
	HBITMAP		m_hOldBmp;

public:
	CBitMap();
	~CBitMap();

public:
	HDC		Get_MemDC() { return m_hMemDC; }
	void	Load_Bmp(const TCHAR* pFilePath);
	void	Release();
};

