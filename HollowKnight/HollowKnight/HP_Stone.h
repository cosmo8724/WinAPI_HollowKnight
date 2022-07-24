#pragma once
#include "Obj.h"
class CHP_Stone : public CObj
{
private:
	int		m_iPlayerCurHP;
	int		m_iPlayerLastHP;
	HPRECT	m_tRectArray[5];

public:
	CHP_Stone();
	virtual ~CHP_Stone();

public:
	virtual void Initialize(void)	override;
	virtual int	 Update(void)		override;
	virtual void Late_Update(void)	override;
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;
	virtual void Update_Rect()		override;
	virtual void Update_HitBox()	override;

private:
	void		 Update_State();
	void		 Motion_Change();
	void		 Move_Array_Frame();
};

