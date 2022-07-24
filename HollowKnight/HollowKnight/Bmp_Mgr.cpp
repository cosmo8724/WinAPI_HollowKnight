#include "stdafx.h"
#include "Bmp_Mgr.h"

CBmp_Mgr*	CBmp_Mgr::m_pInstance = nullptr;

CBmp_Mgr::CBmp_Mgr()
{
}


CBmp_Mgr::~CBmp_Mgr()
{
	Release();
}

void CBmp_Mgr::Insert_Bmp(const TCHAR * _pFilePath, const TCHAR * _pImageKey)
{
	auto	iter = find_if(m_mapBit.begin(), m_mapBit.end(), CTag_Finder(_pImageKey));

	if (iter == m_mapBit.end())
	{
		CBitMap*	pBmp = new CBitMap;
		pBmp->Load_Bmp(_pFilePath);
		m_mapBit.insert({ _pImageKey, pBmp });
	}
}

HDC CBmp_Mgr::Find_Img(const TCHAR * _pImageKey)
{
	auto	iter = find_if(m_mapBit.begin(), m_mapBit.end(), CTag_Finder(_pImageKey));

	if (iter == m_mapBit.end())
		return nullptr;

	return iter->second->Get_MemDC();
}

void CBmp_Mgr::Release()
{
	for_each(m_mapBit.begin(), m_mapBit.end(), CDeleteMap());
	m_mapBit.clear();
}
