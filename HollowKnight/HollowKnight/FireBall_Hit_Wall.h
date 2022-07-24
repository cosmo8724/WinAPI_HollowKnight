#pragma once
#include "Obj.h"
class CFireBall_Hit_Wall : public CObj
{
public:
	CFireBall_Hit_Wall();
	virtual ~CFireBall_Hit_Wall();

public:
	virtual void Initialize(void)	override;
	virtual int	 Update(void)		override;
	virtual void Late_Update(void)	override;
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;
};

