


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CeleReg 1.6.102
// Copyright (C) Yonsm 2008, All Rights Reserved.
#pragma once
#include <Windows.h>

#ifdef WINCE
#define _RegDeleteKey RegDeleteKey
#else
#include <ShLwApi.h>
#pragma comment(lib,"ShLwApi.lib")
#define _RegDeleteKey SHDeleteKey
#endif

#ifdef STR_AppName
#define REG_AppKey TEXT("Software\\") STR_AppName
#else
#define REG_AppKey TEXT("Software\\CeleWare")
#endif

#define REG_AppSubKey(x) REG_AppKey TEXT("\\") TEXT(x)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CeleReg
class CeleReg
{
protected:
	HKEY m_hKey;

public:
	CeleReg(PCTSTR ptzKey = REG_AppKey, HKEY hKey = HKEY_CURRENT_USER, REGSAM regSam = KEY_ALL_ACCESS)
	{
		m_hKey = NULL;
		RegCreateKeyEx(hKey, ptzKey, 0, NULL, 0, regSam, NULL, &m_hKey, NULL);
	}

	CeleReg(HKEY hKey, PCTSTR ptzKey, REGSAM regSam = KEY_READ)
	{
		m_hKey = NULL;
		RegOpenKeyEx(hKey, ptzKey, 0, regSam, &m_hKey);
	}

	~CeleReg()
	{
		if (m_hKey)
		{
			RegCloseKey(m_hKey);
		}
	}

	operator HKEY()
	{
		return m_hKey;
	}

public:
	INT GetInt(PCTSTR ptzName, INT iDef = 0)
	{
		UINT uSize = sizeof(INT);
		GetVal(ptzName, &iDef, &uSize);
		return iDef;
	}

	HRESULT SetInt(PCTSTR ptzName, INT iVal = 0)
	{
		return SetVal(ptzName, &iVal, sizeof(INT), REG_DWORD);
	}

	UINT GetStr(PCTSTR ptzName, PTSTR ptzStr, UINT uLen = MAX_PATH, PCTSTR ptzDef = NULL)
	{
		ptzStr[0] = 0;
		uLen = uLen * sizeof(TCHAR);
		if (GetVal(ptzName, ptzStr, &uLen) == S_OK)
		{
			return (uLen / sizeof(TCHAR)) - 1;
		}
		else if (ptzDef)
		{
			PCTSTR p = ptzDef;
			while (*ptzStr++ = *p++);
			return (UINT) (p - ptzDef);
		}
		else
		{
			return 0;
		}
	}

	HRESULT SetStr(PCTSTR ptzName, PCTSTR ptzStr = TEXT(""))
	{
		UINT uLen = 1;
		while (ptzStr[uLen++]);
		return SetVal(ptzName, ptzStr, uLen * sizeof(TCHAR), REG_SZ);
	}

	HRESULT GetVal(PCTSTR ptzName, PVOID pvData, PUINT puSize, PDWORD pdwType = NULL)
	{
		return RegQueryValueEx(m_hKey, ptzName, NULL, pdwType, (PBYTE) pvData, (PDWORD) puSize);
	}

	HRESULT SetVal(PCTSTR ptzName, PCVOID pvData, UINT uSize, DWORD dwType = REG_BINARY)
	{
		return RegSetValueEx(m_hKey, ptzName, 0, dwType, (PBYTE) pvData, uSize);
	}

public:
	HRESULT GetInfo(PDWORD pcValues = NULL, PDWORD pcSubKeys = NULL, PDWORD pcbMaxValueName = NULL, PDWORD pcbMaxValue = NULL, PDWORD pcbMaxSubKey = NULL)
	{
		return RegQueryInfoKey(m_hKey, NULL, NULL, NULL, pcSubKeys, pcbMaxSubKey, NULL, pcValues, pcbMaxValueName, pcbMaxValue, NULL, NULL);
	}

	HRESULT DelVal(PCTSTR ptzName)
	{
		return RegDeleteValue(m_hKey, ptzName);
	}

	static HRESULT DelKey(PCTSTR ptzKey = REG_AppKey, HKEY hKey = HKEY_CURRENT_USER)
	{
		return _RegDeleteKey(hKey, ptzKey);
	}

	HRESULT EnumVal(DWORD dwIndex, PTSTR ptzName, PBYTE pbVal, PDWORD pdwSize, PDWORD pdwType = NULL)
	{
		DWORD dwSize = MAX_PATH;
		return RegEnumValue(m_hKey, dwIndex, ptzName, &dwSize, NULL, pdwType, pbVal, pdwSize);
	}

	HRESULT EnumKey(DWORD dwIndex, PTSTR ptzKeyName)
	{
		FILETIME ftTime;
		DWORD dwSize = MAX_PATH;
		return RegEnumKeyEx(m_hKey, dwIndex, ptzKeyName, &dwSize, NULL, NULL, 0, &ftTime);
	}

	HRESULT ExistVal(PCTSTR ptzName)
	{
		UINT uSize;
		return GetVal(ptzName, NULL, &uSize);
	}

	static HRESULT ExistKey(PCTSTR ptzKey = REG_AppKey, HKEY hKey = HKEY_CURRENT_USER)
	{
		HRESULT hResult = RegOpenKeyEx(hKey, ptzKey, 0, KEY_READ, &hKey);
		if (hResult)
		{
			RegCloseKey(hKey);
		}
		return hResult;
	}

	UINT GetSize(PCTSTR ptzName)
	{
		UINT uSize = 0;
		GetVal(ptzName, NULL, &uSize);
		return uSize;
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
