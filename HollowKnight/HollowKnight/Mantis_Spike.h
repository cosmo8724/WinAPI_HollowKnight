#pragma once
#include "Obj.h"
class CMantis_Spike : public CObj
{
public:
	CMantis_Spike();
	virtual ~CMantis_Spike();

public:
	virtual void Initialize(void)	override;
	virtual int	 Update(void)		override;
	virtual void Late_Update(void)	override;
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;
	virtual void Update_HitBox()	override;
	virtual void Collision_Event(CObj* _OtherObj, float _fColX, float _fColY) override;
};

