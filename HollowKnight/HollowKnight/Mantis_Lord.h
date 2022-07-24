#pragma once
#include "Obj.h"
#include "Timer.h"

class CMantis_Lord : public CObj
{
	enum STATE { SIT, SIT_TO_STAND, STAND_TO_IDLE, IDLE_TO_SIT, IDLE, DASH_READY, DASH, DASH_TO_IDLE, SLASH_READY, SLASH, SLASH_TO_IDLE, FALL_READY, FALL, FALL_TO_IDLE, DEATH,  };

private:
	RECT		m_tCurRect;
	IDENTIFY	m_eIdentify;
	int			m_iLoopFrameStart = 0;
	bool		m_bLineCol;
	DWORD		m_dwStartTimer;
	bool		m_bStart = false;
	DWORD		m_dwStandToIdleTimer;
	CTimer		m_AttackTimer;
	bool		m_bAttack = false;

	// Load Player Pos
	bool		m_bCheckPlayerPos = true;
	float		m_fPlayerX;

	// State
	STATE		m_ePreState;
	STATE		m_eCurState;
	UNTOUCHABLE	m_eUntouchable = ON;

	// Dash
	CTimer		m_DashReadyTimer;
	CTimer		m_DashTimer;
	CTimer		m_DashToIdleTimer;
	bool		m_bDash = false;
	bool		m_bDashReady = false;
	float		m_fDashSpeed = 70.f;

	// Slash
	CTimer		m_SlashReadyTimer;
	CTimer		m_SlashTimer;
	CTimer		m_SlashToIdleTimer;
	bool		m_bSlash = false;
	bool		m_bSlashReady = false;
	float		m_fSlashSpeed = 70.f;

	// Fall
	CTimer		m_FallReadyTimer;
	CTimer		m_FallTimer;
	CTimer		m_FallToIdleTimer;
	bool		m_bFall = false;
	bool		m_bFallReady = false;
	float		m_fFallSpeed = 70.f;

public:
	CMantis_Lord();
	virtual ~CMantis_Lord();

public:
	const UNTOUCHABLE&	Get_Untouchable() const { return m_eUntouchable; }

	void				Set_Identify(IDENTIFY _Identify) { m_eIdentify = _Identify; }

public:
	virtual void Initialize(void)	override;
	virtual int	 Update(void)		override;
	virtual void Late_Update(void)	override;
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;
	virtual void Update_HitBox()	override;
	virtual void Collision_Event(CObj* _OtherObj, float _fColX, float _fColY) override;

private:
	void		Motion_Change(void);
	void		Attack();
	void		Update_State();
	void		Dash();
	void		Slash();
	void		Fall();
	void		Update_Position();
	void		Update_Untuchable();
	void		StateSound();
};

