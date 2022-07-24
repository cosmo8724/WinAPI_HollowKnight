#pragma once

#include "Logo.h"
#include "MyMenu.h"
#include "Start_Stage.h"
#include "First_Stage.h"
#include "Stage.h"
#include "The_Radiance_Stage.h"

class CScene_Mgr
{
private:
	static CScene_Mgr*	m_pInstance;
	CScene*				m_pScene;
	SCENEID				m_ePreScene;
	SCENEID				m_eCurScene;

private:
	CScene_Mgr();
	~CScene_Mgr();

public:
	void	Scene_Change(SCENEID eID);
	void	Update();
	void	Late_Update();
	void	Render(HDC hDC);
	void	Release();

	SCENEID		Get_SceneID() { return m_eCurScene; }

public:
	static CScene_Mgr*	Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CScene_Mgr;

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

