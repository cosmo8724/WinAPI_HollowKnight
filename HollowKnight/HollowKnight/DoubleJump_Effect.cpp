#include "stdafx.h"
#include "DoubleJump_Effect.h"

#include "Obj_Mgr.h"
#include "Bmp_Mgr.h"
#include "Scroll_Mgr.h"

CDoubleJump_Effect::CDoubleJump_Effect()
{
}


CDoubleJump_Effect::~CDoubleJump_Effect()
{
}

void CDoubleJump_Effect::Initialize(void)
{
	m_tInfo.fCX = 200.f;
	m_tInfo.fCY = 200.f;
	m_eLook = CObj_Mgr::Get_Instance()->Get_Player()->Get_Look();

	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Effect/DoubleJump.bmp", L"DoubleJump_Effect");

	if (m_eLook == LOOK_LEFT)
		m_tFrame.iMotion = LOOK_LEFT;
	else
		m_tFrame.iMotion = LOOK_RIGHT;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.dwFrameSpeed = 90;
	m_tFrame.dwFrameTime = GetTickCount();

	m_pFrameKey = L"DoubleJump_Effect";
}

int CDoubleJump_Effect::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

	float fPlayerX = CObj_Mgr::Get_Instance()->Get_Player()->Get_Info().fX;
	float fPlayerY = CObj_Mgr::Get_Instance()->Get_Player()->Get_Info().fY + 20.f;

	Move_Frame();
	Update_Info(fPlayerX, fPlayerY);
	Update_Rect();

	return OBJ_NOEVENT;
}

void CDoubleJump_Effect::Late_Update(void)
{
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		m_bDead = true;
}

void CDoubleJump_Effect::Render(HDC hDC)
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
		RGB(11, 11, 11));
}

void CDoubleJump_Effect::Release(void)
{
}
