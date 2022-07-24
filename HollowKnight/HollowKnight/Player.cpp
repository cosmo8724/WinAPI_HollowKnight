#include "stdafx.h"
#include "Player.h"
#include "Mantis_Lord.h"
#include "Mantis_Spike.h"

#include "Dash_Effect.h"
#include "Slash_Effect.h"
#include "UpSlash_Effect.h"
#include "DownSlash_Effect.h"
#include "FireBall.h"
#include "DoubleJump_Effect.h"
#include "Wall_Jump_Effect.h"
#include "Hit_Effect.h"
#include "Damaged_Effect.h"
#include "Focus_Effect.h"
#include "Focus_Get_Effect.h"
#include "Horizon_Spear.h"
#include "Vertical_Spear.h"

#include "Abstract_Factory.h"
#include "Collision_Mgr.h"
#include "Obj_Mgr.h"
#include "Bmp_Mgr.h"
#include "Scroll_Mgr.h"
#include "Key_Mgr.h"
#include "Line_Mgr.h"
#include "SoundMgr.h"

float	g_fSound = 1.f;

CPlayer::CPlayer()
	: m_eCurState(IDLE), m_ePreState(MOTION_END)
{
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize(void)
{
	m_tStat		 = { 50, 50, 90, 90, 0 };
	m_tInfo		 = { 500.f, 500.f, 200.f, 150.f };
	m_fSpeed	 = 10.f;

	m_eLook = LOOK_LEFT;
	m_eState = GROUND;

	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Player/Knight/Idle.bmp", L"Player_Idle");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Player/Knight/LookUp.bmp", L"Player_LookUp");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Player/Knight/LookDown.bmp", L"Player_LookDown");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Player/Knight/Run.bmp", L"Player_Run");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Player/Knight/Jump.bmp", L"Player_Jump");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Player/Knight/DoubleJump.bmp", L"Player_Double_Jump");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Player/Knight/Dash.bmp", L"Player_Dash");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Player/Knight/DashToIdle.bmp", L"Player_DashToIdle");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Player/Knight/Slash.bmp", L"Player_Slash");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Player/Knight/SlashUp.bmp", L"Player_SlashUp");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Player/Knight/SlashDown.bmp", L"Player_SlashDown");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Player/Knight/Fire_Ball.bmp", L"Player_Fire_Ball");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Player/Knight/WallSlide.bmp", L"Player_WallSlide");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Player/Knight/Damaged.bmp", L"Player_Damaged");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Player/Knight/Focus.bmp", L"Player_Focus");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Player/Knight/Focus_Get.bmp", L"Player_Focus_Get");
	CBmp_Mgr::Get_Instance()->Insert_Bmp(L"../res/Player/Knight/Focus_End.bmp", L"Player_Focus_End");

	m_pFrameKey = L"Player_Idle";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 6;
	m_tFrame.iMotion = LOOK_RIGHT;
	m_tFrame.dwFrameSpeed = 150;
	m_tFrame.dwFrameTime = GetTickCount();

	m_fOldX = WINCX >> 1;
	m_fOldY = WINCY >> 1;
}

