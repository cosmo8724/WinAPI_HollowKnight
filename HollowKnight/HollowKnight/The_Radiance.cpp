#include "stdafx.h"
#include "The_Radiance.h"

#include "Horizon_Spear.h"
#include "Vertical_Spear.h"
#include "Back_Effect.h"

#include "Abstract_Factory.h"
#include "Obj_Mgr.h"
#include "Bmp_Mgr.h"
#include "Scroll_Mgr.h"
#include "SoundMgr.h"

CThe_Radiance::CThe_Radiance()
{
}


CThe_Radiance::~CThe_Radiance()
{
}

void CThe_Radiance::Initialize(void)
{
	m_tInfo = { 1200.f, 4200.f, 935.f, 935.f };

	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Boss/The Radiance/Pre_Idle.bmp", L"Pre_Idle");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Boss/The Radiance/Idle.bmp", L"The_Radiance_Idle");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Boss/The Radiance/Turn.bmp", L"The_Radiance_Turn");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Boss/The Radiance/Wing1.bmp", L"The_Radiance_Wing1");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Boss/The Radiance/Wing2_wwwww.bmp", L"The_Radiance_Wing2");

	m_pFrameKey = L"Pre_Idle";
	m_eCurState = PRE_IDLE;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 30;
	m_tFrame.iMotion = LOOK_LEFT;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();
	m_iLoopFrameStart = 25;

	m_iStartTime = GetTickCount();
	m_AttackTimer.InitLoop(4.f);

	CSoundMgr::Get_Instance()->PlaySound(L"The_Radiance_Intro.wav", SOUND_BGM, 0.4f);
}

