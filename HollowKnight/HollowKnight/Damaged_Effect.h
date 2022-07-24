#pragma once
#include "Obj.h"
class CDamaged_Effect :	public CObj
{
public:
	CDamaged_Effect();
	virtual ~CDamaged_Effect();

public:
	virtual void Initialize(void)	override;
	virtual int	 Update(void)		override;
	virtual void Late_Update(void)	override;
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;
	virtual void Update_HitBox()	override;
};

