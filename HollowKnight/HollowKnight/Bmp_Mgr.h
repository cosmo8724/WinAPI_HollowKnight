#pragma once
#include "BitMap.h"

class CBmp_Mgr
{
private:
	static CBmp_Mgr*					m_pInstance;
	map<const TCHAR*, CBitMap*>		m_mapBit;

private:
	CBmp_Mgr();
	~CBmp_Mgr();

public:
	void	Insert_Bmp(const TCHAR* _pFilePath, const TCHAR* _pImageKey);
	HDC		Find_Img(const TCHAR* _pImageKey);
	void	Release();

public:
	static CBmp_Mgr*		Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CBmp_Mgr;

		return m_pInstance;
	}
	static void				Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
};

