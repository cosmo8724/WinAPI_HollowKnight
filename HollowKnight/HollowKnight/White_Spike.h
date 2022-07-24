#pragma once
#include "Obj.h"
class CWhite_Spike : public CObj
{
public:
	CWhite_Spike();
	virtual ~CWhite_Spike();

public:
	virtual void Initialize(void)	override;
	virtual int	 Update(void)		override;
	virtual void Late_Update(void)	override;
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;
	virtual void Update_HitBox()	override;
};

