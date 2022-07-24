#pragma once

template<typename T>
void	Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}

template<typename T>
void	Safe_Delete_Array(T& Temp)
{
	if (Temp)
	{
		delete[] Temp;
		Temp = nullptr;
	}
}

class CDeleteObj
{
public:
	template<typename T>
	void	operator()(T& Temp)
	{
		if (Temp)
		{
			delete Temp;
			Temp = nullptr;
		}
	}
};

class CDeleteMap
{
public:
	template<typename T>
	void	operator()(T& Pair)
	{
		if (Pair.second)
		{
			delete Pair.second;
			Pair.second = nullptr;
		}
	}
};

class CTag_Finder
{
private:
	const TCHAR*	m_pKey;

public:
	CTag_Finder(const TCHAR* pKey)
		:m_pKey(pKey)
	{
	}

public:
	template<typename T>
	bool	operator()(T& Pair)
	{
		if (!lstrcmp(m_pKey, Pair.first))
			return true;
		return false;
	}
};