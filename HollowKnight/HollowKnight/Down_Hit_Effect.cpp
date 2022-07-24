#include "stdafx.h"
#include "Down_Hit_Effect.h"

#include "Obj_Mgr.h"
#include "Bmp_Mgr.h"
#include "Scroll_Mgr.h"

CDown_Hit_Effect::CDown_Hit_Effect()
{
}


CDown_Hit_Effect::~CDown_Hit_Effect()
{
}

void CDown_Hit_Effect::Initialize(void)
{
	m_tInfo.fCX = 500.f;
	m_tInfo.fCY = 800.f;

	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Effect/Down_Hit.bmp", L"Down_Hit_Effect");

	m_eLook = CObj_Mgr::Get_Instance()->Get_Player()->Get_Look();

	if (m_eLook == LOOK_LEFT)
		m_tFrame.iMotion = LOOK_LEFT;
	else
		m_tFrame.iMotion = LOOK_RIGHT;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.dwFrameSpeed = 40;
	m_tFrame.dwFrameTime = GetTickCount();
}

int CDown_Hit_Effect::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

	Move_Frame();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CDown_Hit_Effect::Late_Update(void)
{
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		m_bDead = true;
}

void CDown_Hit_Effect::Render(HDC hDC)
{
	HDC hMemDC = CBmp_Mgr::Get_Instance()->Find_Img(L"Down_Hit_Effect");

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
		RGB(11, 11, 11));
}

void CDown_Hit_Effect::Release(void)
{
}

void CDown_Hit_Effect::Update_HitBox()
{
	m_tHitBox = m_tRect;
}
