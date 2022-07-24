#include "stdafx.h"
#include "Scene_Mgr.h"

CScene_Mgr*		CScene_Mgr::m_pInstance = nullptr;

CScene_Mgr::CScene_Mgr()
	: m_pScene(nullptr), m_ePreScene(SC_END), m_eCurScene(SC_START_STAGE)
{
}


CScene_Mgr::~CScene_Mgr()
{
	Release();
}

void CScene_Mgr::Scene_Change(SCENEID eID)
{
	m_eCurScene = eID;

	if (m_ePreScene != m_eCurScene)
	{
		Safe_Delete(m_pScene);

		switch (m_eCurScene)
		{
		case SC_LOGO:
			m_pScene = new CLogo;
			break;

		case SC_MENU:
			m_pScene = new CMyMenu;
			break;

		case SC_EDIT:
			break;

		case SC_START_STAGE:
			m_pScene = new CStart_Stage;
			break;

		case SC_FIRST_STAGE:
			m_pScene = new CFirst_Stage;
			break;

		case SC_MANTIS_LORDS_STAGE:
			m_pScene = new CStage;
			break;

		case SC_THE_RADIANCE_STAGE:
			m_pScene = new CThe_Radiance_Stage;
			break;
		}
		m_pScene->Initialize();
		m_ePreScene = m_eCurScene;
	}
}

void CScene_Mgr::Update()
{
	if (m_pScene)
		m_pScene->Update();
}

void CScene_Mgr::Late_Update()
{
	if (m_pScene)
		m_pScene->Late_Update();
}

void CScene_Mgr::Render(HDC hDC)
{
	if (m_pScene)
		m_pScene->Render(hDC);
}

void CScene_Mgr::Release()
{
	Safe_Delete<CScene*>(m_pScene);
}
