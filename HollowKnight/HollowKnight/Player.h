#pragma once
#include "Obj.h"
class CPlayer :	public CObj
{
	enum STATE { IDLE, LOOK_UP, LOOK_DOWN, WALK, RUN, DASH, DASH_TO_IDLE, JUMP, DOUBLE_JUMP, SLASH, HIT, WALL_SLIDE, DAMAGED, DEAD, FOCUS, FOCUS_GET, FOCUS_END, FIRE_BALL, MOTION_END };

private:
	bool	m_bInputCheck = false;
	int		m_iLoopFrameStart = 0;
	LONG	m_lPreGroundY;
	RECT	CurRect;

	// Look_State
	bool	m_bLookUp = false;
	bool	m_bLookDown = false;

	// Jump
	bool	m_bJump = false;
	bool	m_bDoubleJump = false;
	int		m_iDoubleJumpCnt = 1;
	float	m_fMaxAbsJumpSpeed = 36.f;
	float	m_fInitJumpSpeed = 25.f;
	float	m_fCurJumpSpeed = 0.f;

	// Dash
	bool	m_bDash = false;
	DWORD	m_dwDashWaitTime;
	int		m_iDashCnt = 0;
	int		m_iDashMaxCnt = 15;
	float	m_fDashSpeed = 20.f;
	float	m_fStart_fY;

	// Wall
	int		m_iWallJumpCnt = 0;
	int		m_iWallJumpMaxCnt = 10;
	float	m_fWallSpeed = 5.f;
	int		m_iWallJumpTickSpeed = 15;
	bool	m_bWallJump = false;

	// Charge
	bool	m_bSoulCharge = false;
	DWORD	m_dwSoulChargeStartTime;
	DWORD	m_dwSoulChargeEndTime;

	// Fire_Ball
	DWORD	m_dwFireStartTime;

	// Player_State
	STATE	m_ePreState;
	STATE	m_eCurState;

	// Damaged
	bool	m_bDamaged = false;
	DWORD	m_dwDamagedTime;
	int		m_iKnockBackCnt = 0;
	int		m_iKnockBackMaxCnt = 5;
	float	m_fKnockBackSpeed = 5.f;

	// Offset
	float	m_fOldX;
	float	m_fOldY;
	float	m_fLastScrollX;
	float	m_fLastScrollY;
	int		m_iShakeOffset = 20;
	int		m_iShakeX[10];
	int		m_iShakeY[10];

public:
	CPlayer();
	virtual ~CPlayer();

public:
	virtual void Initialize(void)	override;
	virtual int	 Update(void)		override;
	virtual void Late_Update(void)	override;
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;
	virtual void Update_HitBox()	override;
	virtual void Collision_Event(CObj* _OtherObj, float _fColX, float _fColY) override;
	virtual bool	Move_Frame(int _InitFrame = 0) override;

public:
	float		Get_DashSpeed() { return m_fDashSpeed; }
	const bool&	Get_IsCharge() const { return m_bSoulCharge; }

	void		Set_IsJump(bool _bJump) { m_bJump = _bJump; }
	void		Set_CurJumpSpeed(float _fJumpSpeed) { m_fCurJumpSpeed = _fJumpSpeed; }
	void		Set_IsDoubleJump(bool _bDoubleJump) { m_bDoubleJump = _bDoubleJump; }
	void		Set_DoubleJumpCnt(int _iCount) { m_iDoubleJumpCnt = _iCount; }
	void		Set_IncreaseCurSoul() { m_tStat.iSoul += 10; }

	void		HitKnockBack();


private:
	bool		Key_Input(void);
	void		Update_Damaged();
	void		Jumping(void);
	void		Update_Dash();
	void		Offset(void);
	void		Motion_Change(void);
};

