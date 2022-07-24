#pragma once
#include "Line.h"

class CLine_Mgr
{
private:
	static CLine_Mgr*	m_pInstance;
	LINEPOINT			m_tLinePoint[LINE_END];
	list<CLine*>		m_LineList;

private:
	CLine_Mgr();
	~CLine_Mgr();

public:
	bool	Collision_Line(float& fX, float* pY);
	void	Save_Line();
	void	Load_Line();

public:
	void	Initialize();
	int		Update();
	void	Late_Update();
	void	Render(HDC hDC);
	void	Release();

public:
	static CLine_Mgr*	Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CLine_Mgr;

		return m_pInstance;
	}
	static void			Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
};

