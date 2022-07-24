#pragma once
#include "Obj.h"
class CWall_Jump_Effect : public CObj
{
public:
	CWall_Jump_Effect();
	virtual ~CWall_Jump_Effect();

public:
	virtual void Initialize(void)	override;
	virtual int	 Update(void)		override;
	virtual void Late_Update(void)	override;
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;
};

