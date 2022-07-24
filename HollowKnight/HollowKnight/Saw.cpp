#include "stdafx.h"
#include "Saw.h"

#include "Bmp_Mgr.h"
#include "Scroll_Mgr.h"

CSaw::CSaw()
{
}


CSaw::~CSaw()
{
}

void CSaw::Initialize(void)
{
	m_tInfo.fCX = 300.f;
	m_tInfo.fCY = 300.f;

	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Spike/Saw.bmp", L"Saw");
	m_pFrameKey = L"Saw";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 2;
	m_tFrame.iMotion = 0;
	m_tFrame.dwFrameSpeed = 0;
	m_tFrame.dwFrameTime = GetTickCount();
}

int CSaw::Update(void)
{
	Move_Frame();
	Update_Rect();
	Update_HitBox();

	return OBJ_NOEVENT;
}

void CSaw::Late_Update(void)
{
}

void CSaw::Render(HDC hDC)
{
	HDC hMemDC = CBmp_Mgr::Get_Instance()->Find_Img(m_pFrameKey);

	int iScrollX = (int)CScroll_Mgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScroll_Mgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		(int)m_tInfo.fCX * m_tFrame.iFrameStart,
		(int)m_tInfo.fCY * m_tFrame.iMotion,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(255, 255, 255));
}

void CSaw::Release(void)
{
}

void CSaw::Update_HitBox()
{
}
