#include "stdafx.h"
#include "Mantis_Lord.h"

#include "Slash_Effect.h"
#include "UpSlash_Effect.h"
#include "DownSlash_Effect.h"

#include "Obj_Mgr.h"
#include "Bmp_Mgr.h"
#include "Scroll_Mgr.h"
#include "Line_Mgr.h"
#include "Collision_Mgr.h"
#include "SoundMgr.h"

CMantis_Lord::CMantis_Lord()
{
}

CMantis_Lord::~CMantis_Lord()
{
}

void CMantis_Lord::Initialize(void)
{
	m_tStat = { 100, 100, 0, 0, 10 };
	m_tInfo = { 500.f, 0.f, 700.f, 750.f };
	m_fSpeed = 10.f;

	m_dwDamagedTime = GetTickCount();
	m_eLook = LOOK_RIGHT;

	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Boss/Mantis Lord/Idle.bmp", L"Mantis_Lord_Idle");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Boss/Mantis Lord/Dash_Ready.bmp", L"Mantis_Lord_Dash_Ready");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Boss/Mantis Lord/Dash.bmp", L"Mantis_Lord_Dash");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Boss/Mantis Lord/Dash_To_Idle.bmp", L"Mantis_Lord_Dash_To_Idle");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Boss/Mantis Lord/Slash_Ready.bmp", L"Mantis_Lord_Slash_Ready");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Boss/Mantis Lord/Slash.bmp", L"Mantis_Lord_Slash");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Boss/Mantis Lord/Slash_To_Idle.bmp", L"Mantis_Lord_Slash_To_Idle");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Boss/Mantis Lord/Fall_Ready.bmp", L"Mantis_Lord_Fall_Ready");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Boss/Mantis Lord/Fall.bmp", L"Mantis_Lord_Fall");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Boss/Mantis Lord/Fall_To_Idle.bmp", L"Mantis_Lord_Fall_To_Idle");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Boss/Mantis Lord/Death.bmp", L"Mantis_Lord_Death");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Boss/Mantis Lord/Sit.bmp", L"Mantis_Lord_Sit");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Boss/Mantis Lord/Sit_To_Stand.bmp", L"Mantis_Lord_Sit_To_Stand");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Boss/Mantis Lord/Stand_To_Idle.bmp", L"Mantis_Lord_Stand_To_Idle");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Boss/Mantis Lord/Idle_To_Sit.bmp", L"Mantis_Lord_Idle_To_Sit");

	m_pFrameKey = L"Mantis_Lord_Sit";
	m_eCurState = SIT;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iMotion = LOOK_RIGHT;
	m_tFrame.dwFrameSpeed = 70;
	m_tFrame.dwFrameTime = GetTickCount();
	m_iLoopFrameStart = 0;

	m_dwStartTimer = GetTickCount();
	//m_AttackTimer.InitLoop(3.f);
	m_DashReadyTimer.InitTick(1.f, 0.6f);
	m_SlashReadyTimer.InitTick(1.f, 0.6f);
	m_FallReadyTimer.InitTick(1.f, 0.6f);
}

