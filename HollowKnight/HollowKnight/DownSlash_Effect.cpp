#include "stdafx.h"
#include "DownSlash_Effect.h"

#include "Player.h"
#include "Mantis_Lord.h"
#include "Mantis_Spike.h"
#include "The_Radiance.h"

#include "Down_Hit_Effect.h"

#include "Abstract_Factory.h"
#include "Obj_Mgr.h"
#include "Bmp_Mgr.h"
#include "Scroll_Mgr.h"
#include "SoundMgr.h"

CDownSlash_Effect::CDownSlash_Effect()
{
}


CDownSlash_Effect::~CDownSlash_Effect()
{
}

void CDownSlash_Effect::Initialize(void)
{
	m_tInfo.fCX = 180.f;
	m_tInfo.fCY = 210.f;
	m_eLook = CObj_Mgr::Get_Instance()->Get_Player()->Get_Look();

	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Effect/DownSlash.bmp", L"DownSlash_Effect");

	if (m_eLook == LOOK_LEFT)
		m_tFrame.iMotion = LOOK_LEFT;
	else
		m_tFrame.iMotion = LOOK_RIGHT;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();

	m_pFrameKey = L"DownSlash_Effect";
}

int CDownSlash_Effect::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

	float fPlayerX;
	float fPlayerY;

	fPlayerX = CObj_Mgr::Get_Instance()->Get_Player()->Get_Info().fX;
	fPlayerY = CObj_Mgr::Get_Instance()->Get_Player()->Get_Info().fY + 100.f;

	Move_Frame();

	Update_Info(fPlayerX, fPlayerY);
	Update_Rect();
	Update_HitBox();

	return OBJ_NOEVENT;
}

void CDownSlash_Effect::Late_Update(void)
{
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		m_bDead = true;
}

void CDownSlash_Effect::Render(HDC hDC)
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
		RGB(101, 101, 101));

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

void CDownSlash_Effect::Release(void)
{
}

void CDownSlash_Effect::Update_HitBox()
{
	m_tHitBox = m_tRect;
}

void CDownSlash_Effect::Collision_Event(CObj * _OtherObj, float _fColX, float _fColY)
{
	CMantis_Lord*	pMantisLord = dynamic_cast<CMantis_Lord*>(_OtherObj);
	if (pMantisLord)
	{
		if (!m_bHit)
		{
			if (CObj_Mgr::Get_Instance()->Get_Player()->Get_Stat().iSoul <= 90)
				dynamic_cast<CPlayer*>(CObj_Mgr::Get_Instance()->Get_Player())->Set_IncreaseCurSoul();

			dynamic_cast<CPlayer*>(CObj_Mgr::Get_Instance()->Get_Player())->HitKnockBack();
			CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CDown_Hit_Effect>::Create(CObj_Mgr::Get_Instance()->Get_Player()->Get_Info().fX, (pMantisLord->Get_HitBox().bottom + pMantisLord->Get_HitBox().top) * 0.5f));
			CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Enemy_Damage.wav", SOUND_EFFECT, 0.4f);
			m_bHit = true;
		}
	}
	CMantis_Spike*	pMantis_Spike = dynamic_cast<CMantis_Spike*>(_OtherObj);
	if (pMantis_Spike)
	{
		if (!m_bHit)
		{
			dynamic_cast<CPlayer*>(CObj_Mgr::Get_Instance()->Get_Player())->HitKnockBack();
			dynamic_cast<CPlayer*>(CObj_Mgr::Get_Instance()->Get_Player())->Set_DoubleJumpCnt(1);
			dynamic_cast<CPlayer*>(CObj_Mgr::Get_Instance()->Get_Player())->Set_IsDoubleJump(false);
			CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CDown_Hit_Effect>::Create(CObj_Mgr::Get_Instance()->Get_Player()->Get_Info().fX, (pMantis_Spike->Get_HitBox().bottom + pMantis_Spike->Get_HitBox().top) * 0.5f));
			CSoundMgr::Get_Instance()->PlaySound(L"Parry.wav", SOUND_EFFECT, 1.f);
			m_bHit = true;
		}
	}

	CThe_Radiance*	pTheRadiance = dynamic_cast<CThe_Radiance*>(_OtherObj);
	if (pTheRadiance)
	{
		UNTOUCHABLE	eUntouchable = pTheRadiance->Get_Untouchable();

		if (!eUntouchable && !m_bHit)
		{
			if (CObj_Mgr::Get_Instance()->Get_Player()->Get_Stat().iSoul <= 90)
				dynamic_cast<CPlayer*>(CObj_Mgr::Get_Instance()->Get_Player())->Set_IncreaseCurSoul();

			dynamic_cast<CPlayer*>(CObj_Mgr::Get_Instance()->Get_Player())->HitKnockBack();
			CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CDown_Hit_Effect>::Create(CObj_Mgr::Get_Instance()->Get_Player()->Get_Info().fX, (pTheRadiance->Get_HitBox().bottom + pTheRadiance->Get_HitBox().top) * 0.5f));
			CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Enemy_Damage.wav", SOUND_EFFECT, 0.4f);
			m_bHit = true;
		}
	}
}
