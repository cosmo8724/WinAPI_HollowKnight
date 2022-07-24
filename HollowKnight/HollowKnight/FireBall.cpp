#include "stdafx.h"
#include "FireBall.h"

#include "Mantis_Lord.h"
#include "The_Radiance.h"
#include "Rectangle.h"
#include "FireBall_Hit_Wall.h"

#include "Abstract_Factory.h"
#include "Obj_Mgr.h"
#include "Bmp_Mgr.h"
#include "Scroll_Mgr.h"
#include "SoundMgr.h"

CFireBall::CFireBall()
{
}


CFireBall::~CFireBall()
{
}

void CFireBall::Initialize(void)
{
	m_tStat = { 0, 0, 0, 0, 10 };
	m_tInfo.fCX = 270.f;
	m_tInfo.fCY = 120.f;
	m_fSpeed = 20.f;
	m_eLook = CObj_Mgr::Get_Instance()->Get_Player()->Get_Look();

	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Player/Knight/FireBall.bmp", L"Fire_Ball");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Player/Knight/FireBall_End.bmp", L"Fire_Ball_End");

	if (m_eLook == LOOK_LEFT)
		m_tFrame.iMotion = LOOK_LEFT;
	else
		m_tFrame.iMotion = LOOK_RIGHT;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.dwFrameSpeed = 40;
	m_tFrame.dwFrameTime = GetTickCount();
	m_iLoopFrameStart = 2;

	m_pFrameKey = L"Fire_Ball";
	m_eCurState = IDLE;
}

int CFireBall::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_tFrame.iMotion == LOOK_LEFT)
		m_tInfo.fX -= m_fSpeed;
	else
		m_tInfo.fX += m_fSpeed;

	Motion_Change();
	Move_Frame(m_iLoopFrameStart);
	Update_Rect();
	Update_HitBox();

	return OBJ_NOEVENT;
}

void CFireBall::Late_Update(void)
{
}

void CFireBall::Render(HDC hDC)
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

void CFireBall::Release(void)
{
}

void CFireBall::Update_HitBox()
{
	m_tHitBox.left = m_tRect.left + 13;
	m_tHitBox.top = m_tRect.top + 18;
	m_tHitBox.right = m_tRect.left + 260;
	m_tHitBox.bottom = m_tRect.top + 103;
}

void CFireBall::Collision_Event(CObj * _OtherObj, float _fColX, float _fColY)
{
	CRectangle*	pRectangle = dynamic_cast<CRectangle*>(_OtherObj);
	if (pRectangle)
	{
		if (m_eLook == LOOK_LEFT)
			CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CFireBall_Hit_Wall>::Create(m_tInfo.fX - 60.f, m_tInfo.fY));
		else
			CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CFireBall_Hit_Wall>::Create(m_tInfo.fX + 60.f, m_tInfo.fY));

		CObj_Mgr::Get_Instance()->Get_ObjList(OBJ_EFFECT)->back()->Set_FrameMotion(m_eLook);
		m_bDead = true;
	}

	CMantis_Lord*	pMantisLord = dynamic_cast<CMantis_Lord*>(_OtherObj);
	if (pMantisLord)
	{
		UNTOUCHABLE	eUntouchable = pMantisLord->Get_Untouchable();

		if (!eUntouchable)
		{
			if (m_eLook == LOOK_LEFT)
				CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CFireBall_Hit_Wall>::Create(m_tInfo.fX - 60.f, m_tInfo.fY));
			else
				CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CFireBall_Hit_Wall>::Create(m_tInfo.fX + 60.f, m_tInfo.fY));

			CObj_Mgr::Get_Instance()->Get_ObjList(OBJ_EFFECT)->back()->Set_FrameMotion(m_eLook);
			m_bDead = true;
		}
	}

	CThe_Radiance*	pRadiance = dynamic_cast<CThe_Radiance*>(_OtherObj);
	if (pRadiance)
	{
		UNTOUCHABLE	eUntouchable = pRadiance->Get_Untouchable();

		if (!eUntouchable)
		{
			if (m_eLook == LOOK_LEFT)
				CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CFireBall_Hit_Wall>::Create(m_tInfo.fX - 60.f, m_tInfo.fY));
			else
				CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CFireBall_Hit_Wall>::Create(m_tInfo.fX + 60.f, m_tInfo.fY));

			CObj_Mgr::Get_Instance()->Get_ObjList(OBJ_EFFECT)->back()->Set_FrameMotion(m_eLook);
			m_bDead = true;
		}
	}
}

void CFireBall::Motion_Change(void)
{
}
