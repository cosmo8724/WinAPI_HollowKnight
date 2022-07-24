#pragma once
#include "Obj.h"
class CMyPointer : public CObj
{
public:
	CMyPointer();
	virtual ~CMyPointer();

public:
	void	Set_Motion(int _iMotion) { m_tFrame.iMotion = _iMotion; }
	void	Set_FrameStart(int _iFrameStart) { m_tFrame.iFrameStart = _iFrameStart; }

public:
	virtual void Initialize(void)	override;
	virtual int	 Update(void)		override;
	virtual void Late_Update(void)	override;
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;
};