int CPlayer::Update(void)
{
	m_bInputCheck = Key_Input();
	if (!m_bInputCheck)
	{
		m_pFrameKey = L"Player_Idle";
		m_eCurState = IDLE;
	}
	CKey_Mgr::Get_Instance()->Key_Reset();
	Offset();

	Jumping();
	if (m_bWallJump && m_iWallJumpCnt++ < m_iWallJumpMaxCnt)
	{
		if(m_eLook == LOOK_LEFT)
			m_tInfo.fX += m_iWallJumpTickSpeed;
		else
			m_tInfo.fX -= m_iWallJumpTickSpeed;
	}
	Update_Dash();
	Update_Damaged();
	Motion_Change();

	if(m_eLook == LOOK_LEFT)
		CurRect = CCollision_Mgr::Collision_Ground(m_tHitBox.right, m_tInfo.fY);
	else
		CurRect = CCollision_Mgr::Collision_Ground(m_tHitBox.left, m_tInfo.fY);

	// 점프 없이 아래 쪽 땅으로 떨어질 때
	if (CurRect.top != m_lPreGroundY || CurRect.top == 0)
	{
		if ((m_eState == GROUND && CurRect.top > m_lPreGroundY) || (m_eState == WALL && m_tHitBox.top > CurRect.bottom) || (m_eState == GROUND && CurRect.top == 0))
		{
			m_eState = AIR;
			m_bJump = true;
			m_fCurJumpSpeed = 0;
			m_pFrameKey = L"Player_Jump";
			m_eCurState = JUMP;
		}
			m_lPreGroundY = CurRect.top;
	}
	//m_bLineCol = CLine_Mgr::Get_Instance()->Collision_Line(m_tInfo.fX, &fY);

	Move_Frame(m_iLoopFrameStart);
	if (m_bDash && m_tFrame.iFrameStart == 7)
	{
		m_eCurState = DASH_TO_IDLE;
		m_pFrameKey = L"Player_DashToIdle";
	}
	if (m_eCurState == DASH_TO_IDLE && m_tFrame.iFrameStart == 3)
	{
		m_eCurState = IDLE;
		m_pFrameKey = L"Player_Idle";
	}
	if (m_ePreState == DOUBLE_JUMP && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	{
		m_eCurState = JUMP;
		m_pFrameKey = L"Player_Jump";
	}

	if (m_bSoulCharge)
	{
		m_dwSoulChargeEndTime = GetTickCount();
	}

	switch (m_eState)
	{
	case AIR:
		if (m_tHitBox.bottom <= CurRect.top || CurRect.top == 0)
		{
			if (m_eCurState == DAMAGED)
			{
				m_tInfo.fY -= m_fCurJumpSpeed * 0.1f;
				if (fabs(m_fCurJumpSpeed) < m_fMaxAbsJumpSpeed) // limit max speed
					m_fCurJumpSpeed -= GRAVITY;
				m_bJump = true;
			}
			else
			{
				m_tInfo.fY -= m_fCurJumpSpeed;
				if (fabs(m_fCurJumpSpeed) < m_fMaxAbsJumpSpeed) // limit max speed
					m_fCurJumpSpeed -= GRAVITY;
				m_bJump = true;
				if (m_bDoubleJump && m_iDoubleJumpCnt)
				{
					m_fCurJumpSpeed = m_fInitJumpSpeed;
					m_iDoubleJumpCnt--;
				}
			}
		}
		else
		{
			//m_tInfo.fY = CurRect.top - (m_tHitBox.bottom - m_tHitBox.top) * 0.5f;
			m_eState = GROUND;
			m_eCurState = RUN;
			m_pFrameKey = L"Player_Run";
			m_fCurJumpSpeed = 0.f;
			CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"Land.wav", SOUND_PLAYER, 0.4f);
		}
		break;
	case WALL:
		m_fCurJumpSpeed = 0.f;
		if (m_tHitBox.bottom < CurRect.top)
			m_tInfo.fY += m_fWallSpeed;
		else
		{
			m_tInfo.fY = CurRect.top - (m_tHitBox.bottom - m_tHitBox.top) * 0.5f;
			m_eState = GROUND;
		}
		m_bDoubleJump = false;
		m_bWallJump = false;
		m_iWallJumpCnt = 0;
		break;
	case GROUND:
		if (m_ePreState == JUMP || m_ePreState == DOUBLE_JUMP)
		{
			m_eCurState = IDLE;
			m_pFrameKey = L"Player_Idle";
		}
		m_fCurJumpSpeed = 0.f;
		m_iDoubleJumpCnt = 1;
		//m_tInfo.fY = CurRect.top - (m_tHitBox.bottom - m_tHitBox.top) * 0.5f;
		m_bJump = false;
		m_bDoubleJump = false;
		m_bWallJump = false;
		m_iWallJumpCnt = 0;
		break;

	default:
		break;
	}

	Update_Rect();
	Update_HitBox();

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update(void)
{
}

void CPlayer::Render(HDC hDC)
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

void CPlayer::Release(void)
{
}

bool CPlayer::Key_Input(void)
{
	bool bInputCheck = false;

	if (m_bDamaged)
		return false;

	if (m_bSoulCharge)
	{
		if (m_eState == GROUND && CKey_Mgr::Get_Instance()->Key_Up('A'))
		{
			DWORD	PressTime = m_dwSoulChargeEndTime - m_dwSoulChargeStartTime;

			if (PressTime > 800 && m_bSoulCharge)
			{
				m_pFrameKey = L"Player_Focus_Get";
				m_eCurState = FOCUS_GET;
				m_tStat.iSoul -= 30;
				m_tStat.iHp += 10;
				m_bSoulCharge = false;
				m_iShakeOffset = 0;
				CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CFocus_Get_Effect>::Create(m_tInfo.fX, m_tInfo.fY - 20.f));
				CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
				CSoundMgr::Get_Instance()->PlaySound(L"Focus_Get.wav", SOUND_PLAYER, 0.4f);
			}
			else
			{
				m_pFrameKey = L"Player_Focus_End";
				m_eCurState = FOCUS_END;
				m_bSoulCharge = false;
			}
		}
		return true;
	}

	if (m_eCurState == JUMP || m_eCurState == DASH || m_eCurState == SLASH || m_eCurState == DOUBLE_JUMP || m_eCurState == FIRE_BALL || m_eCurState == FOCUS || m_eCurState == FOCUS_GET || m_eCurState == FOCUS_END)
	{
		bInputCheck = true;
		
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd && m_eCurState != JUMP && m_eCurState != DOUBLE_JUMP && m_eCurState != FOCUS)
			bInputCheck = false;
	}

	if (CKey_Mgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		m_tInfo.fX -= m_fSpeed;
		m_eLook = LOOK_LEFT;
		if (m_eCurState != JUMP && m_eCurState != DOUBLE_JUMP && m_eCurState != DASH && m_eCurState != SLASH)
		{
			bInputCheck = true;
			m_eCurState = RUN;
			m_pFrameKey = L"Player_Run";
			CSoundMgr::Get_Instance()->PlaySound(L"FootSteps.wav", SOUND_PLAYER, 0.4f);
		}
		if (m_eState == GROUND && CKey_Mgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			m_pFrameKey = L"Player_Jump";
			m_ePreState = RUN;
			m_eCurState = JUMP;
			m_eState = AIR;
			Jumping();
			CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"Jump.wav", SOUND_PLAYER, 0.4f);
		}
		if (m_eState == AIR && CKey_Mgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			m_pFrameKey = L"Player_Double_Jump";
			m_eCurState = DOUBLE_JUMP;
			if (!m_bDoubleJump)
			{
				CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CDoubleJump_Effect>::Create(m_tInfo.fX, m_tInfo.fY + 20.f));
				m_bDoubleJump = true;
			}
			CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"Double_Jump.wav", SOUND_PLAYER, 0.4f);
		}
		if (m_eState == WALL)
		{
			m_pFrameKey = L"Player_WallSlide";
			m_eCurState = WALL_SLIDE;
			//CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"Wall_Slide.wav", SOUND_PLAYER, 0.4f);

			if (CKey_Mgr::Get_Instance()->Key_Down(VK_SPACE))
			{
				if (!m_bWallJump)
				{
					m_pFrameKey = L"Player_Jump";
					m_eCurState = JUMP;
					m_eState = AIR;
					m_fCurJumpSpeed = m_fInitJumpSpeed;
					m_bWallJump = true;
					CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CWall_Jump_Effect>::Create(m_tHitBox.left + 100, m_tHitBox.bottom - 100));
					CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
					CSoundMgr::Get_Instance()->PlaySound(L"Wall_Jump.wav", SOUND_PLAYER, 0.4f);
				}
			}
		}
		if (CKey_Mgr::Get_Instance()->Key_Down('D'))
		{
			if (!m_bDash)
			{
				m_pFrameKey = L"Player_Dash";
				m_eCurState = DASH;
				m_bDash = true;
				m_iDashCnt = 0;
				m_dwDashWaitTime = GetTickCount();
				m_fStart_fY = m_tInfo.fY;
				if (m_eLook == LOOK_LEFT)
					CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CDash_Effect>::Create(float(m_tRect.right) + 60.f, m_tInfo.fY));
				else
					CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CDash_Effect>::Create(float(m_tRect.left) - 60.f, m_tInfo.fY));
				CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
				CSoundMgr::Get_Instance()->PlaySound(L"Dash.wav", SOUND_PLAYER, 0.4f);
			}
		}
		if (CKey_Mgr::Get_Instance()->Key_Down('S'))
		{
			if (m_bLookUp)
				m_pFrameKey = L"Player_SlashUp";
			else if (m_bLookDown)
				m_pFrameKey = L"Player_SlashDown";
			else if (!m_bLookUp && !m_bLookDown)
				m_pFrameKey = L"Player_Slash";
			m_eCurState = SLASH;
			if (!m_bLookDown && !m_bLookUp)
			{
				if (m_eLook == LOOK_LEFT)
					CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CSlash_Effect>::Create(float(m_tRect.left), m_tInfo.fY));
				else
					CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CSlash_Effect>::Create(float(m_tRect.right), m_tInfo.fY));
			}
			else if (m_bLookUp)
			{
				CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CUpSlash_Effect>::Create(m_tInfo.fX, m_tInfo.fY));
			}
			else if (m_bLookDown)
			{
				CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CDownSlash_Effect>::Create(m_tInfo.fX, m_tInfo.fY));
			}
			CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
			if (GetTickCount() % 5 == 0)
				CSoundMgr::Get_Instance()->PlaySound(L"Slash_1.wav", SOUND_PLAYER, 0.4f);
			else if (GetTickCount() % 5 == 1)
				CSoundMgr::Get_Instance()->PlaySound(L"Slash_2.wav", SOUND_PLAYER, 0.4f);
			else if (GetTickCount() % 5 == 2)
				CSoundMgr::Get_Instance()->PlaySound(L"Slash_3.wav", SOUND_PLAYER, 0.4f);
			else if (GetTickCount() % 5 == 3)
				CSoundMgr::Get_Instance()->PlaySound(L"Slash_4.wav", SOUND_PLAYER, 0.4f);
			else if (GetTickCount() % 5 == 4)
				CSoundMgr::Get_Instance()->PlaySound(L"Slash_5.wav", SOUND_PLAYER, 0.4f);
		}
	}
	else if (m_eState == WALL && CKey_Mgr::Get_Instance()->Key_Up(VK_LEFT))
	{
		if (!m_bWallJump)
		{
			m_pFrameKey = L"Player_Jump";
			m_eCurState = JUMP;
			m_eState = AIR;
		}
	}
	else if (m_eState == GROUND && CKey_Mgr::Get_Instance()-> Key_Up(VK_LEFT))
		CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);

	if (CKey_Mgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		m_tInfo.fX += m_fSpeed;
		m_eLook = LOOK_RIGHT;
		if (m_eCurState != JUMP && m_eCurState != DOUBLE_JUMP && m_eCurState != DASH && m_eCurState != SLASH)
		{
			bInputCheck = true;
			m_pFrameKey = L"Player_Run";
			m_eCurState = RUN;
			CSoundMgr::Get_Instance()->PlaySound(L"FootSteps.wav", SOUND_PLAYER, 0.4f);
		}
		if (m_eState == GROUND && CKey_Mgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			m_pFrameKey = L"Player_Jump";
			m_ePreState = RUN;
			m_eCurState = JUMP;
			m_eState = AIR;
			Jumping();
			CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"Jump.wav", SOUND_PLAYER, 0.4f);
		}
		if (m_eState == AIR && CKey_Mgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			m_pFrameKey = L"Player_Double_Jump";
			m_eCurState = DOUBLE_JUMP;
			if (!m_bDoubleJump)
			{
				CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CDoubleJump_Effect>::Create(m_tInfo.fX, m_tInfo.fY + 20.f));
				m_bDoubleJump = true;
			}
			CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"Double_Jump.wav", SOUND_PLAYER, 0.4f);
		}
		if (m_eState == WALL)
		{
			m_pFrameKey = L"Player_WallSlide";
			m_eCurState = WALL_SLIDE;
			//CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"Wall_Slide.wav", SOUND_PLAYER, 0.4f);

			if (CKey_Mgr::Get_Instance()->Key_Down(VK_SPACE))
			{
				m_pFrameKey = L"Player_Jump";
				m_eCurState = JUMP;
				m_eState = AIR;
				m_fCurJumpSpeed = m_fInitJumpSpeed;
				m_bWallJump = true;
				CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CWall_Jump_Effect>::Create(m_tHitBox.right - 100, m_tHitBox.bottom - 100));
				CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
				CSoundMgr::Get_Instance()->PlaySound(L"Wall_Jump.wav", SOUND_PLAYER, 0.4f);
			}
		}
		if (CKey_Mgr::Get_Instance()->Key_Down('D'))
		{
			if (!m_bDash)
			{
				m_pFrameKey = L"Player_Dash";
				m_eCurState = DASH;
				m_bDash = true;
				m_iDashCnt = 0;
				m_dwDashWaitTime = GetTickCount();
				m_fStart_fY = m_tInfo.fY;
				if (m_eLook == LOOK_LEFT)
					CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CDash_Effect>::Create(float(m_tRect.right) + 60.f, m_tInfo.fY));
				else
					CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CDash_Effect>::Create(float(m_tRect.left) - 60.f, m_tInfo.fY));
				CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
				CSoundMgr::Get_Instance()->PlaySound(L"Dash.wav", SOUND_PLAYER, 0.4f);
			}
		}
		if (CKey_Mgr::Get_Instance()->Key_Down('S'))
		{
			if (m_bLookUp)
				m_pFrameKey = L"Player_SlashUp";
			else if (m_bLookDown)
				m_pFrameKey = L"Player_SlashDown";
			else if (!m_bLookUp && !m_bLookDown)
				m_pFrameKey = L"Player_Slash";
			m_eCurState = SLASH;
			if (!m_bLookUp && !m_bLookDown)
			{
				if (m_eLook == LOOK_LEFT)
					CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CSlash_Effect>::Create(float(m_tRect.left), m_tInfo.fY));
				else
					CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CSlash_Effect>::Create(float(m_tRect.right), m_tInfo.fY));
			}
			else if (m_bLookUp)
			{
				CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CUpSlash_Effect>::Create(m_tInfo.fX, m_tInfo.fY));
			}
			else if (m_bLookDown)
			{
				CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CDownSlash_Effect>::Create(m_tInfo.fX, m_tInfo.fY));
			}
			CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
			if (GetTickCount() % 5 == 0)
				CSoundMgr::Get_Instance()->PlaySound(L"Slash_1.wav", SOUND_PLAYER, 0.4f);
			else if (GetTickCount() % 5 == 1)
				CSoundMgr::Get_Instance()->PlaySound(L"Slash_2.wav", SOUND_PLAYER, 0.4f);
			else if (GetTickCount() % 5 == 2)
				CSoundMgr::Get_Instance()->PlaySound(L"Slash_3.wav", SOUND_PLAYER, 0.4f);
			else if (GetTickCount() % 5 == 3)
				CSoundMgr::Get_Instance()->PlaySound(L"Slash_4.wav", SOUND_PLAYER, 0.4f);
			else if (GetTickCount() % 5 == 4)
				CSoundMgr::Get_Instance()->PlaySound(L"Slash_5.wav", SOUND_PLAYER, 0.4f);
		}
	}
	else if (m_eState == WALL && CKey_Mgr::Get_Instance()->Key_Up(VK_RIGHT))
	{
		if (!m_bWallJump)
		{
			m_pFrameKey = L"Player_Jump";
			m_eCurState = JUMP;
			m_eState = AIR;
		}
	}
	else if (m_eState == GROUND && CKey_Mgr::Get_Instance()->Key_Up(VK_RIGHT))
		CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);

	if (CKey_Mgr::Get_Instance()->Key_Pressing(VK_UP))
	{
		m_bLookUp = true;
		if (m_eCurState != JUMP && m_eCurState != DASH && m_eCurState != SLASH)
		{
			bInputCheck = true;
			m_pFrameKey = L"Player_LookUp";
			m_eCurState = LOOK_UP;
		}
		if (CKey_Mgr::Get_Instance()->Key_Down('S'))
		{
			m_pFrameKey = L"Player_SlashUp";
			m_eCurState = SLASH;
			CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CUpSlash_Effect>::Create(m_tInfo.fX, m_tInfo.fY));
			CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
			if (GetTickCount() % 5 == 0)
				CSoundMgr::Get_Instance()->PlaySound(L"Slash_1.wav", SOUND_PLAYER, 0.4f);
			else if (GetTickCount() % 5 == 1)
				CSoundMgr::Get_Instance()->PlaySound(L"Slash_2.wav", SOUND_PLAYER, 0.4f);
			else if (GetTickCount() % 5 == 2)
				CSoundMgr::Get_Instance()->PlaySound(L"Slash_3.wav", SOUND_PLAYER, 0.4f);
			else if (GetTickCount() % 5 == 3)
				CSoundMgr::Get_Instance()->PlaySound(L"Slash_4.wav", SOUND_PLAYER, 0.4f);
			else if (GetTickCount() % 5 == 4)
				CSoundMgr::Get_Instance()->PlaySound(L"Slash_5.wav", SOUND_PLAYER, 0.4f);
		}
	}
	else if (m_bLookUp && CKey_Mgr::Get_Instance()->Key_Up(VK_UP))
	{
		m_bLookUp = false;
		m_pFrameKey = L"Player_Idle";
		m_eCurState = IDLE;
	}

	if (CKey_Mgr::Get_Instance()->Key_Pressing(VK_DOWN))
	{
		m_bLookDown = true;
		if (m_eCurState != JUMP && m_eCurState != DASH && m_eCurState != SLASH)
		{
			bInputCheck = true;
			m_pFrameKey = L"Player_LookDown";
			m_eCurState = LOOK_DOWN;
		}
		if (m_eState == AIR && CKey_Mgr::Get_Instance()->Key_Down('S'))
		{
			m_pFrameKey = L"Player_SlashDown";
			m_eCurState = SLASH;
			CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CDownSlash_Effect>::Create(m_tInfo.fX, m_tInfo.fY));
			CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
			if (GetTickCount() % 5 == 0)
				CSoundMgr::Get_Instance()->PlaySound(L"Slash_1.wav", SOUND_PLAYER, 0.4f);
			else if (GetTickCount() % 5 == 1)
				CSoundMgr::Get_Instance()->PlaySound(L"Slash_2.wav", SOUND_PLAYER, 0.4f);
			else if (GetTickCount() % 5 == 2)
				CSoundMgr::Get_Instance()->PlaySound(L"Slash_3.wav", SOUND_PLAYER, 0.4f);
			else if (GetTickCount() % 5 == 3)
				CSoundMgr::Get_Instance()->PlaySound(L"Slash_4.wav", SOUND_PLAYER, 0.4f);
			else if (GetTickCount() % 5 == 4)
				CSoundMgr::Get_Instance()->PlaySound(L"Slash_5.wav", SOUND_PLAYER, 0.4f);
		}
	}
	else if (m_bLookDown && CKey_Mgr::Get_Instance()->Key_Up(VK_DOWN))
	{
		m_bLookDown = false;
		m_pFrameKey = L"Player_Idle";
		m_eCurState = IDLE;
	}

	if (CKey_Mgr::Get_Instance()->Key_Down('D'))
	{
		bInputCheck = true;
		if (!m_bDash)
		{
			m_pFrameKey = L"Player_Dash";
			m_eCurState = DASH;
			m_bDash = true;
			m_iDashCnt = 0;
			m_dwDashWaitTime = GetTickCount();
			m_fStart_fY = m_tInfo.fY;
			if(m_eLook == LOOK_LEFT)
				CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CDash_Effect>::Create(m_tRect.right + 60.f, m_tInfo.fY));
			else
				CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CDash_Effect>::Create(m_tRect.left - 60.f, m_tInfo.fY));
			CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"Dash.wav", SOUND_EFFECT, g_fSound);
		}
	}
	if (CKey_Mgr::Get_Instance()->Key_Down('S'))
	{
		bInputCheck = true;
		if (m_bLookUp)
			m_pFrameKey = L"Player_SlashUp";
		else
			m_pFrameKey = L"Player_Slash";
		m_eCurState = SLASH;
		if (m_eLook == LOOK_LEFT)
			CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CSlash_Effect>::Create(m_tRect.left + 30.f, m_tInfo.fY + 10.f));
		else
			CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CSlash_Effect>::Create(m_tRect.right - 30.f, m_tInfo.fY + 10.f));
		
		CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
		if (GetTickCount() % 5 == 0)
			CSoundMgr::Get_Instance()->PlaySound(L"Slash_1.wav", SOUND_PLAYER, 0.4f);
		else if (GetTickCount() % 5 == 1)
			CSoundMgr::Get_Instance()->PlaySound(L"Slash_2.wav", SOUND_PLAYER, 0.4f);
		else if (GetTickCount() % 5 == 2)
			CSoundMgr::Get_Instance()->PlaySound(L"Slash_3.wav", SOUND_PLAYER, 0.4f);
		else if (GetTickCount() % 5 == 3)
			CSoundMgr::Get_Instance()->PlaySound(L"Slash_4.wav", SOUND_PLAYER, 0.4f);
		else if (GetTickCount() % 5 == 4)
			CSoundMgr::Get_Instance()->PlaySound(L"Slash_5.wav", SOUND_PLAYER, 0.4f);
	}
	if (m_eState == GROUND && CKey_Mgr::Get_Instance()->Key_Down(VK_SPACE))
	{
		bInputCheck = true;
		m_pFrameKey = L"Player_Jump";
		m_ePreState = RUN;
		m_eCurState = JUMP;
		m_eState = AIR;
		CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
		CSoundMgr::Get_Instance()->PlaySound(L"Jump.wav", SOUND_PLAYER, 0.4f);
	}
	if (m_eState == AIR && CKey_Mgr::Get_Instance()->Key_Down(VK_SPACE) && m_iDoubleJumpCnt)
	{
		bInputCheck = true;
		m_pFrameKey = L"Player_Double_Jump";
		m_eCurState = DOUBLE_JUMP;
		m_bDoubleJump = true;
		CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CDoubleJump_Effect>::Create(m_tInfo.fX, m_tInfo.fY + 20.f));
		CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
		CSoundMgr::Get_Instance()->PlaySound(L"Double_Jump.wav", SOUND_PLAYER, 0.4f);
	}
	if (m_eState == GROUND && m_tStat.iSoul >= 30 && m_tStat.iHp != 50 && CKey_Mgr::Get_Instance()->Key_Down('A'))
	{
		bInputCheck = true;
		m_pFrameKey = L"Player_Focus";
		m_eCurState = FOCUS;
		m_bSoulCharge = true;
		m_dwSoulChargeStartTime = GetTickCount();
		CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CFocus_Effect>::Create(m_tInfo.fX, m_tInfo.fY - 40.f));
		CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
		CSoundMgr::Get_Instance()->PlaySound(L"Focus.wav", SOUND_PLAYER, 0.4f);
	}
	if (m_tStat.iSoul >= 40 && CKey_Mgr::Get_Instance()->Key_Down('W'))
	{
		bInputCheck = true;
		m_pFrameKey = L"Player_Fire_Ball";
		m_eCurState = FIRE_BALL;
		m_dwFireStartTime = GetTickCount();
		m_iShakeOffset = 0;
		m_tStat.iSoul -= 40;
		g_iFrameSpeed = 100;
		CObj_Mgr::Get_Instance()->Add_Object(OBJ_SWORD, CAbstract_Factory<CFireBall>::Create(m_tInfo.fX, m_tInfo.fY));
		CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
		CSoundMgr::Get_Instance()->PlaySound(L"FireBall.wav", SOUND_PLAYER, 0.4f);
	}

	if (CKey_Mgr::Get_Instance()->Key_Down(VK_F1) && m_tStat.iHp > 0)
		m_tStat.iHp -= 10;

	if (CKey_Mgr::Get_Instance()->Key_Down(VK_F2) && m_tStat.iHp < 50)
		m_tStat.iHp += 10;

	if (CKey_Mgr::Get_Instance()->Key_Down(VK_F3) && m_tStat.iSoul > 0)
		m_tStat.iSoul -= 10;

	if (CKey_Mgr::Get_Instance()->Key_Down(VK_F4) && m_tStat.iSoul < 90)
		m_tStat.iSoul += 10;
	
	return bInputCheck;
}

