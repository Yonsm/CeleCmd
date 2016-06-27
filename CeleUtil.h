


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CeleUtil 1.6.108
// Copyright (C) Yonsm 2008, All Rights Reserved.
#pragma once
#include "AppDef.h"
#include "UniBase.h"
#include "CeleReg.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// System specification
#define SYS_MajorVer	0x000000FF
#define SYS_MinorVer	0x0000FF00

#define SYS_TypeMask	0x00FF0000
#define SYS_PocketPC	0x00010000
#define SYS_WM5			0x00020000
#define SYS_WM6			0x00040000

#define ST_SP2003		0
#define ST_PPC2003		SYS_PocketPC
#define ST_SP5			SYS_WM5
#define ST_PPC5			(SYS_WM5 | SYS_PocketPC)
#define ST_SP6			(SYS_WM5 | SYS_WM6)
#define ST_PPC6			(SYS_WM5 | SYS_WM6 | SYS_PocketPC)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CeleUtil
class CeleUtil
{
private:
	STATIC DWORD m_dwSysSpec;

private:
	ISTATIC DWORD InitSysSpec()
	{
		OSVERSIONINFO vi;
		vi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&vi);
		DWORD dwSysSpec = MAKEWORD(vi.dwMajorVersion, vi.dwMinorVersion);

		if (vi.dwMajorVersion >= 5)
		{
			dwSysSpec |= SYS_WM5;
			if (vi.dwMinorVersion >= 2)
			{
				dwSysSpec |= SYS_WM6;
			}
		}

		TCHAR tzStr[MAX_NAME];
		SystemParametersInfo(SPI_GETPLATFORMTYPE, MAX_NAME, tzStr, 0);
		if (!UStrCmpI(tzStr, TEXT("PocketPC")))
		{
			dwSysSpec |= SYS_PocketPC;
		}

		return dwSysSpec;
	}

public:
	ISTATIC DWORD GetSysSpec()
	{
		return m_dwSysSpec;
	}

	ISTATIC BYTE GetMajorVer()
	{
		return (BYTE) (GetSysSpec() & SYS_MajorVer);
	}

	ISTATIC BYTE GetMinorVer()
	{
		return (BYTE) ((GetSysSpec() & SYS_MinorVer) >> 8);
	}

	ISTATIC BYTE GetSysType()
	{
		return (BYTE) ((GetSysSpec() & SYS_TypeMask) >> 16);
	}

	ISTATIC BOOL IsWM5()
	{
		return GetSysSpec() & SYS_WM5;
	}

	ISTATIC BOOL IsWM6()
	{
		return GetSysSpec() & SYS_WM6;
	}

	ISTATIC BOOL IsPPC()
	{
		return GetSysSpec() & SYS_PocketPC;
	}

public:
	ISTATIC UINT GetOwnerName(PTSTR ptzName)
	{
		CeleReg reg(TEXT("ControlPanel\\Owner"));
		UINT uRet = reg.GetStr(TEXT("Name"), ptzName);
		if (uRet == 0)
		{
			uRet = reg.GetStr(TEXT("Owner"), ptzName, MAX_NAME, STR_Author);
		}
		return uRet;
	}

	ISTATIC DWORD GetFileAttr(PCTSTR ptzPath)
	{
		WIN32_FILE_ATTRIBUTE_DATA fa;
		fa.dwFileAttributes = 0;
		GetFileAttributesEx(ptzPath, GetFileExInfoStandard, &fa);
		return fa.dwFileAttributes;
	}

	ISTATIC BOOL RemoveFileAttr(PCTSTR ptzPath, DWORD dwAttr = FILE_ATTRIBUTE_READONLY)
	{
		WIN32_FILE_ATTRIBUTE_DATA fa;
		BOOL bResult = GetFileAttributesEx(ptzPath, GetFileExInfoStandard, &fa);
		if (bResult && (fa.dwFileAttributes & dwAttr))
		{
			bResult = SetFileAttributes(ptzPath, fa.dwFileAttributes & ~dwAttr);
		}
		return bResult;
	}

	ISTATIC BOOL ShellOpen(PCTSTR ptzPath, PCTSTR ptzParam = NULL, HWND hWnd = GetActiveWindow(), DWORD dwMask = 0)
	{
		SHELLEXECUTEINFO se ={0};
		se.cbSize = sizeof(se);
		//se.lpVerb = TEXT("Open");
		se.lpFile = ptzPath;
		se.hwnd = hWnd;
		se.lpParameters = ptzParam;
		se.nShow = SW_SHOWNORMAL;
		se.fMask = dwMask;
		BOOL bResult = ShellExecuteEx(&se);
		return (dwMask | SEE_MASK_NOCLOSEPROCESS) ? (BOOL) se.hProcess : bResult;
	}

	ISTATIC BOOL ExecCtrlPanel(INT iIndex, INT iPage = 0)
	{
		TCHAR tzParam[MAX_PATH];
		UStrPrint(tzParam, TEXT("cplmain.cpl, %d, %d"), iIndex, iPage);
		return ShellOpen(TEXT("ctlpnl.exe"), tzParam);
	}

	ISTATIC BOOL SaveBitmap(PCTSTR ptzPath, PBITMAPINFO pmInfo, PVOID pvBits)
	{
		HANDLE hFile = UFileOpen(ptzPath, UFILE_WRITE);
		if (hFile)
		{
			BITMAPFILEHEADER bf;
			UINT nInfoSize = sizeof(pmInfo->bmiHeader) + pmInfo->bmiHeader.biClrUsed * sizeof(pmInfo->bmiColors);
			bf.bfType = 'MB';
			bf.bfOffBits = sizeof(bf) + nInfoSize;
			bf.bfSize = bf.bfOffBits + pmInfo->bmiHeader.biSizeImage;
			bf.bfReserved1 = 0;
			bf.bfReserved2 = 0;
			UFileWrite(hFile, &bf, sizeof(bf));
			UFileWrite(hFile, pmInfo, bf.bfOffBits - sizeof(bf));
			UFileWrite(hFile, pvBits, pmInfo->bmiHeader.biSizeImage);
			UFileClose(hFile);
			return TRUE;
		}
		return FALSE;
	}
};

__declspec(selectany) DWORD CeleUtil::m_dwSysSpec = CeleUtil::InitSysSpec();
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
