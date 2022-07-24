#pragma once
#include "Obj.h"
#include "Timer.h"

class CThe_Radiance : public CObj
{
	enum STATE { PRE_IDLE, IDLE, APPEAR_TURN, DISAPPEAR_TURN, WING1, WING2 };

private:
	// State
	bool	m_bStart = false;
	DWORD	m_iStartTime;
	bool	m_bCreateBackEffect = false;
	STATE	m_eCurState;
	STATE	m_ePreState;
	int		m_iLoopFrameStart;
	UNTOUCHABLE	m_eUntouchable = ON;


	CTimer	m_AttackTimer;
	bool	m_bAttack = false;
	bool	m_bAttackReady = false;

	// Vertical Spear
	CTimer	m_VertSpearReadyTimer;
	CTimer	m_VertSpearTimer;
	CTimer	m_VertSpearToTurnTimer;
	bool	m_bVertSpear = false;
	bool	m_bVertSpearReady = false;

	// Horizon Spear
	CTimer	m_HorizSpearReadyTimer;
	CTimer	m_HorizSpearTimer;
	CTimer	m_HorizSpearToTurnTimer;
	bool	m_bHorizSpear = false;
	bool	m_bHorizSpearReady = false;

	// Rotate Spear
	CTimer	m_RotSpearReadyTimer;
	CTimer	m_RotSpearTimer;
	CTimer	m_RotSpearToTurnTimer;
	bool	m_bRotSpear = false;
	bool	m_bRotSpearReady = false;

	// Vertical Lazer
	CTimer	m_VertLazerReadyTimer;
	CTimer	m_VertLazerTimer;
	CTimer	m_VertLazerToTurnTimer;
	bool	m_bVertLazer = false;
	bool	m_bVertLazerReady = false;

	// Rotate Lazer
	CTimer	m_RotLazerReadyTimer;
	CTimer	m_RotLazerTimer;
	CTimer	m_RotLazerToTurnTimer;
	bool	m_bRotLazer = false;
	bool	m_bRotLazerReady = false;

	// Tang Tang Ball
	CTimer	m_TTBReadyTimer;
	CTimer	m_TTBTimer;
	CTimer	m_TTBToTurnTimer;
	bool	m_bTTB = false;
	bool	m_bTTBReady = false;

public:
	CThe_Radiance();
	virtual ~CThe_Radiance();

public:
	virtual void Initialize(void)	override;
	virtual int	 Update(void)		override;
	virtual void Late_Update(void)	override;
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;
	virtual void Update_HitBox()	override;
	virtual void Collision_Event(CObj* _OtherObj, float _fColX, float _fColY) override;

public:
	const UNTOUCHABLE&	Get_Untouchable() const { return m_eUntouchable; }

private:
	void		Motion_Change(void);
	void		Update_State();
	void		Update_Position();
	void		Update_Untuchable();

	void		VertSpear();
	void		HorizSpear();
	void		RotSpear();
	void		VertLazer();
	void		RotLazer();
	void		TangTangBall();
};