void CPlayer::Jumping(void)
{
	if (!m_bJump)
	{
		m_fCurJumpSpeed = m_fInitJumpSpeed;
	}

	if (m_eCurState == DOUBLE_JUMP && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	{
		m_pFrameKey = L"Player_Jump";
		m_tFrame.iFrameStart = 7;
		m_iLoopFrameStart = 7;
	}
}

void CPlayer::Update_Dash()
{
	if (m_dwDashWaitTime + 500 < GetTickCount())
	{
		m_bDash = false;
	}
	if (m_bDash && m_iDashCnt++ < m_iDashMaxCnt)
	{
		if (m_eLook == LOOK_LEFT)
			m_tInfo.fX -= m_fDashSpeed;
		else if (m_eLook == LOOK_RIGHT)
			m_tInfo.fX += m_fDashSpeed;
		m_tInfo.fY = m_fStart_fY;
		m_fCurJumpSpeed = 0;
	}
}

void CPlayer::Update_Damaged()
{
	if (m_bDamaged)
	{
		m_pFrameKey = L"Player_Damaged";
		m_eCurState = DAMAGED;

		if (m_dwDamagedTime + 1000 < GetTickCount())
			m_bDamaged = false;

		else if (m_dwDamagedTime + 300 < GetTickCount())
			g_iFrameSpeed = 10;
		
		else if (m_dwDamagedTime + 100 < GetTickCount())
			g_iFrameSpeed = 100;

		if (m_bDamaged && m_iKnockBackCnt++ < m_iKnockBackMaxCnt)
		{
			m_eState = AIR;
			
			if (m_eLook == LOOK_LEFT)
				m_tInfo.fX += m_fKnockBackSpeed;
			else
				m_tInfo.fX -= m_fKnockBackSpeed;
		}
	}
}

void CPlayer::Offset(void)
{
	//int		iOffsetX = WINCX >> 1;
	float	fOffsetMinX = 790.f;
	float	fOffsetMaxX = 810.f;
	float	fOffsetMinY = 100.f;
	float	fOffsetMaxY = 450.f;
	float	fDiffX = m_tInfo.fX - m_fOldX;
	float	fDiffY = m_tInfo.fY - m_fOldY;
	int		iScrollX = int(CScroll_Mgr::Get_Instance()->Get_ScrollX());
	int		iScrollY = int(CScroll_Mgr::Get_Instance()->Get_ScrollY());



	if (fOffsetMinX > m_tInfo.fX + iScrollX)
		CScroll_Mgr::Get_Instance()->Set_ScrollX(-fDiffX);

	if (fOffsetMaxX < m_tInfo.fX + iScrollX)
		CScroll_Mgr::Get_Instance()->Set_ScrollX(-fDiffX);
	
	if (fOffsetMinY > m_tInfo.fY + iScrollY)
		CScroll_Mgr::Get_Instance()->Set_ScrollY(-fDiffY);

	if (fOffsetMaxY < m_tInfo.fY + iScrollY)
		CScroll_Mgr::Get_Instance()->Set_ScrollY(-fDiffY);
	
	m_fOldX = m_tInfo.fX;
	m_fOldY = m_tInfo.fY;

	if (m_dwFireStartTime + 200 < GetTickCount())
		g_iFrameSpeed = 10;

	if (m_iShakeOffset < 20)
	{
		int	iShakeX = sin(GetTickCount()) * 50;
		int	iShakeY = sin(GetTickCount()) * 30;

		if (m_iShakeOffset < 10)
		{
			CScroll_Mgr::Get_Instance()->Set_ScrollX(-iShakeX);
			m_iShakeX[m_iShakeOffset] = iShakeX;
			CScroll_Mgr::Get_Instance()->Set_ScrollY(-iShakeY);
			m_iShakeY[m_iShakeOffset] = iShakeY;
		}
		else
		{
			iShakeX = m_iShakeX[m_iShakeOffset - 10];
			CScroll_Mgr::Get_Instance()->Set_ScrollX(iShakeX);
			iShakeY = m_iShakeY[m_iShakeOffset - 10];
			CScroll_Mgr::Get_Instance()->Set_ScrollY(iShakeY);
		}
		m_iShakeOffset++;
	}
}

void CPlayer::Motion_Change(void)
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 0;
			break;

		case LOOK_UP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;

			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 1;
			break;

		case LOOK_DOWN:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 1;
			break;

		case RUN:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 12;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;

			m_tFrame.dwFrameSpeed = 80;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 3;
			break;

		case JUMP:
			if (m_ePreState == DOUBLE_JUMP)
				m_tFrame.iFrameStart = 8;
			else
				m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 11;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 45;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 8;
			break;

		case DOUBLE_JUMP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 60;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 0;
			break;

		case DASH:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 40;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 0;
			break;

		case DASH_TO_IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 0;
			break;

		case SLASH:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 40;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 0;
			break;

		case WALL_SLIDE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 40;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 0;
			break;

		case DAMAGED:
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

		case FOCUS:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 40;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 2;
			break;

		case FOCUS_GET:
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

		case FOCUS_END:
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

		case FIRE_BALL:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;

			if (m_eLook == LOOK_LEFT)
				m_tFrame.iMotion = LOOK_LEFT;
			else
				m_tFrame.iMotion = LOOK_RIGHT;
			m_tFrame.dwFrameSpeed = 40;
			m_tFrame.dwFrameTime = GetTickCount();
			m_iLoopFrameStart = 8;
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CPlayer::HitKnockBack()
{
	if (m_eState == AIR)
	{
		if (m_bLookDown)
			m_fCurJumpSpeed = m_fInitJumpSpeed * 0.5f;
		else if (m_bLookUp)
			m_fCurJumpSpeed -= m_fKnockBackSpeed * 3.f;
	}
	else
	{
		if (m_eLook == LOOK_LEFT)
		{
			m_tInfo.fX += m_fKnockBackSpeed;
		}
		else
		{
			m_tInfo.fX -= m_fKnockBackSpeed;
		}
	}
}

bool CPlayer::Move_Frame(int _InitFrame)
{
	bool bCheckEnd = false;

	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		m_tFrame.iFrameStart++;

		m_tFrame.dwFrameTime = GetTickCount();

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{


			m_tFrame.iFrameStart = _InitFrame;
			bCheckEnd = true;
		}
	}

	return bCheckEnd;
}

