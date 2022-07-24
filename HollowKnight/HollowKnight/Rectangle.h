#pragma once
#include "Obj.h"
class CRectangle : public CObj
{
public:
	CRectangle();
	CRectangle(INFO _tInfo);
	virtual ~CRectangle();

public:
	virtual void Initialize(void)	override;
	virtual int	 Update(void)		override;
	virtual void Late_Update(void)	override;
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;
	virtual void Update_HitBox()	override;
	virtual void Collision_Event(CObj* _OtherObj, float _fColX, float _fColY) override;

public:
};