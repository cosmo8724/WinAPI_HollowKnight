#pragma once
#include "stdafx.h"


class CTimer
{
	enum TIMER_TYPE { LOOP, TICK };

private:
	TIMER_TYPE	m_eType = LOOP;

	float		m_fTime;
	DWORD		m_dwLastTime;

	float		m_fTick;
	DWORD		m_dwTickTime;

public:
	CTimer() {}
	~CTimer() {}

public:
	virtual void InitLoop(float _fTick)
	{
		m_eType = LOOP;
		m_dwTickTime = GetTickCount();
		m_fTick = _fTick;
	}
	virtual void InitTick(float _fTime, float _fTick = 0.f)
	{
		m_eType = TICK;
		m_fTime = _fTime;
		m_fTick = _fTick;
		Reset();
	}
	virtual bool Check()
	{
		if (m_eType == LOOP)
		{
			if (m_dwTickTime + (m_fTick * 1000) < GetTickCount())
			{
				m_dwTickTime = GetTickCount();
				return true;
			}

			return false;
		}
		else
		{
			if (m_dwLastTime > GetTickCount())
			{
				if (m_dwTickTime + (m_fTick * 1000) < GetTickCount())
				{
					m_dwTickTime = GetTickCount();
					return true;
				}
			}
			return false;
		}
	}

	virtual void Reset()
	{
		m_dwTickTime = GetTickCount();
		m_dwLastTime = GetTickCount() + DWORD(m_fTime * 1000);
	}
};