int CThe_Radiance::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (!m_bStart && m_iStartTime + 23000 < GetTickCount())
	{
		m_bStart = true;
		m_iStartTime = GetTickCount();
	}
	else if (!m_bStart && m_iStartTime + 18000 < GetTickCount())
	{
		m_pFrameKey = L"The_Radiance_Idle";
		m_eCurState = IDLE;

		if (!m_bCreateBackEffect)
		{
			CObj_Mgr::Get_Instance()->Add_Object(OBJ_MONEFFECT, CAbstract_Factory<CBack_Effect>::Create());
			m_bCreateBackEffect = true;
		}
		CSoundMgr::Get_Instance()->PlaySound(L"The_Radiance_Scream.wav", SOUND_EFFECT, 0.3f);
	}
	else if (m_bStart && !m_bAttackReady && !m_bAttack && m_AttackTimer.Check())
	{
		m_pFrameKey = L"The_Radiance_Turn";
		m_eCurState = DISAPPEAR_TURN;
		m_bAttackReady = true;
		CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
		CSoundMgr::Get_Instance()->PlaySound(L"The_Radiance_Teleport.wav", SOUND_EFFECT, 0.6f);
		CSoundMgr::Get_Instance()->PlaySound(L"The_Radiance_OST.wav", SOUND_BGM, 0.6f);
	}

	if (m_bAttackReady)
	{
		if (m_ePreState == DISAPPEAR_TURN && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{
			m_pFrameKey = L"The_Radiance_Turn";
			m_eCurState = APPEAR_TURN;
			m_bAttack = true;
		}
		if (m_ePreState == APPEAR_TURN && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{
			//if (GetTickCount() % 2 == 0)
			//{
			//	m_pFrameKey = L"The_Radiance_Wing1";
			//	m_eCurState = WING1;
			//}
			//else if (GetTickCount() % 2 == 1)
			//{
			//	m_pFrameKey = L"The_Radiance_Wing2";
			//	m_eCurState = WING2;
			//}

			if (true)
			{
				m_pFrameKey = L"The_Radiance_Wing2";
				m_eCurState = WING2;
			}
		}
	}

	if (m_bAttack)
	{
		Update_State();
	}

	Motion_Change();
	Move_Frame(m_iLoopFrameStart);
	Update_Position();
	Update_Untuchable();
	Update_Rect();
	Update_HitBox();

	return OBJ_NOEVENT;
}

void CThe_Radiance::Late_Update(void)
{
}

void CThe_Radiance::Render(HDC hDC)
{
	if (m_eCurState == WING2)
	{
		int a = 3;
	}

	HDC hMemDC = CBmp_Mgr::Get_Instance()->Find_Img(m_pFrameKey);

	int iScrollX = (int)CScroll_Mgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScroll_Mgr::Get_Instance()->Get_ScrollY();

	if (m_eCurState == PRE_IDLE)
	{
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
			RGB(255, 255, 230));
	}
	else 
	{
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

	if (m_bVisibleHitBox)
	{
		HPEN myPen = (HPEN)CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
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

void CThe_Radiance::Release(void)
{
}

void CThe_Radiance::Update_HitBox()
{
	m_tHitBox.left = m_tRect.left + 363;
	m_tHitBox.top = m_tRect.top + 293;
	m_tHitBox.right = m_tRect.left + 647;
	m_tHitBox.bottom = m_tRect.top + 560;
}

void CThe_Radiance::Collision_Event(CObj * _OtherObj, float _fColX, float _fColY)
{
}

void CThe_Radiance::Motion_Change(void)
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tInfo.fCX = 1000.f;
			m_tInfo.fCY = 1000.f;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = LOOK_LEFT;
			m_tFrame.dwFrameSpeed = 90;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 0;
			break;

		case APPEAR_TURN:
			m_tInfo.fCX = 1000.f;
			m_tInfo.fCY = 1000.f;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iMotion = LOOK_LEFT;
			m_tFrame.dwFrameSpeed = 70;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 0;
			break;

		case DISAPPEAR_TURN:
			m_tInfo.fCX = 1000.f;
			m_tInfo.fCY = 1000.f;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 70;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 0;
			break;

		case WING1:
			m_tInfo.fCX = 1000.f;
			m_tInfo.fCY = 1000.f;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iMotion = LOOK_LEFT;
			m_tFrame.dwFrameSpeed = 70;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 0;
			break;

		case WING2:
			m_tInfo.fCX = 1000.f;
			m_tInfo.fCY = 1000.f;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iMotion = LOOK_LEFT;
			m_tFrame.dwFrameSpeed = 70;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 0;
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CThe_Radiance::Update_State()
{
	if (m_bVertSpearReady)
		VertSpear();
	else if (m_bHorizSpearReady)
		HorizSpear();
	else if (m_bRotSpearReady)
		VertSpear(); //RotSpear();
	else if (m_bVertLazerReady)
		HorizSpear(); //VertLazer();
	else if (m_bRotLazerReady)
		VertSpear(); //RotLazer();
	else if (m_bTTBReady)
		HorizSpear(); //TangTangBall();
	else if (!m_bVertSpearReady && !m_bHorizSpearReady && !m_bRotSpearReady && !m_bVertLazerReady && !m_bRotLazerReady && !m_bTTBReady)
	{
		if (GetTickCount() % 6 == 0)
			m_bVertSpearReady = true;
		else if (GetTickCount() % 6 == 1)
			m_bHorizSpearReady = true;
		else if (GetTickCount() % 6 == 2)
			m_bRotSpearReady = true;
		else if (GetTickCount() % 6 == 3)
			m_bVertLazerReady = true;
		else if (GetTickCount() % 6 == 4)
			m_bRotLazerReady = true;
		else if (GetTickCount() % 6 == 5)
			m_bTTBReady = true;
	}
}

void CThe_Radiance::Update_Position()
{
	if (m_ePreState == DISAPPEAR_TURN && m_tFrame.iMotion == LOOK_RIGHT && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	{
		m_tInfo.fX = float(GetTickCount() % 1270 + 910);
		m_tInfo.fY = float(GetTickCount() % 430 + 4130);
	}
}

void CThe_Radiance::Update_Untuchable()
{
	switch (m_eCurState)
	{
	case PRE_IDLE:
		m_eUntouchable = ON;
		break;

	case IDLE:
		m_eUntouchable = OFF;
		break;

	case APPEAR_TURN:
		m_eUntouchable = ON;
		break;

	case DISAPPEAR_TURN:
		m_eUntouchable = ON;
		break;

	case WING1:
		m_eUntouchable = OFF;
		break;

	case WING2:
		m_eUntouchable = OFF;
		break;
	}
}

void CThe_Radiance::VertSpear()
{
	if (m_eCurState == WING1 || m_eCurState == WING2)
	{
		int RandArray[3];

		for (int i = 0; i < 3; ++i)
		{
			RandArray[i] = GetTickCount() % 6 + 1;

			if (i > 0 && RandArray[i] == RandArray[i - 1])
			{
				RandArray[i] = GetTickCount() % 6 + 1;
				i--;
			}
		}

		if (GetTickCount() % 2 == 0)
		{
			for (int i = 0; i < 9; ++i)
			{
				if (i == RandArray[0] || i == RandArray[1] || i == RandArray[2])
					continue;

				CObj_Mgr::Get_Instance()->Add_Object(OBJ_SWORD, CAbstract_Factory<CVertical_Spear>::Create(700.f, 4000.f + (i * 110)));
			}
		}

		else if (GetTickCount() % 2 == 1)
		{
			for (int i = 0; i < 9; ++i)
			{
				if (i == RandArray[0] || i == RandArray[1] || i == RandArray[2])
					continue;

				CObj_Mgr::Get_Instance()->Add_Object(OBJ_SWORD, CAbstract_Factory<CVertical_Spear>::Create(2300.f, 4000.f + (i * 110)));
			}
		}

		m_bVertLazerReady = false;
		m_bHorizSpearReady = false;
		m_bRotSpearReady = false;
		m_bVertLazerReady = false;
		m_bRotLazerReady = false;
		m_bTTBReady = false;
		m_bAttack = false;
		m_bAttackReady = false;
	}
}

void CThe_Radiance::HorizSpear()
{
	if (m_eCurState == WING1 || m_eCurState == WING2)
	{
		int RandArray[4];

		for (int i = 0; i < 4; ++i)
		{
			RandArray[i] = GetTickCount() % 11 + 2;
			
			if (i > 0 && RandArray[i] == RandArray[i - 1])
			{
				RandArray[i] = GetTickCount() % 11 + 2;
				i--;
			}
		}

		for (int i = 0; i < 15; ++i)
		{
			if (i == RandArray[0] || i == RandArray[1] || i == RandArray[2] || i == RandArray[3])
				continue;
			CObj_Mgr::Get_Instance()->Add_Object(OBJ_SWORD, CAbstract_Factory<CHorizon_Spear>::Create(820.f + i * 100, 3900.f));
		}

		m_bVertLazerReady = false;
		m_bHorizSpearReady = false;
		m_bRotSpearReady = false;
		m_bVertLazerReady = false;
		m_bRotLazerReady = false;
		m_bTTBReady = false;
		m_bAttack = false;
		m_bAttackReady = false;
	}
}

void CThe_Radiance::RotSpear()			// Idle Motion during Spawn Spear
{

}

void CThe_Radiance::VertLazer()
{

}

void CThe_Radiance::RotLazer()			// Idle Motion
{

}

void CThe_Radiance::TangTangBall()		// Idle Motion
{

}
