#pragma once
#include "Obj.h"

class CObj_Mgr
{
private:
	static	CObj_Mgr*	m_pInstance;
	list<CObj*>			m_ObjList[OBJ_END];

private:
	CObj_Mgr();
	~CObj_Mgr();

public:
	void	Add_Object(OBJID eID, CObj* pObj);
	int		Update();
	void	Late_Update();
	void	Render(HDC hDC);
	void	Release();
	void	Delete_OBJID(OBJID eID);
	void	Load_Wall(const TCHAR* _szFileName);

public:
	CObj*			Get_Player() { return m_ObjList[OBJ_PLAYER].front(); }
	CObj*			Get_Target(OBJID eID, CObj* pObj);
	list<CObj*>*	Get_ObjList(OBJID eID) { return &m_ObjList[eID]; }

public:
	static CObj_Mgr*	Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CObj_Mgr;

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

