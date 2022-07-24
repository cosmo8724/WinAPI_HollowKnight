#include "stdafx.h"
#include "Back_Effect.h"

#include "Bmp_Mgr.h"
#include "Obj_Mgr.h"
#include "Scroll_Mgr.h"


CBack_Effect::CBack_Effect()
{
}


CBack_Effect::~CBack_Effect()
{
}

void CBack_Effect::Initialize(void)
{
	m_tInfo.fCX = 832.f;
	m_tInfo.fCY = 832.f;

	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Effect/Back_Effect1.bmp", L"Back_Effect");
	m_pFrameKey = L"Back_Effect";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 8;
	m_tFrame.iMotion = 0;
	m_tFrame.dwFrameSpeed = 80;
	m_tFrame.dwFrameTime = GetTickCount();
}

int CBack_Effect::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

	float fThe_Radiance_fX;
	float fThe_Radiance_fY;

	fThe_Radiance_fX = CObj_Mgr::Get_Instance()->Get_ObjList(OBJ_MONSTER)->back()->Get_Info().fX;
	fThe_Radiance_fY = CObj_Mgr::Get_Instance()->Get_ObjList(OBJ_MONSTER)->back()->Get_Info().fY - 100;

	Move_Frame();

	Update_Info(fThe_Radiance_fX, fThe_Radiance_fY);
	Update_Rect();

	return OBJ_NOEVENT;
}

void CBack_Effect::Late_Update(void)
{
	if (CObj_Mgr::Get_Instance()->Get_ObjList(OBJ_MONSTER)->empty())
		m_bDead = true;
}

void CBack_Effect::Render(HDC hDC)
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
		RGB(255, 207, 88));
}

void CBack_Effect::Release(void)
{
}