int  CMantis_Lord::Update(void)
{
	if (!m_bStart)
	{
		Update_Position();
		if (m_dwStartTimer + 5000 < GetTickCount())
		{
			m_pFrameKey = L"Mantis_Lord_Sit_To_Stand";
			m_eCurState = SIT_TO_STAND;
			m_dwStartTimer = GetTickCount() + 999999;
			m_dwStandToIdleTimer = GetTickCount();
		}
		if (m_dwStandToIdleTimer + 2000 < GetTickCount() && m_ePreState == SIT_TO_STAND && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{
			m_pFrameKey = L"Mantis_Lord_Stand_To_Idle";
			m_eCurState = STAND_TO_IDLE;
			m_dwStandToIdleTimer = GetTickCount() + 999999;
			CSoundMgr::Get_Instance()->PlaySound(L"Mantis_Lords_OST.wav", SOUND_BGM, 0.4f);
		}
		if (m_ePreState == STAND_TO_IDLE && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{
			m_bStart = true;
			m_AttackTimer.InitLoop(3.f);
		}
	}
	if (m_bStart && m_bDead && m_ePreState == DEATH && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	{
		m_pFrameKey = L"Mantis_Lord_Idle_To_Sit";
		m_eCurState = IDLE_TO_SIT;
		Update_Position();
	}
	if (m_bStart && !m_bDead)
	{
		if (!m_bAttack)
		{
			m_eCurState = IDLE;
			m_pFrameKey = L"Mantis_Lord_Idle";
		}

		Attack();

		if (m_bCheckPlayerPos)
			m_fPlayerX = CObj_Mgr::Get_Instance()->Get_Player()->Get_Info().fX;

		if (m_eCurState == DASH)
		{
			if (m_eLook == LOOK_LEFT && m_tInfo.fX > 1510.f)
			{
				m_tInfo.fX -= m_fDashSpeed;
			}
			else if (m_eLook == LOOK_RIGHT && m_tInfo.fX < 2760.f)
			{
				m_tInfo.fX += m_fDashSpeed;
			}
		}

		if (m_bStart && m_eCurState != IDLE)
		{
			INFO	tPlayerInfo = CObj_Mgr::Get_Instance()->Get_Player()->Get_Info();

			if (m_eLook == LOOK_LEFT)
				m_tCurRect = CCollision_Mgr::Collision_Ground(m_tHitBox.right, tPlayerInfo.fY);
			else
				m_tCurRect = CCollision_Mgr::Collision_Ground(m_tHitBox.left, tPlayerInfo.fY);
			//m_bLineCol = CLine_Mgr::Get_Instance()->Collision_Line(m_tInfo.fX, &fY);
			m_tInfo.fY = m_tCurRect.top - m_tInfo.fCY * 0.5f + 75.f;
		}
		else
			Update_Position();
	}

	Motion_Change();
	StateSound();
	Move_Frame(m_iLoopFrameStart);
	Update_Untuchable();
	Update_Rect();
	Update_HitBox();

	return OBJ_NOEVENT;
}

void CMantis_Lord::Late_Update(void)
{
}

void CMantis_Lord::Attack()
{
	if (m_AttackTimer.Check())
	{
		m_bAttack = true;
		m_bCheckPlayerPos = false;
	}

	if (m_bAttack)
		Update_State();
	else
	{
		m_bCheckPlayerPos = true;
		m_bDashReady = false;
		m_bSlashReady = false;
		m_bFallReady = false;
		m_DashReadyTimer.Reset();
		m_SlashReadyTimer.Reset();
		m_FallReadyTimer.Reset();
	}
}

void CMantis_Lord::Update_State()
{
	if (!m_bDead)
	{
		if (m_bSlashReady)
			Slash();
		else if (m_bDashReady)
			Dash();
		else if (m_bFallReady)
			Fall();
		else if (!m_bDashReady && !m_bSlashReady && !m_bFallReady)
		{
			if (rand() % 3 == 0)
				Slash();
			else if (rand() % 3 == 1)
				Dash();
			else
				Fall();

			Update_Position();
			Update_HitBox();
		}
	}
}

void CMantis_Lord::Dash()
{
	if (!m_bDash)
	{
		m_bDashReady = true;

		if (!m_DashReadyTimer.Check())
		{
			m_pFrameKey = L"Mantis_Lord_Dash_Ready";
			m_eCurState = DASH_READY;
		}

		else
		{
			m_bDash = true;

			m_DashTimer.InitTick(0.2f, 0.1f);
			m_DashToIdleTimer.InitTick(1.f, 0.9f);
		}
	}
	else
	{
		if (m_DashTimer.Check())
		{
			m_pFrameKey = L"Mantis_Lord_Dash";
			m_eCurState = DASH;
		}

		else if(m_DashToIdleTimer.Check())
		{
			m_pFrameKey = L"Mantis_Lord_Dash_To_Idle";
			m_eCurState = DASH_TO_IDLE;
		}
	}

	if (m_bDash && m_ePreState == DASH_TO_IDLE && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	{
		m_pFrameKey = L"Mantis_Lord_Idle";
		m_eCurState = IDLE;
		m_bDash = false;
		m_bAttack = false;
	}
}

void CMantis_Lord::Slash()
{
	if (!m_bSlash)
	{
		m_bSlashReady = true;

		if (!m_SlashReadyTimer.Check())
		{
			m_pFrameKey = L"Mantis_Lord_Slash_Ready";
			m_eCurState = SLASH_READY;
		}

		else
		{
			m_bSlash = true;

			m_SlashTimer.InitTick(0.5f, 0.4f);
			m_SlashToIdleTimer.InitTick(2.f, 1.8f);
		}
	}
	else
	{
		if (m_SlashTimer.Check())
		{
			m_pFrameKey = L"Mantis_Lord_Slash";
			m_eCurState = SLASH;
		}

		else if (m_SlashToIdleTimer.Check())
		{
			m_pFrameKey = L"Mantis_Lord_Slash_To_Idle";
			m_eCurState = SLASH_TO_IDLE;
		}
	}

	if (m_bSlash && m_ePreState == SLASH_TO_IDLE && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	{
		m_pFrameKey = L"Mantis_Lord_Idle";
		m_eCurState = IDLE;
		m_bSlash = false;
		m_bAttack = false;
	}
}

void CMantis_Lord::Fall()
{
	if (!m_bFall)
	{
		m_bFallReady = true;

		if (!m_FallReadyTimer.Check())
		{
			m_pFrameKey = L"Mantis_Lord_Fall_Ready";
			m_eCurState = FALL_READY;
		}

		else
		{
			m_bFall = true;

			m_FallTimer.InitTick(0.2f, 0.1f);
			m_FallToIdleTimer.InitTick(0.3f, 0.2f);
		}
	}
	else
	{
		if (m_FallTimer.Check())
		{
			m_pFrameKey = L"Mantis_Lord_Fall";
			m_eCurState = FALL;
		}

		else if (m_FallToIdleTimer.Check())
		{
			m_pFrameKey = L"Mantis_Lord_Fall_To_Idle";
			m_eCurState = FALL_TO_IDLE;
			
		}
	}

	if (m_bFall && m_ePreState == FALL_TO_IDLE && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	{
		m_pFrameKey = L"Mantis_Lord_Idle";
		m_eCurState = IDLE;
		m_bFall = false;
		m_bAttack = false;
	}
}

void CMantis_Lord::Update_Position()
{
	switch (m_eCurState)
	{
	case SIT:
		if (m_eIdentify == LEFT_LORD)
		{
			m_tInfo.fX = 1834.f;
			m_tInfo.fY = 7650.f;
		}
		else if (m_eIdentify == MIDDLE_LORD)
		{
			m_tInfo.fX = 2088.f;
			m_tInfo.fY = 7550.f;
		}
		else
		{
			m_tInfo.fX = 2385.f;
			m_tInfo.fY = 7640.f;
		}
		break;

	case DASH_READY:
		if (m_fPlayerX < 2088.f)	// 2088.f == Middle Chair
			m_tInfo.fX = 2760.f;
		else
			m_tInfo.fX = 1510.f;
		break;

	case SLASH_READY:
		if (m_fPlayerX < 2088.f)	// 1080.f == Middle Chair
			m_tInfo.fX = 2760.f;
		else
			m_tInfo.fX = 1510.f;
		break;

	case FALL_READY:
		m_tInfo.fX = m_fPlayerX;
		break;

	case IDLE:
		m_tInfo.fX = 2088.f;
		m_tInfo.fY = -500.f;
		break;

	case IDLE_TO_SIT:
		if (m_eIdentify == LEFT_LORD)
		{
			m_tInfo.fX = 1834.f;
			m_tInfo.fY = 7650.f;
		}
		else if (m_eIdentify == MIDDLE_LORD)
		{
			m_tInfo.fX = 2088.f;
			m_tInfo.fY = 7550.f;
		}
		else
		{
			m_tInfo.fX = 2385.f;
			m_tInfo.fY = 7640.f;
		}
		break;
	}

	if (m_fPlayerX < 2088.f)	// 1080.f == Middle Chair
	{
		if (m_eCurState == FALL_READY)
			m_eLook = LOOK_RIGHT;
		else
			m_eLook = LOOK_LEFT;
	}
	else
	{
		if (m_eCurState == FALL_READY)
			m_eLook = LOOK_LEFT;
		else
			m_eLook = LOOK_RIGHT;
	}
}

void CMantis_Lord::Update_Untuchable()
{
	switch (m_eCurState)
	{
	case SIT:
		m_eUntouchable = ON;
		break;

	case SIT_TO_STAND:
		m_eUntouchable = ON;
		break;

	case STAND_TO_IDLE:
		m_eUntouchable = ON;
		break;

	case IDLE:
		m_eUntouchable = ON;
		break;

	case DASH_READY:
		if (m_tFrame.iFrameStart <= 2)
			m_eUntouchable = ON;
		else
			m_eUntouchable = OFF;
		break;

	case DASH:
		m_eUntouchable = OFF;
		break;

	case DASH_TO_IDLE:
		if (m_tFrame.iFrameStart <= 1)
			m_eUntouchable = OFF;
		else
			m_eUntouchable = ON;
		break;

	case SLASH_READY:
		if (m_tFrame.iFrameStart <= 2)
			m_eUntouchable = ON;
		else
			m_eUntouchable = OFF;
		break;

	case SLASH:
		m_eUntouchable = OFF;
		break;

	case SLASH_TO_IDLE:
		if (m_tFrame.iFrameStart <= 1)
			m_eUntouchable = OFF;
		else
			m_eUntouchable = ON;
		break;

	case FALL_READY:
		if (m_tFrame.iFrameStart <= 1)
			m_eUntouchable = ON;
		else
			m_eUntouchable = OFF;
		break;

	case FALL:
		m_eUntouchable = OFF;
		break;

	case FALL_TO_IDLE:
		if (m_tFrame.iFrameStart >= 13)
			m_eUntouchable = ON;
		else
			m_eUntouchable = OFF;
		break;

	case DEATH:
		m_eUntouchable = ON;
		break;

	case IDLE_TO_SIT:
		m_eUntouchable = ON;
		break;
	}
}

void CMantis_Lord::StateSound()
{
	switch (m_eCurState)
	{
	case STAND_TO_IDLE:
		if (m_tFrame.iFrameStart == 1)
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_MANTIS_LORD);
			CSoundMgr::Get_Instance()->PlaySound(L"Mantis_Lords_Jump.wav", SOUND_MANTIS_LORD, 0.4f);
		}
		break;

	case DASH_READY:
		if (m_tFrame.iFrameStart == 0)
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_MANTIS_LORD);
			CSoundMgr::Get_Instance()->PlaySound(L"Mantis_Lords_Land_Ground.wav", SOUND_MANTIS_LORD, 0.4f);
		}
		break;

	case DASH:
		if (m_tFrame.iFrameStart == 1)
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_MANTIS_LORD);
			CSoundMgr::Get_Instance()->PlaySound(L"Mantis_Lords_Dash.wav", SOUND_MANTIS_LORD, 0.4f);
		}
		break;

	case DASH_TO_IDLE:
		if (m_tFrame.iFrameStart == 2)
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_MANTIS_LORD);
			CSoundMgr::Get_Instance()->PlaySound(L"Mantis_Lords_Jump_Ground.wav", SOUND_MANTIS_LORD, 0.4f);
		}
		break;

	case SLASH_READY:
		if (m_tFrame.iFrameStart == 2)
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_MANTIS_LORD);
			CSoundMgr::Get_Instance()->PlaySound(L"Mantis_Lords_Land_Wall.wav", SOUND_MANTIS_LORD, 0.4f);
		}	
		break;

	case SLASH:
		if (m_tFrame.iFrameStart == 3)
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_MANTIS_LORD);
			CSoundMgr::Get_Instance()->PlaySound(L"Mantis_Lords_Slash.wav", SOUND_MANTIS_LORD, 0.4f);
		}
		break;

	case SLASH_TO_IDLE:
		if (m_tFrame.iFrameStart == 0)
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_MANTIS_LORD);
			CSoundMgr::Get_Instance()->PlaySound(L"Mantis_Lords_Jump_Wall.wav", SOUND_MANTIS_LORD, 0.4f);
		}
		break;

	case FALL_READY:
		if (m_tFrame.iFrameStart == 1)
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_MANTIS_LORD);
			CSoundMgr::Get_Instance()->PlaySound(L"Mantis_Lords_Fall_Ready.wav", SOUND_MANTIS_LORD, 0.4f);
		}
		break;

	case FALL:
		if (m_tFrame.iFrameStart == 0)
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_MANTIS_LORD);
			CSoundMgr::Get_Instance()->PlaySound(L"Mantis_Lords_Fall.wav", SOUND_MANTIS_LORD, 0.4f);
		}
		break;

	case FALL_TO_IDLE:
		if (m_tFrame.iFrameStart == 13)
		{
			CSoundMgr::Get_Instance()->StopSound(SOUND_MANTIS_LORD);
			CSoundMgr::Get_Instance()->PlaySound(L"Mantis_Lords_Jump_Ground.wav", SOUND_MANTIS_LORD, 0.4f);
		}
		break;
	}
}