void CPlayer::Update_HitBox()
{
	m_tHitBox.left = m_tRect.left + 70;
	m_tHitBox.right = m_tRect.right - 75;

	if (m_eCurState == DASH && m_tFrame.iFrameStart > 0 && m_tFrame.iFrameStart < 6)
	{
		m_tHitBox.top = m_tRect.top + 64;
		m_tHitBox.bottom = m_tRect.top + 115;
	}
	else
	{
		m_tHitBox.top = m_tRect.top + 14;
		m_tHitBox.bottom = m_tRect.bottom - 9;
	}
}

void CPlayer::Collision_Event(CObj * _OtherObj, float _fColX, float _fColY)
{
	CMantis_Lord*	pMantisLord = dynamic_cast<CMantis_Lord*>(_OtherObj);
	if (pMantisLord)
	{
		UNTOUCHABLE	eUntouchable = pMantisLord->Get_Untouchable(); 

		if (!eUntouchable && !m_bDamaged)
		{
			m_tStat.iHp -= pMantisLord->Get_Stat().iDamage;
			m_bDamaged = true;
			m_iShakeOffset = 0;
			m_fLastScrollX = CScroll_Mgr::Get_Instance()->Get_ScrollX();
			m_fLastScrollY = CScroll_Mgr::Get_Instance()->Get_ScrollY();
			m_iKnockBackCnt = 0;
			m_dwDamagedTime = GetTickCount();
			CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CDamaged_Effect>::Create((m_tHitBox.right + m_tHitBox.left) * 0.5f, (m_tHitBox.top + m_tHitBox.bottom) * 0.5f));
			CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"Damaged.wav", SOUND_PLAYER, 1.f);
		}
		if (m_tStat.iHp <= 0)
			m_bDead = true;
	}

	CMantis_Spike*	pMantisSpike = dynamic_cast<CMantis_Spike*>(_OtherObj);
	if (pMantisSpike)
	{
		if (!m_bDamaged)
		{
			m_tStat.iHp -= 10;
			m_bDamaged = true;
			m_iKnockBackCnt = 0;
			m_fLastScrollX = CScroll_Mgr::Get_Instance()->Get_ScrollX();
			m_fLastScrollY = CScroll_Mgr::Get_Instance()->Get_ScrollY();
			m_dwDamagedTime = GetTickCount();
			CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CDamaged_Effect>::Create((m_tHitBox.right + m_tHitBox.left) * 0.5f, (m_tHitBox.top + m_tHitBox.bottom) * 0.5f));
			CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"Damaged.wav", SOUND_PLAYER, 1.f);
		}
		if (m_tStat.iHp <= 0)
			m_bDead = true;
	}

	CHorizon_Spear*	pHorizonSpear = dynamic_cast<CHorizon_Spear*>(_OtherObj);
	if (pHorizonSpear)
	{
		if (!m_bDamaged)
		{
			m_tStat.iHp -= 10;
			m_bDamaged = true;
			m_iShakeOffset = 0;
			m_iKnockBackCnt = 0;
			m_fLastScrollX = CScroll_Mgr::Get_Instance()->Get_ScrollX();
			m_fLastScrollY = CScroll_Mgr::Get_Instance()->Get_ScrollY();
			m_dwDamagedTime = GetTickCount();
			CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CDamaged_Effect>::Create((m_tHitBox.right + m_tHitBox.left) * 0.5f, (m_tHitBox.top + m_tHitBox.bottom) * 0.5f));
			CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"Damaged.wav", SOUND_PLAYER, 1.f);
		}
		if (m_tStat.iHp <= 0)
			m_bDead = true;
	}

	CVertical_Spear* pVerticalSpear = dynamic_cast<CVertical_Spear*>(_OtherObj);
	if (pVerticalSpear)
	{
		if (!m_bDamaged)
		{
			m_tStat.iHp -= 10;
			m_bDamaged = true;
			m_iShakeOffset = 0;
			m_iKnockBackCnt = 0;
			m_fLastScrollX = CScroll_Mgr::Get_Instance()->Get_ScrollX();
			m_fLastScrollY = CScroll_Mgr::Get_Instance()->Get_ScrollY();
			m_dwDamagedTime = GetTickCount();
			CObj_Mgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstract_Factory<CDamaged_Effect>::Create((m_tHitBox.right + m_tHitBox.left) * 0.5f, (m_tHitBox.top + m_tHitBox.bottom) * 0.5f));
			CSoundMgr::Get_Instance()->StopSound(SOUND_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"Damaged.wav", SOUND_PLAYER, 1.f);
		}
		if (m_tStat.iHp <= 0)
			m_bDead = true;
	}
}