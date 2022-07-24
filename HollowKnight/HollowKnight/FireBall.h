#pragma once
#include "Obj.h"
class CFireBall : public CObj
{
	enum STATE { IDLE, DEAD };
private:
	bool	m_bHit = false;
	int		m_iLoopFrameStart;

	STATE	m_eCurState;
	STATE	m_ePreState;

public:
	CFireBall();
	virtual ~CFireBall();

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
};

