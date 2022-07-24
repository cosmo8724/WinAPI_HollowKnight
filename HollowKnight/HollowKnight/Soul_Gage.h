#pragma once
#include "Obj.h"
class CSoul_Gage : public CObj
{
private:
	HPRECT	m_tSoulRect;
	int		m_iPlayerCurSoul;
	int		m_iPlayerLastSoul;

public:
	CSoul_Gage();
	virtual ~CSoul_Gage();

public:
	virtual void Initialize(void)	override;
	virtual int	 Update(void)		override;
	virtual void Late_Update(void)	override;
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;
	virtual void Update_HitBox()	override;

private:
	void		 Update_State();
	void		 Motion_Change();
	void		 Move_Soul_Frame();
};