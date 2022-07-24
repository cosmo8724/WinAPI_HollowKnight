#include "stdafx.h"
#include "Horizon_Spear.h"

#include "Player.h"

#include "Bmp_Mgr.h"
#include "Scroll_Mgr.h"
#include "SoundMgr.h"

CHorizon_Spear::CHorizon_Spear()
{
}


CHorizon_Spear::~CHorizon_Spear()
{
}

void CHorizon_Spear::Initialize(void)
{
	m_tInfo.fCX = 200.f;
	m_tInfo.fCY = 500.f;
	m_fSpeed = 20.f;

	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Boss/The Radiance/Horizon_Spear.bmp", L"Horizon_Spear");

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.iMotion = 0;
	m_tFrame.dwFrameSpeed = 80;
	m_tFrame.dwFrameTime = GetTickCount();

	CSoundMgr::Get_Instance()->PlaySound(L"The_Radiance_Sword_Create.wav", SOUND_EFFECT, 0.3f);
}

int CHorizon_Spear::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	{
		m_tInfo.fY += m_fSpeed;
		
		if (!m_bSoundChange)
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"The_Radiance_Sword_Shoot.wav", SOUND_EFFECT, 0.3f);
			m_bSoundChange = true;
		}
	}

	Move_Frame(m_iLoopFrameStart);
	Update_Rect();
	Update_HitBox();

	return OBJ_NOEVENT;
}

void CHorizon_Spear::Late_Update(void)
{
	if (m_tRect.top > 5000)
		m_bDead = true;
}

void CHorizon_Spear::Render(HDC hDC)
{
	HDC hMemDC = CBmp_Mgr::Get_Instance()->Find_Img(L"Horizon_Spear");

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
		RGB(255, 255, 250));

	if (m_bVisibleHitBox)
	{
		HPEN myPen = (HPEN)CreatePen(PS_SOLID, 2, RGB(255, 0, 255));
		HPEN oldPen = (HPEN)::SelectObject(hDC, (HGDIOBJ)myPen);
		HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, myBrush);

		Rectangle(hDC, m_tHitBox.left + iScrollX, m_tHitBox.top + iScrollY, m_tHitBox.right + iScrollX, m_tHitBox.bottom + iScrollY);

		SelectObject(hDC, oldBrush);
		DeleteObject(myBrush);
		SelectObject(hDC, oldPen);
		DeleteObject(myPen);
	}
}

void CHorizon_Spear::Release(void)
{
}

void CHorizon_Spear::Update_HitBox()
{
	m_tHitBox.left = m_tRect.left + 94;
	m_tHitBox.top = m_tRect.top + 99;
	m_tHitBox.right = m_tRect.right - 94;
	m_tHitBox.bottom = m_tRect.bottom - 35;
}

void CHorizon_Spear::Collision_Event(CObj * _OtherObj, float _fColX, float _fColY)
{
}
