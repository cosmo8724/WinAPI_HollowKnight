#include "stdafx.h"
#include "Dash_Effect.h"
#include "Player.h"

#include "Obj_Mgr.h"
#include "Bmp_Mgr.h"
#include "Scroll_Mgr.h"


CDash_Effect::CDash_Effect()
{
}


CDash_Effect::~CDash_Effect()
{
}

void CDash_Effect::Initialize(void)
{
	m_tInfo.fCX = 400.f;
	m_tInfo.fCY = 190.f;
	m_eLook = CObj_Mgr::Get_Instance()->Get_Player()->Get_Look();

	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Effect/Dash3.bmp", L"Dash_Effect");

	if (m_eLook == LOOK_LEFT)
		m_tFrame.iMotion = LOOK_LEFT;
	else
		m_tFrame.iMotion = LOOK_RIGHT;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 6;
	m_tFrame.dwFrameSpeed = 40;
	m_tFrame.dwFrameTime = GetTickCount();

	m_pFrameKey = L"Dash_Effect";
}

int CDash_Effect::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

	Move_Frame();

	if (m_eLook == LOOK_LEFT)
		m_tInfo.fX -= dynamic_cast<CPlayer*>(CObj_Mgr::Get_Instance()->Get_Player())->Get_DashSpeed() * 0.95f;
	else
		m_tInfo.fX += dynamic_cast<CPlayer*>(CObj_Mgr::Get_Instance()->Get_Player())->Get_DashSpeed() * 0.95f;

	Update_Rect();

	return OBJ_NOEVENT;
}

void CDash_Effect::Late_Update(void)
{
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		m_bDead = true;
}

void CDash_Effect::Render(HDC hDC)
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
		RGB(100, 100, 100));
}

void CDash_Effect::Release(void)
{
}
