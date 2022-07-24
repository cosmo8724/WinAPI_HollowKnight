#pragma once
#include "Obj.h"
class CFocus_Effect : public CObj
{
private:
	int		m_iLoopFrameStart = 0;

public:
	CFocus_Effect();
	virtual ~CFocus_Effect();

public:
	virtual void Initialize(void)	override;
	virtual int	 Update(void)		override;
	virtual void Late_Update(void)	override;
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;
};

