#include "stdafx.h"
#include "BitMap.h"


CBitMap::CBitMap()
{
}


CBitMap::~CBitMap()
{
	Release();
}

void CBitMap::Load_Bmp(const TCHAR * pFilePath)
{
	HDC		hDC = GetDC(g_hWnd);

	m_hMemDC = CreateCompatibleDC(hDC);

	ReleaseDC(g_hWnd, hDC);

	m_hBitmap = (HBITMAP)LoadImage(nullptr,
									pFilePath,
									IMAGE_BITMAP,
									0,
									0,
									LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
}

void CBitMap::Release()
{
	SelectObject(m_hMemDC, m_hOldBmp);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}