void CMantis_Lord::Render(HDC hDC)
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

void CMantis_Lord::Release(void)
{
}

void CMantis_Lord::Motion_Change(void)
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case SIT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 40;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 0;
			break;

		case SIT_TO_STAND:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 80;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 2;
			break;

		case STAND_TO_IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 40;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 3;
			break;

		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 40;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 0;
			break;

		case DASH_READY:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 40;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 5;
			break;

		case DASH:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 14;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 30;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 14;
			break;

		case DASH_TO_IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 30;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 3;
			break;

		case SLASH_READY:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 40;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 6;
			break;

		case SLASH:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 70;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 6;
			break;

		case SLASH_TO_IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 40;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 4;
			break;

		case FALL_READY:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 10;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 40;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 10;
			break;

		case FALL:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 40;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 2;
			break;

		case FALL_TO_IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 14;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 40;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 14;
			break;
		
		case DEATH:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 80;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 7;
			break;

		case IDLE_TO_SIT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 80;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 6;
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CMantis_Lord::Update_HitBox()
{
	if (m_eLook == RIGHT)
	{
		if (m_eCurState == SLASH_READY || m_eCurState == SLASH || m_eCurState == SLASH_TO_IDLE)
		{
			m_tHitBox.left = m_tRect.left + (LONG)200.f;
			m_tHitBox.top = m_tRect.top + (LONG)200.f;
			m_tHitBox.right = m_tRect.right - (LONG)400.f;
			m_tHitBox.bottom = m_tRect.bottom - (LONG)265.f;
		}
		else if (m_eCurState == FALL_READY)
		{
			m_tHitBox.left = m_tRect.left + (LONG)340.f;
			m_tHitBox.top = m_tRect.top + (LONG)210.f;
			m_tHitBox.right = m_tRect.right - (LONG)210.f;
			m_tHitBox.bottom = m_tRect.bottom - (LONG)345.f;
		}
		else if (m_eCurState == FALL || m_eCurState == FALL_TO_IDLE)
		{
			m_tHitBox.left = m_tRect.left + (LONG)410.f;
			m_tHitBox.top = m_tRect.top + (LONG)505.f;
			m_tHitBox.right = m_tRect.right - (LONG)195.f;
			m_tHitBox.bottom = m_tRect.bottom - (LONG)105.f;
		}
		else if (m_eCurState == SIT || m_eCurState == DEATH || m_eCurState == IDLE_TO_SIT)
		{
			m_tHitBox.left = 0;
			m_tHitBox.top = 0;
			m_tHitBox.right = 0;
			m_tHitBox.bottom = 0;
		}
		else
		{
			m_tHitBox.left = m_tRect.left + (LONG)240.f;
			m_tHitBox.top = m_tRect.top + (LONG)535.f;
			m_tHitBox.right = m_tRect.right - (LONG)205.f;
			m_tHitBox.bottom = m_tRect.bottom - (LONG)115.f;
		}
	}
	else
	{
		if (m_eCurState == SLASH_READY || m_eCurState == SLASH || m_eCurState == SLASH_TO_IDLE)
		{
			m_tHitBox.left = m_tRect.left + (LONG)400.f;
			m_tHitBox.top = m_tRect.top + (LONG)200.f;
			m_tHitBox.right = m_tRect.right - (LONG)200.f;
			m_tHitBox.bottom = m_tRect.bottom - (LONG)265.f;
		}
		else if (m_eCurState == FALL_READY)
		{
			m_tHitBox.left = m_tRect.left + (LONG)210.f;
			m_tHitBox.top = m_tRect.top + (LONG)210.f;
			m_tHitBox.right = m_tRect.right - (LONG)340.f;
			m_tHitBox.bottom = m_tRect.bottom - (LONG)345.f;
		}
		else if (m_eCurState == FALL || m_eCurState == FALL_TO_IDLE)
		{
			m_tHitBox.left = m_tRect.left + (LONG)195.f;
			m_tHitBox.top = m_tRect.top + (LONG)505.f;
			m_tHitBox.right = m_tRect.right - (LONG)410.f;
			m_tHitBox.bottom = m_tRect.bottom - (LONG)105.f;
		}
		else if (m_eCurState == SIT || m_eCurState == DEATH || m_eCurState == IDLE_TO_SIT)
		{
			m_tHitBox.left = 0;
			m_tHitBox.top = 0;
			m_tHitBox.right = 0;
			m_tHitBox.bottom = 0;
		}
		else
		{
			m_tHitBox.left = m_tRect.left + (LONG)205.f;
			m_tHitBox.top = m_tRect.top + (LONG)535.f;
			m_tHitBox.right = m_tRect.right - (LONG)240.f;
			m_tHitBox.bottom = m_tRect.bottom - (LONG)115.f;
		}
	}
}

void CMantis_Lord::Collision_Event(CObj * _OtherObj, float _fColX, float _fColY)
{
	if (!(m_dwDamagedTime + 300 < GetTickCount()))
		m_bDamaged = false;

	if (!m_bDamaged && m_eUntouchable == OFF)
	{
		CSlash_Effect*	pSlash = dynamic_cast<CSlash_Effect*>(_OtherObj);
		if (pSlash && m_dwDamagedTime + 300 < GetTickCount())
		{
			m_tStat.iHp -= _OtherObj->Get_Stat().iDamage;
			m_bDamaged = true;
			m_dwDamagedTime = GetTickCount();

			if (!m_bDead && m_tStat.iHp <= 0)
			{
				m_bDead = true;
				m_pFrameKey = L"Mantis_Lord_Death";
				m_eCurState = DEATH;
			}
		}
	}
}