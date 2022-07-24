#include "stdafx.h"
#include "Obj_Mgr.h"
#include "Collision_Mgr.h"

#include "Rectangle.h"

CObj_Mgr*	CObj_Mgr::m_pInstance = nullptr;

CObj_Mgr::CObj_Mgr()
{
}

CObj_Mgr::~CObj_Mgr()
{
	Release();
}

void CObj_Mgr::Add_Object(OBJID eID, CObj * pObj)
{
	if (OBJ_END <= eID || nullptr == pObj)
		return;

	m_ObjList[eID].push_back(pObj);
}

int CObj_Mgr::Update()
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter = m_ObjList[i].begin(); iter != m_ObjList[i].end();)
		{
			int iEvent = (*iter)->Update();

			if (OBJ_DEAD == iEvent)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}
	return 0;
}

void CObj_Mgr::Late_Update()
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		for (auto& Obj : m_ObjList[i])
		{
			Obj->Late_Update();

			if (m_ObjList[i].empty())
				break;
		}
	}

	CCollision_Mgr::Collision_Rect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_RECTANGLE]);
	CCollision_Mgr::Collision_Rect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_SPIKE]);
	CCollision_Mgr::Collision_Rect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_SWORD]);
	CCollision_Mgr::Collision_Rect(m_ObjList[OBJ_EFFECT], m_ObjList[OBJ_SPIKE]);
	CCollision_Mgr::Collision_Rect(m_ObjList[OBJ_SWORD], m_ObjList[OBJ_RECTANGLE]);
	CCollision_Mgr::Collision_Rect(m_ObjList[OBJ_SWORD], m_ObjList[OBJ_MONSTER]);
	CCollision_Mgr::Collision_Rect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_MONSTER]);
	CCollision_Mgr::Collision_Rect(m_ObjList[OBJ_EFFECT], m_ObjList[OBJ_MONSTER]);
}

void CObj_Mgr::Render(HDC hDC)
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		for (auto& Obj : m_ObjList[i])
		{
			Obj->Render(hDC);
		}
	}
}

void CObj_Mgr::Release()
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), Safe_Delete<CObj*>);
		m_ObjList[i].clear();
	}
}

void CObj_Mgr::Delete_OBJID(OBJID eID)
{
	for (auto& iter : m_ObjList[eID])
		Safe_Delete(iter);

	m_ObjList[eID].clear();
}

CObj * CObj_Mgr::Get_Target(OBJID eID, CObj * pObj)
{
	if (m_ObjList[eID].empty())
		return nullptr;

	CObj*	pTarget = nullptr;
	float	fDistance = 0.f;

	for (auto& iter : m_ObjList[eID])
	{
		if (iter->Get_Dead())
			continue;

		float	fWidth = abs(pObj->Get_Info().fX - iter->Get_Info().fX);
		float	fHeight = abs(pObj->Get_Info().fY - iter->Get_Info().fY);
		float	fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

		if ((!pTarget) || (fDistance > fDiagonal))
		{
			pTarget = iter;
			fDistance = fDiagonal;
		}
	}
	return pTarget;
}

void CObj_Mgr::Load_Wall(const TCHAR* _szFileName)
{
	for_each(m_ObjList[OBJ_RECTANGLE].begin(), m_ObjList[OBJ_RECTANGLE].end(), Safe_Delete<CObj*>);
	m_ObjList[OBJ_RECTANGLE].clear();

	HANDLE		hFile = CreateFile(_szFileName,		// 파일의 경로와 이름
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD	dwByte = 0;

	INFO	tInfo{};

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		m_ObjList[OBJ_RECTANGLE].push_back(new CRectangle(tInfo));
	}


	CloseHandle(hFile);
}