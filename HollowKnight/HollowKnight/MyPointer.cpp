#include "stdafx.h"
#include "MyPointer.h"

#include "Bmp_Mgr.h"
#include "Key_Mgr.h"

CMyPointer::CMyPointer()
{
}


CMyPointer::~CMyPointer()
{
}

void CMyPointer::Initialize(void)
{
	m_tInfo.fCX = 53.363636f;
	m_tInfo.fCY = 40.f;

	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Menu/Pointer.bmp", L"Pointer");

	m_pFrameKey = L"Pointer";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 10;
	m_tFrame.dwFrameSpeed = 40;
	m_tFrame.dwFrameTime = GetTickCount();
}

int CMyPointer::Update(void)
{
	Move_Frame(m_tFrame.iFrameEnd);
	Update_Rect();

	return OBJ_NOEVENT;
}

void CMyPointer::Late_Update(void)
{
}

void CMyPointer::Render(HDC hDC)
{
	HDC	hMemDC = CBmp_Mgr::Get_Instance()->Find_Img(m_pFrameKey);

	GdiTransparentBlt(hDC,
		m_tRect.left - 130,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		(int)m_tInfo.fCX * m_tFrame.iFrameStart,
		(int)m_tInfo.fCY * LOOK_RIGHT,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(11, 11, 11));

	GdiTransparentBlt(hDC,
		m_tRect.left + 120,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		(int)m_tInfo.fCX * m_tFrame.iFrameStart,
		(int)m_tInfo.fCY * LOOK_LEFT,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(11, 11, 11));
}

void CMyPointer::Release(void)
{
}