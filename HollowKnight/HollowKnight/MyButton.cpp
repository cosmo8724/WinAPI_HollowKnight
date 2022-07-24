#include "stdafx.h"
#include "MyButton.h"

#include "Bmp_Mgr.h"


CMyButton::CMyButton()
{
}


CMyButton::~CMyButton()
{
}

void CMyButton::Initialize(void)
{
	m_tInfo.fCX = 165.f;
	m_tInfo.fCY = 40.f;
}

int CMyButton::Update(void)
{
	Update_Rect();

	return OBJ_NOEVENT;
}

void CMyButton::Late_Update(void)
{
}

void CMyButton::Render(HDC hDC)
{
	HDC	hMemDC = CBmp_Mgr::Get_Instance()->Find_Img(m_pFrameKey);

	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		0,
		0,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(11, 11, 11));
}

void CMyButton::Release(void)
{
}
