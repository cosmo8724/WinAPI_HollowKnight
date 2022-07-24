#include "stdafx.h"
#include "Damaged_Effect.h"

#include "Obj_Mgr.h"
#include "Bmp_Mgr.h"
#include "Scroll_Mgr.h"

CDamaged_Effect::CDamaged_Effect()
{
}


CDamaged_Effect::~CDamaged_Effect()
{
}

void CDamaged_Effect::Initialize(void)
{
	m_tInfo.fCX = 1000.f;
	m_tInfo.fCY = 500.f;

	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Effect/Damaged.bmp", L"Damaged_Effect");

	m_eLook = CObj_Mgr::Get_Instance()->Get_Player()->Get_Look();

	if (m_eLook == LOOK_LEFT)
		m_tFrame.iMotion = LOOK_RIGHT;
	else
		m_tFrame.iMotion = LOOK_LEFT;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();
}

int CDamaged_Effect::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

	Move_Frame();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CDamaged_Effect::Late_Update(void)
{
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		m_bDead = true;
}

void CDamaged_Effect::Render(HDC hDC)
{
	HDC hMemDC = CBmp_Mgr::Get_Instance()->Find_Img(L"Damaged_Effect");

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

void CDamaged_Effect::Release(void)
{
}

void CDamaged_Effect::Update_HitBox()
{
	m_tHitBox = m_tRect;
}
