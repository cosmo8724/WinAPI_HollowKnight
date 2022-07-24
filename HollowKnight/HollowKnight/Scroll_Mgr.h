#pragma once
#include "Include.h"

#include "Scene_Mgr.h"

class CScroll_Mgr
{
private:
	static CScroll_Mgr*		m_pInstance;
	float					m_fScrollX;
	float					m_fScrollY;
	SCENEID					m_eCurScene;

private:
	CScroll_Mgr();
	~CScroll_Mgr();

public:
	void	Set_ScrollX(float _fX) { m_fScrollX += _fX; }
	void	Set_ScrollY(float _fY) { m_fScrollY += _fY; }
	void	ResetScroll(float _fX, float _fY)
	{
		m_fScrollX = _fX;
		m_fScrollY = _fY;
	}

	float	Get_ScrollX() { return m_fScrollX; }
	float	Get_ScrollY() { return m_fScrollY; }

public:
	void	Scroll_Lock();
	void	Initialize();

public:
	static CScroll_Mgr*		Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CScroll_Mgr;

		return m_pInstance;
	}
	static void		Destroy_Instance(void)
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
};

