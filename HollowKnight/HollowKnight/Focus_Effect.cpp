#include "stdafx.h"
#include "Focus_Effect.h"

#include "Player.h"

#include "Obj_Mgr.h"
#include "Bmp_Mgr.h"
#include "Scroll_Mgr.h"

CFocus_Effect::CFocus_Effect()
{
}


CFocus_Effect::~CFocus_Effect()
{
}

void CFocus_Effect::Initialize(void)
{
	m_tInfo.fCX = 150.f;
	m_tInfo.fCY = 300.f;
	m_eLook = CObj_Mgr::Get_Instance()->Get_Player()->Get_Look();

	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Effect/Focus.bmp", L"Focus_Effect");

	if (m_eLook == LOOK_LEFT)
		m_tFrame.iMotion = LOOK_LEFT;
	else
		m_tFrame.iMotion = LOOK_RIGHT;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 12;
	m_tFrame.dwFrameSpeed = 40;
	m_tFrame.dwFrameTime = GetTickCount();
	m_iLoopFrameStart = 6;

	m_pFrameKey = L"Focus_Effect";
}

int CFocus_Effect::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

	Move_Frame(m_iLoopFrameStart);
	Update_Rect();

	return OBJ_NOEVENT;
}

void CFocus_Effect::Late_Update(void)
{
	if (!(dynamic_cast<CPlayer*>(CObj_Mgr::Get_Instance()->Get_Player())->Get_IsCharge()))
		m_bDead = true;
}

void CFocus_Effect::Render(HDC hDC)
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

void CFocus_Effect::Release(void)
{
}
