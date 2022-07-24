#include "stdafx.h"
#include "FireBall_Hit_Wall.h"

#include "Obj_Mgr.h"
#include "Bmp_Mgr.h"
#include "Scroll_Mgr.h"
#include "SoundMgr.h"

CFireBall_Hit_Wall::CFireBall_Hit_Wall()
{
}


CFireBall_Hit_Wall::~CFireBall_Hit_Wall()
{
}

void CFireBall_Hit_Wall::Initialize(void)
{
	m_tInfo.fCX = 150.f;
	m_tInfo.fCY = 200.f;
	m_eLook = CObj_Mgr::Get_Instance()->Get_Player()->Get_Look();

	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Effect/FireBall_Hit_Wall.bmp", L"FireBall_Hit_Wall");

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.dwFrameSpeed = 40;
	m_tFrame.dwFrameTime = GetTickCount();

	m_pFrameKey = L"FireBall_Hit_Wall";

	CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
	CSoundMgr::Get_Instance()->PlaySound(L"FireBall_Hit.wav", SOUND_PLAYER, 0.4f);
}

int CFireBall_Hit_Wall::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

	Move_Frame();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CFireBall_Hit_Wall::Late_Update(void)
{
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		m_bDead = true;
}

void CFireBall_Hit_Wall::Render(HDC hDC)
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

void CFireBall_Hit_Wall::Release(void)
{
}
