

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header
#include "Main.h"

#define CC_SEP 1
#define CC_LEN 4

#if (_WIN32_WCE < 0x500)
#include "ZLib/TGZX.cpp"
#endif

enum CMD
{
	CMD_LOAD, CMD_BATC, CMD_IFEX, CMD_ELSE,
	CMD_CALL, CMD_GOTO, CMD_PROC, CMD_ENDP,
	CMD_PLAY, CMD_BEEP, CMD_MSGX, CMD_DLGX,
	CMD_LINK, CMD_FILE, CMD_REGX, CMD_RARX,
	CMD_SEND, CMD_WAIT, CMD_KILL, CMD_SHUT,
	CMD_EXEC, CMD_CDLL, CMD_EVAL, CMD_ASOC,
	CMD_DISP, CMD_POST, CMD_HELP, CMD_CCUI,
#ifdef ZLIB_H
	CMD_TGZX,
#endif
};

const TCHAR c_tzCmd[][CC_LEN + 1] =
{
	TEXT("LOAD"), TEXT("BATC"), TEXT("IFEX"), TEXT("ELSE"),
	TEXT("CALL"), TEXT("GOTO"), TEXT("PROC"), TEXT("ENDP"),
	TEXT("PLAY"), TEXT("BEEP"), TEXT("MSGX"), TEXT("DLGX"),
	TEXT("LINK"), TEXT("FILE"), TEXT("REGX"), TEXT("RARX"),
	TEXT("SEND"), TEXT("WAIT"), TEXT("KILL"), TEXT("SHUT"),
	TEXT("EXEC"), TEXT("CDLL"), TEXT("EVAL"), TEXT("ASOC"),
	TEXT("DISP"), TEXT("POST"), TEXT("HELP"), TEXT("CCUI"),
#ifdef ZLIB_H
	TEXT("TGZX"), 
#endif

};

typedef struct _CXT
{
	PCTSTR ptzCur;
	PCTSTR ptzNext;
	PCTSTR ptzFile;
	HRESULT hXVar;
	BOOL bElse;
}
CXT, *PCXT;

TCHAR g_tzXVar[10][MAX_PATH] = {0};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get Registry root
HKEY GetRootKey(PTSTR& ptzKey)
{
	STATIC struct {HKEY hKey; TCHAR tzKey[20];} c_sRegRoot[] =
	{
		{HKEY_USERS, TEXT("HKU\\")},
		{HKEY_CURRENT_USER, TEXT("HKCU\\")},
		{HKEY_CLASSES_ROOT, TEXT("HKCR\\")},
		{HKEY_LOCAL_MACHINE, TEXT("HKLM\\")},
		{HKEY_USERS, TEXT("HKEY_USERS\\")},
		{HKEY_CURRENT_USER, TEXT("HKEY_CURRENT_USER\\")},
		{HKEY_CLASSES_ROOT, TEXT("HKEY_CLASSES_ROOT\\")},
		{HKEY_LOCAL_MACHINE, TEXT("HKEY_LOCAL_MACHINE\\")},
	};

	for (UINT i = 0; i < _NumOf(c_sRegRoot); i++)
	{
		UINT n = UStrEqual(c_sRegRoot[i].tzKey, ptzKey);
		if (c_sRegRoot[i].tzKey[n] == 0)
		{
			ptzKey += n;
			return c_sRegRoot[i].hKey;
		}
	}
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Import Windows Registry Script
#ifndef REG_MUI_SZ
#define REG_MUI_SZ 21
#endif
#define _SkipBlank(p) while ((*p == '\r') || (*p == '\n') || (*p == '\t') || (*p == ' ')) p++;
HRESULT Import(PTSTR ptzScript, PCTSTR* pptzStop = NULL)
{
	HKEY hKey = NULL;
	_SkipBlank(ptzScript);
	HRESULT hResult = S_OK;
	PTSTR ptzEnd = ptzScript;
	PTSTR ptzStart = ptzScript;
	while (*ptzScript)
	{
		switch (*ptzScript)
		{
		case '"':
			if (*ptzStart != '[')
			{
				ptzScript++;
				*ptzEnd++ = CC_SEP;
				continue;
			}
			break;

		case '\\':
			if (*ptzStart != '[')
			{
				ptzScript++;
				_SkipBlank(ptzScript);
				*ptzEnd++ = *ptzScript++;
				continue;
			}
			break;

		case '\r':
		case '\n':
			*ptzEnd = 0;
			if (*ptzStart == '[')
			{
				if (hKey)
				{
					RegCloseKey(hKey);
					hKey = NULL;
				}
				ptzScript[-1] = 0;
				if (*++ptzStart == '-')
				{
					HKEY hRoot = GetRootKey(++ptzStart);
					if (hRoot)
					{
						CeleReg::DelKey(ptzStart, hRoot);
					}
				}
				else
				{
					HKEY hRoot = GetRootKey(ptzStart);
					if (hRoot)
					{
						RegCreateKeyEx(hRoot, ptzStart, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, NULL);
					}
				}
			}
			else if (hKey && ((*ptzStart == '@') || (*ptzStart == CC_SEP)))
			{
				PTSTR ptzVal;
				if (*ptzStart++ == '@')
				{
					ptzVal = UStrChr(ptzStart, '=');
					ptzStart = NULL;
				}
				else if (ptzVal = UStrChr(ptzStart, CC_SEP))
				{
					*ptzVal++ = 0;
					ptzVal = UStrChr(ptzVal, '=');
				}

				if (ptzVal)
				{
					PTSTR ptzTemp;
					if (ptzTemp = UStrStrI(ptzVal, TEXT("DWORD:")))
					{
						ptzVal = ptzTemp + 4;
						ptzVal[0] = '0';
						ptzVal[1] = 'x';
						DWORD dwData = UStrToInt(ptzVal);
						RegSetValueEx(hKey, ptzStart, 0, REG_DWORD, (PBYTE) &dwData, sizeof(DWORD));
					}
					else if (ptzTemp = UStrStrI(ptzVal, TEXT("HEX")))
					{
						DWORD dwType = REG_BINARY;
						if (ptzTemp[3] == '(')
						{
							ptzTemp[2] = '0';
							ptzTemp[3] = 'x';
							dwType = UStrToInt(ptzTemp + 2);
						}

						if (ptzVal = UStrChr(ptzTemp, ':'))
						{
							ptzVal++;
							UINT i = 0;
							PBYTE pbVal = (PBYTE) ptzVal;
							while (*ptzVal)
							{
								BYTE bVal = UChrToHex(ptzVal);
								while (*ptzVal && (*ptzVal++ != ','));
								pbVal[i++] = bVal;
							}
							RegSetValueEx(hKey, ptzStart, 0, dwType, pbVal, i);
						}
					}
					else if (ptzTemp = UStrStrI(ptzVal, TEXT("MULTI_SZ:")))
					{
						if (ptzVal = UStrChr(ptzTemp, CC_SEP))
						{
							ptzEnd = ++ptzVal;
							ptzTemp = ptzVal;
							while (*ptzTemp)
							{
								if (*ptzTemp == CC_SEP)
								{
									ptzTemp++;
									*ptzEnd++ = 0;
									while (*ptzTemp && (*ptzTemp++ != CC_SEP));
								}
								else
								{
									*ptzEnd++ = *ptzTemp++;
								}
							}
							*ptzEnd++ = 0;
							RegSetValueEx(hKey, ptzStart, 0, REG_MULTI_SZ, (PBYTE) ptzVal, (DWORD) (ptzEnd - ptzVal) * sizeof(TCHAR));
						}
					}
					else
					{
						DWORD dwType = UStrStrI(ptzVal, TEXT("MUI_SZ:")) ? REG_MUI_SZ : REG_SZ;
						if (ptzTemp = UStrChr(ptzVal, CC_SEP))
						{
							ptzVal = ptzTemp + 1;
							if (ptzTemp = UStrChr(ptzVal, CC_SEP))
							{
								*ptzTemp++ = 0;
								RegSetValueEx(hKey, ptzStart, 0, dwType, (PBYTE) ptzVal, (DWORD) (ptzTemp - ptzVal) * sizeof(TCHAR));
							}
						}
						else if (ptzVal[1] == '-')
						{
							RegDeleteValue(hKey, ptzStart);
						}
					}
				}
			}
			else if (*ptzStart != ';')
			{
				hResult = ERROR_NOT_REGISTRY_FILE;
				if (pptzStop)
				{
					*ptzScript = '\n';
					ptzScript = ptzStart;
					goto _Stop;
				}
			}
			ptzScript++;
			_SkipBlank(ptzScript);
			ptzStart = ptzEnd = ptzScript;
			continue;
		}

		*ptzEnd++ = *ptzScript++;
	}

_Stop:
	if (pptzStop)
	{
		*pptzStop = ptzScript;
	}
	if (hKey)
	{
		RegCloseKey(hKey);
	}
	return hResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOAD script file
#include <CfgMgrApi.h>
#define CC_REG TEXT("REGEDIT4")
#define CC_REX TEXT("Windows Registry Editor Version 5.00")
HRESULT LOAD(PCTSTR ptzPath)
{
	// LOAD file
	CONST PTSTR ptzFile = UFileToStr(ptzPath);
	if (ptzFile == NULL)
	{
		return ERROR_OPEN_FAILED;
	}

	// Skip UNICODE BOM and white space
	PTSTR ptzScript = ptzFile;
	while ((*ptzScript == '\r') || (*ptzScript == '\n') || (*ptzScript == '\t') || (*ptzScript == ' ') || ((UTCHAR) *ptzScript >= 0x7F))
	{
		ptzScript++;
	}

	// Select process engine
	HRESULT hResult;
	if (*ptzScript == '<')
	{
		PTSTR ptzOut = NULL;
		hResult = DMProcessConfigXML(ptzScript, CFGFLAG_PROCESS, &ptzOut);
		if (ptzOut)
		{
			delete[] ptzOut;
		}
	}
	else if (UStrEqual(ptzScript, CC_REG) >= _NumOf(CC_REG) - 1)
	{
		hResult = Import(ptzScript + _NumOf(CC_REG));
	}
	else if (UStrEqual(ptzScript, CC_REX) >= _NumOf(CC_REX) - 1)
	{
		hResult = Import(ptzScript + _NumOf(CC_REX));
	}
	else
	{
		hResult = Process(ptzScript, ptzPath);
	}

	UMemFree(ptzFile);
	return hResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BATch Command
HRESULT Dispatch(PTSTR ptzCmd, CXT& XT);
HRESULT BATC(PTSTR ptzCmd, CXT& XT)
{
	for (PTSTR p = ptzCmd; *p; p++)
	{
		if (*p == ';')
		{
			*p = 0;
			XT.hXVar = Dispatch(ptzCmd, XT);
			ptzCmd = p + 1;
		}
	}
	return Dispatch(ptzCmd, XT);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IF condition is true, EXecute command
HRESULT IFEX(PTSTR ptzCmd)
{
	BOOL bNot = (*ptzCmd == '!');
	if (bNot)
	{
		ptzCmd++;
	}

	BOOL bResult;
	PTSTR p = ptzCmd;
	for (INT iVal = UStrToInt(ptzCmd); *p; p++)
	{
		switch (*p)
		{
		case '=':
			if (p[1] == '=')
			{
				bResult = (iVal == UStrToInt(p + 2));
			}
			else
			{
				*p++ = 0;
				bResult = (UStrCmpI(ptzCmd, p) == 0);
			}
			break;

		case '!':
			if (p[1] == '=')
			{
				bResult = (iVal != UStrToInt(p + 2));
			}
			else
			{
				*p++ = 0;
				bResult = (UStrCmpI(ptzCmd, p) != 0);
			}
			break;

		case '>':
			if (p[1] == '=')
			{
				bResult = (iVal >= UStrToInt(p + 2));
			}
			else
			{
				bResult = (iVal > UStrToInt(p + 1));
			}
			break;

		case '<':
			if (p[1] == '=')
			{
				bResult = (iVal <= UStrToInt(p + 2));
			}
			else
			{
				bResult = (iVal < UStrToInt(p + 1));
			}
			break;

		case '&':
			if (p[1] == '&')
			{
				bResult = (iVal && UStrToInt(p + 2));
			}
			else
			{
				bResult = (iVal & UStrToInt(p + 1));
			}
			break;

		case '|':
			if (p[1] == '|')
			{
				bResult = (iVal || UStrToInt(p + 2));
			}
			else
			{
				bResult = (iVal | UStrToInt(p + 1));
			}
			break;

		default:
			continue;
		}
		break;
	}

	if (!*p)
	{
		bResult = (GetFileAttributes(ptzCmd) != -1);
	}

	return (bResult && !bNot) || (!bResult && bNot);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EVALuate variable
HRESULT EVAL(PTSTR ptzCmd)
{
	if (UChrIsNum(*ptzCmd))
	{
		UINT i = (*ptzCmd++ - '0');
		if (*ptzCmd++ == '=')
		{
			UStrCopy(g_tzXVar[i], ptzCmd);
			return S_OK;
		}
		else
		{
			return UStrToInt(g_tzXVar[i]);
		}
	}
	else if ((*ptzCmd == 'x') || (*ptzCmd == 'X'))
	{
		return UStrToInt(ptzCmd + 1 + (ptzCmd[1] == '='));
	}
	else
	{
		_Zero(g_tzXVar);
		return S_OK;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PLAY sound
#include <MMSystem.h>
HRESULT PLAY(PTSTR ptzCmd)
{
	DWORD dwFlags = SND_FILENAME;
	while (TRUE)
	{
		if (*ptzCmd == '!') dwFlags |= SND_ASYNC;
		else if (*ptzCmd == '$') dwFlags |= SND_ALIAS;
		else if (*ptzCmd == '*') dwFlags |= SND_LOOP;
		else break;
		ptzCmd++;
	}
	return !PlaySound(ptzCmd[0] ? ptzCmd : NULL, NULL, dwFlags);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MesSaGe boX
HRESULT MSGX(PTSTR ptzCmd)
{
	UINT uType = MB_ICONINFORMATION;
	PTSTR ptzCaption = UStrChr(ptzCmd, CC_SEP);
	if (ptzCaption)
	{
		*ptzCaption++ = 0;
		PTSTR ptzType = UStrChr(ptzCaption, CC_SEP);
		if (ptzType)
		{
			*ptzType++ = 0;
			uType = UStrToInt(ptzType);
		}
	}
	else
	{
		ptzCaption = STR_AppName;
	}
	return MessageBox(g_hWnd, ptzCmd, ptzCaption, uType | MB_SETFOREGROUND);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DiaLoG boX
#define CC_CTL 4531
#define CC_CHECK (WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX)
#define CC_RADIO (WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTORADIOBUTTON)
#define CC_GROUP (CC_RADIO | WS_GROUP)
HRESULT REGX(PTSTR ptzCmd);
INT_PTR CALLBACK DLGX(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	STATIC UINT s_uCount = 0;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		s_uCount = 0;
		if (lParam)
		{
			PTSTR p = (PTSTR) lParam;
			CeleCtrl::InitDlgBar(hWnd, IDR_DlgMenu, TRUE, (*p == '#') ? (p++, TBSTATE_HIDDEN) : TBSTATE_ENABLED);

			RECT rtWnd;
			GetClientRect(hWnd, &rtWnd);
			rtWnd.left = GetSystemMetrics(SM_CXSMICON) / 2;
			rtWnd.right -= rtWnd.left * 2;
			rtWnd.bottom = GetSystemMetrics(SM_CYSMICON) + 5;

			DWORD dwGroupStart = CC_CTL;
			for (PTSTR q = p; s_uCount < 32; q++)
			{
				TCHAR t = *q;
				if ((t == 0) || (t == CC_SEP))
				{
					*q = 0;
					switch (*p)
					{
					case '@':
						if (PTSTR ptzReg = UStrChr(p, '|'))
						{
							*ptzReg++ = 0;
							HWND hCtrl = CreateWindow(TEXT("BUTTON"), p + 1, CC_CHECK,
								rtWnd.left, (5 + s_uCount * rtWnd.bottom), rtWnd.right, rtWnd.bottom - 1, hWnd, (HMENU) (CC_CTL + s_uCount), g_hInst, 0);
							SetWindowLong(hCtrl, GWL_USERDATA, (LONG) ptzReg);
							if (REGX(ptzReg))
							{
								CheckDlgButton(hWnd, (CC_CTL + s_uCount), BST_CHECKED);
							}
						}
						break;

					case '{':
						dwGroupStart = (CC_CTL + s_uCount);

					case '[':
					case '<':
						CreateWindow(TEXT("BUTTON"), p + 2 + (p[1] - 2 != p[0]), ((*p == '{') ? CC_GROUP : ((*p == '<') ? CC_RADIO : CC_CHECK)),
							rtWnd.left, (5 + s_uCount * rtWnd.bottom), rtWnd.right, rtWnd.bottom - 1, hWnd, (HMENU) (CC_CTL + s_uCount), g_hInst, 0);
						if ((p[1] - 2 != p[0]) && (p[1] != '0'))
						{
							if (*p == '[')
							{
								CheckDlgButton(hWnd, (CC_CTL + s_uCount), BST_CHECKED);
							}
							else
							{
								CheckRadioButton(hWnd, dwGroupStart, (CC_CTL + s_uCount), (CC_CTL + s_uCount));
							}
						}
						break;

					case '$':
						CreateWindow(TEXT("STATIC"), p + 1, WS_CHILD | WS_VISIBLE | SS_LEFT,
							rtWnd.left, (5 + s_uCount * rtWnd.bottom), rtWnd.right, rtWnd.bottom - 1, hWnd, (HMENU) (CC_CTL + s_uCount), g_hInst, 0);
						break;

					default:
						CreateWindow(TEXT("EDIT"), p, WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | WS_BORDER,
							rtWnd.left, (5 + s_uCount * rtWnd.bottom), rtWnd.right, rtWnd.bottom - 1, hWnd, (HMENU) (CC_CTL + s_uCount), g_hInst, 0);
						break;
					}

					s_uCount++;
					p = q + 1;
					if (t == 0)
					{
						break;
					}
				}
			}
		}
		break;
		//return TRUE;

	case  WM_SIZE:
		for (UINT i = 0; i < s_uCount; i++)
		{
			CeleCtrl::ReWidth(hWnd, CC_CTL + i, LOWORD(lParam));
		}
		break;

	case WM_HOTKEY:
		if (HIWORD(lParam) == VK_TBACK)
		{
			SHSendBackToFocusWindow(uMsg, wParam, lParam);
		}
		break;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			UINT j = 0;
			UINT k = 0;
			for (UINT i = 0; i < s_uCount; i++)
			{
				HWND hCtrl = GetDlgItem(hWnd, CC_CTL + i);
				if (GetWindowLong(hCtrl, GWL_STYLE) & WS_BORDER)
				{
					if (k < _NumOf(g_tzXVar))
					{
						GetWindowText(hCtrl, g_tzXVar[k++], MAX_STR);
					}
				}
				else
				{
					BOOL bChecked = IsDlgButtonChecked(hWnd, (CC_CTL + i));
					if (bChecked)
					{
						j |= (1 << i);
					}

					if (PTSTR ptzReg = (PTSTR) GetWindowLong(hCtrl, GWL_USERDATA))
					{
						PTSTR ptzElse = UStrChr(ptzReg, '?');
						if (ptzElse)
						{
							*ptzElse++ = 0;
							if (!bChecked)
							{
								PTSTR p = UStrChr(ptzReg, '=');
								if (p)
								{
									p++;
									while (*p++ = *ptzElse++);
								}
							}
						}
						REGX(ptzReg);
					}
				}
			}
			return EndDialog(hWnd, j);
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			return EndDialog(hWnd, -1);
		}
		break;
	}

	STATIC CeleScroll s_celeScroll;
	return s_celeScroll.HandleScrollEx(hWnd, uMsg, wParam, lParam);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LINK shortcut to target
HRESULT LINK(PTSTR ptzCmd)
{
	// Parse Shortcut,Target,Param,IconPath,IconIndex
	PTSTR ptzTarget = UStrChr(ptzCmd, CC_SEP);
	if (ptzTarget == NULL)
	{
		return ERROR_PATH_NOT_FOUND;
	}

	*ptzTarget++ = 0;
	PTSTR ptzIcon = NULL;
	PTSTR ptzArg = UStrChr(ptzTarget, CC_SEP);
	if (ptzArg)
	{
		*ptzArg++ = 0;
		ptzIcon = UStrChr(ptzArg, CC_SEP);
		if (ptzIcon)
		{
			*ptzIcon++ = 0;
			PTSTR ptzIndex = UStrChr(ptzIcon, CC_SEP);
			if (ptzIndex)
			{
				*ptzIndex = ',';
			}
		}
	}

	// Generate link file
	TCHAR tzFile[1024];
	PTSTR p = tzFile;
	if (ptzArg && ptzArg[0])
	{
		p += UStrPrint(p, TEXT("%u#\"%s\" %s"), UStrLen(ptzTarget) + UStrLen(ptzTarget) + 3, ptzTarget, ptzArg);
	}
	else
	{
		if (UStrChr(ptzTarget, ' '))
		{
			p += UStrPrint(p, TEXT("%u#\"%s\""), UStrLen(ptzTarget) + 2, ptzTarget);
		}
		else
		{
			p += UStrPrint(p, TEXT("%u#%s"), UStrLen(ptzTarget), ptzTarget);
		}
	}
	if (ptzIcon && ptzIcon[0])
	{
		if (ptzTarget[0])
		{
			p += UStrPrint(p, TEXT("?%s"), ptzIcon);
		}
		else
		{
			p += UStrPrint(p, TEXT("%s"), ptzIcon);
		}
	}


#ifdef _UNICODE
	CHAR szFile[1024];
	p = (PTSTR) szFile;
	INT iSize = UWStrToAStr(szFile, tzFile, sizeof(szFile)) - 1;
#else
	INT iSize = p - tzFile;
#endif

	UDirCreate(ptzCmd);
	return UFileSave(ptzCmd, p, iSize) ? S_OK : E_FAIL;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// COPY files
struct FINDPARAM {BOOL bRecursion; PTSTR ptzFile; PTSTR ptzParam;};
HRESULT UCALLBACK COPY(FINDPARAM* pfParam, PCTSTR ptzDir, CONST PWIN32_FIND_DATA pFind)
{
	if (pFind->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		if ((pFind->cFileName[0] == '.') || !pfParam->bRecursion)
		{
			return ERROR_FILE_NOT_FOUND;
		}
	}
	else if (pfParam->bRecursion && !UStrMatch(pFind->cFileName, pfParam->ptzFile))
	{
		return ERROR_FILE_NOT_FOUND;
	}

	TCHAR tzPath[MAX_PATH];
	UStrPrint(tzPath, TEXT("%s\\%s"), ptzDir, pFind->cFileName);

	TCHAR tzDst[MAX_PATH];
	if (UStrEnd(pfParam->ptzParam)[-1] == '\\')
	{
		UStrPrint(tzDst, (pFind->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? TEXT("%s%s\\") : TEXT("%s%s"), pfParam->ptzParam, pFind->cFileName);
	}
	else
	{
		UStrCopy(tzDst, pfParam->ptzParam);
	}

	UDirCreate(tzDst);
	if (pFind->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		PTSTR ptzParam = pfParam->ptzParam;
		pfParam->ptzParam = tzDst;
		HRESULT hResult = UFileFind((UFOUNDFILEPROC) COPY, pfParam, tzPath);
		pfParam->ptzParam = ptzParam;
		return hResult;
	}
	else
	{
		CeleUtil::RemoveFileAttr(tzDst, FILE_ATTRIBUTE_READONLY);

		if (CopyFile(tzPath, tzDst, FALSE))
		{
			return S_OK;
		}

		UINT uSize = -1;
		PVOID pvData = UFileLoad(tzPath, &uSize);
		if (pvData == NULL)
		{
			return ERROR_OPEN_FAILED;
		}
		UFileSave(tzDst, pvData, uSize);
		UMemFree(pvData);
		return S_OK;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FILE or directory operation
HRESULT FILe(PTSTR ptzCmd)
{
	PTSTR ptzDst = NULL;
	PTSTR p = ptzCmd;
	for (; *p; p++)
	{
		switch (*p)
		{
		case ';':
			*p = 0;
			break;

		case '\\':
			if (p[1] == 0)
			{
				// Create directory
				return !UDirCreate(ptzCmd);
			}
			break;

		case '>':
			*p = 0;
			if (p[-1] == '-')
			{
				p[-1] = 0;
				break;
			}
			else if (p[-1] == '=')
			{
				p[-1] = 0;
			}
			FINDPARAM fp;
			fp.ptzParam = p + 1;
			fp.bRecursion = (*ptzCmd == '!');
			fp.ptzFile = UPathSplit(&ptzCmd);
			if (*fp.ptzFile == 0) fp.ptzFile = TEXT("*");
			return UFileFind((UFOUNDFILEPROC) COPY, &fp, (fp.bRecursion ? ptzCmd + 1 : ptzCmd), fp.bRecursion ? TEXT("*") : fp.ptzFile);

		case '<':
			// Create ASCII file
			*p++ = 0;
			UINT uLen;
			BOOL bAppend;
			bAppend = (*p++ == '=');
			uLen = UStrLen(p);
#ifdef _UNICODE
			CHAR szFile[MAX_STR];
			uLen = WideCharToMultiByte(CP_ACP, 0, p, uLen, szFile, sizeof(szFile), NULL, NULL);
			p = (PTSTR) szFile;
#endif
			return !UFileSave(ptzCmd, p, uLen, bAppend);

		case '{':
			// Create UNICODE file
			*p++ = 0;
			bAppend = (*p++ == '=');
			uLen = UStrLen(p);
#ifndef _UNICODE
			WCHAR wzFile[MAX_STR];
			uLen = MultiByteToWideChar(CP_ACP, 0, p, uLen, wzFile, _NumOf(wzFile));
			p = (PTSTR) wzFile;
#endif
			return !UFileSave(ptzCmd, p, uLen * sizeof(WCHAR), bAppend);
		}
	}

	SHFILEOPSTRUCT so = {0};
	so.pFrom = ptzCmd;
	so.pTo = ptzDst;
	so.wFunc = so.pTo ? FO_MOVE : FO_DELETE;
	so.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_SILENT;

	TCHAR t = p[1];
	p[1] = 0;
	HRESULT hResult = SHFileOperation(&so);
	p[1] = t;
	return hResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// REGistry eXecution
HRESULT REGX(PTSTR ptzCmd)
{
	HKEY hRoot = GetRootKey(ptzCmd);
	if (!hRoot)
	{
		return E_INVALIDARG;
	}

	PTSTR ptzElse = NULL;
	PTSTR ptzVal = UStrChr(ptzCmd, '=');
	if (ptzVal)
	{
		*ptzVal++ = 0;
		ptzElse = UStrChr(ptzVal, '?');
	}

	PTSTR ptzName = UStrRChr(ptzCmd, '\\');
	if (!ptzName)
	{
		return E_INVALIDARG;
	}
	else
	{
		*ptzName++ = 0;
	}

	HRESULT hResult;
	if (ptzVal && (ptzElse == NULL))
	{
		CeleReg reg(ptzCmd, hRoot);
		if (*ptzName == '#')
		{
			hResult = reg.SetInt(ptzName + 1, UStrToInt(ptzVal));
		}
		else if (*ptzName == '@')
		{
			UINT i = 0;
			PBYTE pbVal = (PBYTE) ptzVal;
			while (*ptzVal)
			{
				pbVal[i++] = UChrToHex(ptzVal);
				while (*ptzVal && (*ptzVal++ != ','));
			}
			hResult = reg.SetVal(ptzName + 1, pbVal, i);
		}
		else
		{
			hResult = reg.SetStr(ptzName, ptzVal);
		}
	}
	else
	{
		if ((*ptzName == '-') && (ptzName[1] == 0))
		{
			return CeleReg::DelKey(ptzCmd, hRoot);
		}

		CeleReg reg(hRoot, ptzCmd);
		if (*ptzName == '-')
		{
			hResult = reg.DelVal(ptzName + 1);
		}
		else
		{
			if (*ptzName == '#')
			{
				hResult = reg.GetInt(ptzName + 1);
			}
			else
			{
				g_tzXVar[0][0] = 0;
				UINT uSize = sizeof(g_tzXVar[0]);
				hResult = reg.GetVal(ptzName, g_tzXVar[0], &uSize);
			}
			if (ptzElse)
			{
				ptzElse[0] = 0;
				hResult = (*ptzName == '#') ? (hResult == UStrToInt(ptzVal)) : (UStrCmpI(g_tzXVar[0], ptzVal) == 0);
				ptzVal[-1] = '=';
				ptzElse[0] = '?';
				ptzName[-1] = '\\';
			}
		}
	}

	return hResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RAR eXtract
#include "UnRAR.h"
#ifdef _UNICODE
#define _ArcName ArcNameW
#define STR_RARProcessFile TEXT("RARProcessFileW")
#else
#define _ArcName ArcName
#define STR_RARProcessFile TEXT("RARProcessFile")
#endif
typedef HANDLE (PASCAL *PRAROpenArchiveEx)(struct RAROpenArchiveDataEx *ArchiveData);
typedef INT (PASCAL *PRARReadHeader)(HANDLE hArc, struct RARHeaderData *HeaderData);
typedef INT (PASCAL *PRARProcessFile)(HANDLE hArc, INT iOperation, PTSTR pwzDestPath, PTSTR pwzDestName);
typedef INT (PASCAL *PRARCloseArchive)(HANDLE hArc);
HRESULT RARX(PCTSTR ptzSrcFile, PCTSTR ptzDstDir = TEXT(""))
{
	HMODULE hLib = LoadLibrary(TEXT("UnRAR"));
	if (hLib == NULL)
	{
		return ERROR_DLL_NOT_FOUND;
	}

	PRAROpenArchiveEx pRAROpenArchiveEx = (PRAROpenArchiveEx) GetProcAddress(hLib, TEXT("RAROpenArchiveEx"));
	PRARReadHeader pRARReadHeader = (PRARReadHeader) GetProcAddress(hLib, TEXT("RARReadHeader"));
	PRARProcessFile pRARProcessFile = (PRARProcessFile) GetProcAddress(hLib, STR_RARProcessFile);
	PRARCloseArchive pRARCloseArchive = (PRARCloseArchive) GetProcAddress(hLib, TEXT("RARCloseArchive"));
	if (!pRAROpenArchiveEx || !pRARReadHeader || !pRARProcessFile || !pRARCloseArchive)
	{
		FreeLibrary(hLib);
		return E_NOINTERFACE;
	}

	// File in ROM/Compressed
	TCHAR tzTempFile[MAX_NAME];
	if (GetFileAttributes(ptzSrcFile) & FILE_ATTRIBUTE_COMPRESSED)
	{
		UFileGetTemp(tzTempFile);
		CopyFile(ptzSrcFile, tzTempFile, 0);
		ptzSrcFile = tzTempFile;
	}
	else
	{
		tzTempFile[0] = 0;
	}

	CHAR szCmtBufSize[16384];
	struct RAROpenArchiveDataEx od = {0};
	od._ArcName = (PTSTR) ptzSrcFile;
	od.CmtBuf = szCmtBufSize;
	od.CmtBufSize = sizeof(szCmtBufSize);
	od.OpenMode = RAR_OM_EXTRACT;
	HANDLE hArc = pRAROpenArchiveEx(&od);
	if (od.OpenResult == S_OK)
	{
		struct RARHeaderData hd = {0};
		while ((od.OpenResult = pRARReadHeader(hArc, &hd)) == S_OK)
		{
			od.OpenResult = pRARProcessFile(hArc, RAR_EXTRACT, (PTSTR) ptzDstDir, NULL);
			if (od.OpenResult != S_OK)
			{
				break;
			}
		}
	}

	pRARCloseArchive(hArc);
	FreeLibrary(hLib);
	if (tzTempFile[0])
	{
		UFileDelete(tzTempFile);
	}
	return od.OpenResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SEND keystroke
HRESULT SEND(PCTSTR ptzCmd)
{
	INT i = 0;
	PCTSTR p = ptzCmd;
	do
	{
		if ((*p == CC_SEP) || (*p == 0))
		{
			i = UStrToInt(ptzCmd);
			if (*(p - 1) != '^')
			{
				keybd_event(i, 0, 0, 0);
			}
			if (*(p - 1) != '_')
			{
				keybd_event(i, 0, KEYEVENTF_KEYUP, 0);
			}
			ptzCmd = p + 1;
		}
	}
	while (*p++);
	return i ? S_OK : S_FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// KILL process or window
#include <TLHelp32.h>
#pragma comment(lib, "ToolHelp.lib")
HRESULT KILL(PCTSTR ptzCmd)
{
	HRESULT hResult = S_FALSE;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(PROCESSENTRY32);
		for (BOOL b = Process32First(hSnap, &pe); b; b = Process32Next(hSnap, &pe))
		{
			if (!ptzCmd[UStrEqualI(ptzCmd, pe.szExeFile)])
			{
				HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
				if (hProcess)
				{
					if (TerminateProcess(hProcess, 0))
					{
						hResult = S_OK;
					}
					CloseHandle(hProcess);
				}
			}
		}
		CloseToolhelp32Snapshot(hSnap);
	}

	return hResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SHUTdown
#include <PM.h>
#define IOCTL_HAL_REBOOT		0x101003C
#define IOCTL_HAL_SHUTDOWN		0x1012000
#define IOCTL_COLDBOOT_P527		0x1012004
#define IOCTL_COLDBOOT_Prophet	0x1012594
#define IOCTL_COLDBOOT_Diamond	0x1012048
extern "C" BOOL WINAPI SetCleanRebootFlag();
extern "C" BOOL WINAPI GwesPowerOffSystem();
extern "C" BOOL WINAPI ExitWindowsEx(UINT uFlags, DWORD dwReason);
extern "C" BOOL KernelIoControl(DWORD dwIoControlCode, PVOID pInBuf, DWORD nInBufSize, PVOID pOutBuf, DWORD nOutBufSize, PDWORD pBytesReturned);
HRESULT SHUT(PCTSTR ptzCmd)
{
	BOOL bResult;
	switch (UChrToUpper(*ptzCmd))
	{
	case 'S':
		// HTC
		KernelIoControl(IOCTL_HAL_SHUTDOWN, NULL, 0, NULL, 0, NULL);

	case 'G':
		bResult = GwesPowerOffSystem();
		break;

	case 'P':
		bResult = ExitWindowsEx(EWX_POWEROFF, 0);
		break;

	case 'K':
		keybd_event(VK_OFF, 0, KEYEVENTF_SILENT, 0);
		keybd_event(VK_OFF, 0, KEYEVENTF_SILENT | KEYEVENTF_KEYUP, 0);
		bResult = TRUE;
		break;

	case 'H':
		// HTC
		KernelIoControl((ptzCmd[1] == '0') ? UStrToInt(ptzCmd + 1) : IOCTL_COLDBOOT_Diamond, NULL, 0, NULL, 0, NULL);

	case 'C':
		SetCleanRebootFlag();

	case 'R':
		bResult = KernelIoControl(IOCTL_HAL_REBOOT, NULL, 0, NULL, 0, NULL);
		break;

	default:
		bResult = ExitWindowsEx(EWX_REBOOT, 0);
		break;
	}

	return !bResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Call DLL function
typedef HRESULT (WINAPI *PROC1)(PVOID pv0);
typedef HRESULT (WINAPI *PROC2)(PVOID pv0, PVOID pv1);
typedef HRESULT (WINAPI *PROC3)(PVOID pv0, PVOID pv1, PVOID pv2);
typedef HRESULT (WINAPI *PROC4)(PVOID pv0, PVOID pv1, PVOID pv2, PVOID pv3);
HRESULT CDLL(PTSTR ptzPath, PTSTR ptzProc)
{
	UINT uArg = 0;
	PTSTR ptzArg[4];
	HRESULT hResult = E_NOINTERFACE;
	if (*ptzProc)
	{
		for (PTSTR p = ptzProc; (uArg < 4) && (p = UStrChr(p, CC_SEP)); uArg++)
		{
			*p++ = 0;
			ptzArg[uArg] = p;
			if (*p == '#')
			{
				ptzArg[uArg] = (PTSTR) (INT_PTR) UStrToInt(p + 1);
			}
		}
	}
	else
	{
		ptzProc = TEXT("DllRegisterServer");
	}

	HMODULE hLib = LoadLibrary(ptzPath);
	if (hLib)
	{
		PROC f = GetProcAddress(hLib, ptzProc);
		if (f)
		{
			switch (uArg)
			{
			case 0: hResult = f(); break;
			case 1: hResult = ((PROC1) f)(ptzArg[0]); break;
			case 2: hResult = ((PROC2) f)(ptzArg[0], ptzArg[1]); break;
			case 3: hResult = ((PROC3) f)(ptzArg[0], ptzArg[1], ptzArg[2]); break;
			case 4: hResult = ((PROC4) f)(ptzArg[0], ptzArg[1], ptzArg[2], ptzArg[3]); break;
			}
		}
		FreeLibrary(hLib);
	}

	return hResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EXECute executable file
HRESULT EXEC(PTSTR ptzCmd)
{
	BOOL bAsync = (*ptzCmd == '!');
	if (bAsync)
	{
		ptzCmd++;
	}

	PTSTR ptzArg = UStrChr(ptzCmd, CC_SEP);
	if (ptzArg)
	{
		*ptzArg++ = 0;
	}

	// Pass to shell to execute it
	HANDLE hProcess = (HANDLE) CeleUtil::ShellOpen(ptzCmd, ptzArg, g_hWnd, SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI);
	if (hProcess == NULL)
	{
		return GetLastError();
	}

	// Wait for end
	if (!bAsync)
	{
		WaitForSingleObject(hProcess, INFINITE);
	}
	CloseHandle(hProcess);
	return S_OK;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Associate file
HRESULT ASOC(PCTSTR ptzCmd)
{
	if (*ptzCmd == '!')
	{
		return CeleReg::DelKey(ptzCmd[1] ? &ptzCmd[1] : STR_AppName, HKEY_CLASSES_ROOT);
	}

	if (*ptzCmd)
	{
		CeleReg reg1(ptzCmd, HKEY_CLASSES_ROOT);
		reg1.SetStr(NULL, STR_AppName);
	}

	CeleReg reg2(STR_AppName, HKEY_CLASSES_ROOT);
	reg2.SetStr(NULL, STR_AppName);

	TCHAR tzPath[MAX_PATH];
	PTSTR p = tzPath;
	*p++ = '"';
	p += GetModuleFileName(NULL, p, MAX_PATH);
	CeleReg reg3(STR_AppName TEXT("\\DefaultIcon"), HKEY_CLASSES_ROOT);
	UStrCopy(p, TEXT(",-101"));
	reg3.SetStr(NULL, tzPath + 1);

	UStrCopy(p, TEXT("\" %1"));
	CeleReg reg4(STR_AppName TEXT("\\Shell\\Open\\Command"), HKEY_CLASSES_ROOT);
	reg4.SetStr(NULL, tzPath);
	return S_OK;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DISPlay rotation
HRESULT DISP(PTSTR ptzCmd)
{
	DEVMODE dm = {0};
	dm.dmSize = sizeof(DEVMODE);
	dm.dmFields = DM_DISPLAYORIENTATION;
	HRESULT hResult = ChangeDisplaySettingsEx(NULL, &dm, NULL, CDS_TEST, NULL);
	if (hResult == DISP_CHANGE_SUCCESSFUL)
	{
		if (ptzCmd[0])
		{
			dm.dmDisplayOrientation = UStrToInt(ptzCmd);
		}
		else
		{
			dm.dmDisplayOrientation = !dm.dmDisplayOrientation;
		}
		hResult = ChangeDisplaySettingsEx(NULL, &dm, NULL, CDS_RESET, NULL);
	}
	return hResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Send MeSsaGe
HRESULT POST(PTSTR ptzCmd)
{
	BOOL bSendMessage = (*ptzCmd == '=');
	if (bSendMessage)
	{
		ptzCmd++;
	}

	PTSTR ptzMsg = UStrChr(ptzCmd, CC_SEP);
	if (ptzMsg == NULL)
	{
		return E_INVALIDARG;
	}
	else
	{
		*ptzMsg++ = 0;
	}

	HWND hWnd;
	if (*ptzCmd == '$')
	{
		hWnd = HWND_BROADCAST;
	}
	else
	{
		hWnd = FindWindow(ptzCmd, NULL);
		if (hWnd = NULL)
		{
			hWnd = FindWindow(NULL, ptzCmd);
			if (hWnd == NULL)
			{
				return ERROR_INVALID_HANDLE;
			}
		}
	}

	PTSTR wParam = UStrChr(ptzMsg, CC_SEP);
	if (wParam)
	{
		*wParam++ = 0;
		if (*wParam == '$')
		{
			wParam++;
		}
		else
		{
			wParam = (PTSTR) (INT_PTR) UStrToInt(wParam + 1);
		}
	}

	PTSTR lParam = UStrChr(ptzMsg, CC_SEP);
	if (lParam)
	{
		*lParam++ = 0;
		if (*lParam == '$')
		{
			lParam++;
		}
		else
		{
			lParam = (PTSTR) (INT_PTR) UStrToInt(lParam + 1);
		}
	}

	if (bSendMessage)
	{
		return SendMessage(hWnd, UStrToInt(ptzMsg), (WPARAM) wParam, (LPARAM) lParam);
	}
	else
	{
		return PostMessage(hWnd, UStrToInt(ptzMsg), (WPARAM) wParam, (LPARAM) lParam);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OPEN file or shell command
HRESULT UCALLBACK OPEN(FINDPARAM* pfParam, PCTSTR ptzDir, CONST PWIN32_FIND_DATA pFind)
{
	if (pFind->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		if ((pFind->cFileName[0] == '.') || !pfParam->bRecursion)
		{
			return ERROR_FILE_NOT_FOUND;
		}
	}
	else if (pfParam->bRecursion && !UStrMatch(pFind->cFileName, pfParam->ptzFile))
	{
		return ERROR_FILE_NOT_FOUND;
	}

	TCHAR tzPath[MAX_PATH];
	UStrPrint(tzPath, TEXT("%s\\%s"), ptzDir, pFind->cFileName);
	if (pFind->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		return UFileFind((UFOUNDFILEPROC) OPEN, pfParam, tzPath);
	}

	PTSTR ptzExt = UStrRChr(pFind->cFileName, '.');
	if (ptzExt)
	{
		ptzExt++;
		if ((UStrCmpI(ptzExt, TEXT("ccs")) == 0) || (UStrCmpI(ptzExt, TEXT("reg")) == 0) || (UStrCmpI(ptzExt, TEXT("rgu")) == 0) ||
			(UStrCmpI(ptzExt, TEXT("xml")) == 0) || (UStrCmpI(ptzExt, TEXT("provxml")) == 0))
		{
			return LOAD(tzPath);
		}
		else if ((UStrCmpI(ptzExt, TEXT("dll")) == 0) || (UStrCmpI(ptzExt, TEXT("ocx")) == 0) || (UStrCmpI(ptzExt, TEXT("ax")) == 0))
		{
			return CDLL(tzPath, pfParam->ptzParam);
		}
		else if (UStrCmpI(ptzExt, TEXT("rar")) == 0)
		{
			return RARX(tzPath, pfParam->ptzParam);
		}
#ifdef ZLIB_H
		else if (UStrCmpI(ptzExt, TEXT("tgz")) == 0)
		{
			return RARX(tzPath, pfParam->ptzParam);
		}
#endif
	}

	return CeleUtil::ShellOpen(tzPath, pfParam->ptzParam, g_hWnd, SEE_MASK_FLAG_NO_UI);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dispatch command
HRESULT Dispatch(PTSTR ptzCmd, CXT& XT)
{
	// Get command ID
	UINT uCmd;
	PTSTR p = ptzCmd;
	for (uCmd = 0; uCmd < _NumOf(c_tzCmd); uCmd++)
	{
		if (UStrEqualI(p, c_tzCmd[uCmd]) >= CC_LEN)
		{
			// Skip white space
			for (p += CC_LEN; (*p == ' ') || (*p == '\t'); p++);
			break;
		}
	}

	switch (uCmd)
	{
	case CMD_LOAD:
		return LOAD(p);

	case CMD_BATC:
		return BATC(p, XT);

	case CMD_IFEX:
		if (*p)
		{
			if (PTSTR ptzArg = UStrChr(p, CC_SEP))
			{
				*ptzArg++ = 0;
				XT.bElse = !IFEX(p);
				if (!XT.bElse)
				{
					Dispatch(ptzArg, XT);
				}
				return XT.hXVar;
			}
			else if (XT.bElse = !IFEX(p))
			{
				if (p = UStrStr(XT.ptzNext, TEXT("IFEX\r\n")))
				{
					XT.ptzNext = p + CC_LEN + 2;
					return S_OK;
				}
				else if (p = UStrStr(XT.ptzNext, TEXT("IFEX\n")))
				{
					XT.ptzNext = p + CC_LEN + 1;
					return S_OK;
				}
				else
				{
					XT.ptzNext = TEXT("");
					return S_FALSE;
				}
			}
		}
		return S_OK;

	case CMD_ELSE:
		if (XT.bElse) Dispatch(p, XT);
		return XT.hXVar;

	case CMD_EVAL:
		return EVAL(p);

	case CMD_LINK:
		return LINK(p);

	case CMD_FILE:
		return FILe(p);

	case CMD_REGX:
		return REGX(p);

	case CMD_RARX:
		return RARX(p, UStrSplit(p, CC_SEP));

#ifdef ZLIB_H
	case CMD_TGZX:
		return TGZX(p, UStrSplit(p, CC_SEP));
#endif

	case CMD_SEND:
		return SEND(p);

	case CMD_WAIT:
		Sleep(UStrToInt(p));
		return S_OK;

	case CMD_KILL:
		return KILL(p);

	case CMD_SHUT:
		return SHUT(p);

	case CMD_PLAY:
		return PLAY(p);

	case CMD_BEEP:
		return !MessageBeep(UStrToInt(p));

	case CMD_MSGX:
		return MSGX(p);

	case CMD_DLGX:
		if ((*p == '@') || (*p == '{') || (*p == '[') || (*p == '<') || (*p == '$'))
		{
			return CeleCtrl::DlgBox(g_hWnd, DLGX, p, g_ptzAppName);
		}
		return CeleCtrl::DlgBox(g_hWnd, DLGX, UStrSplit(p, CC_SEP), p);

	case CMD_EXEC:
		return EXEC(p);

	case CMD_CDLL:
		return CDLL(p, UStrSplit(p, CC_SEP));

	case CMD_DISP:
		return DISP(p);

	case CMD_POST:
		return POST(p);

	case CMD_HELP:
		return CeleCtrl::DlgBox(g_hWnd, HELP, p);

	case CMD_CCUI:
		if (HWND hWnd = FindWindow(NULL, STR_AppName))
		{
			return SetForegroundWindow((HWND) (((DWORD) hWnd) | 0x00000001));
		}
		else
		{
			SHInitExtraControls();
			g_ptzCurFile = XT.ptzFile;
			return DialogBoxParam(g_hInst, MAKEINTRESOURCE(IDD_Main), NULL, CCUI, (LPARAM) p);
		}
		break;

	case CMD_CALL:
	case CMD_GOTO:
	case CMD_PROC:
		UMemCopy(ptzCmd, c_tzCmd[(uCmd == CMD_PROC) ? CMD_ENDP : CMD_PROC], CC_LEN * sizeof(TCHAR));
		if (p = UStrStr(XT.ptzNext, ptzCmd))
		{
			p += CC_LEN;
			while (*p && (*p++ != '\n'));
			if (uCmd == CMD_CALL)
			{
				return Process(p, XT.ptzFile);
			}
			else
			{
				XT.ptzNext = p;
			}
			return S_OK;
		}
		else if (uCmd == CMD_PROC)
		{
			XT.ptzNext = TEXT("");
		}
		return S_FALSE;

	case CMD_ENDP:
		XT.ptzNext = TEXT("");
		return S_OK;

	case CMD_ASOC:
		return ASOC(p);

	default:
		FINDPARAM fp;
		fp.bRecursion = (*p == '!');
		fp.ptzParam = UStrSplit(p, CC_SEP);
		fp.ptzFile = UPathSplit(&p);
		return UFileFind((UFOUNDFILEPROC) OPEN, &fp, fp.bRecursion ? (p + 1) : p, fp.bRecursion ? TEXT("*") : fp.ptzFile);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Expand macro
#include <ShlObj.h>
#pragma comment(lib, "CEShell.lib")
PTSTR Expand(PTSTR ptzDst, TCHAR tMacro, CXT& XT)
{
	INT i;
	switch (tMacro)
	{
	case 'E':
		for (PCTSTR p = XT.ptzFile; *p; *ptzDst++ = *p++);
		return ptzDst;

	case 'C':
		if (PCTSTR q = UStrRChr(XT.ptzFile, '\\'))
		{
			for (PCTSTR p = XT.ptzFile; p < q; *ptzDst++ = *p++);
		}
		return ptzDst;

	case 'T':
		ptzDst += GetTimeFormat(LOCALE_USER_DEFAULT, 0, NULL, NULL, ptzDst, MAX_NAME) - 1;
		return ptzDst;

	case 'D':
		ptzDst += GetDateFormat(LOCALE_USER_DEFAULT, 0, NULL, NULL, ptzDst, MAX_NAME) - 1;
		return ptzDst;

	case 'X':
	case 'x':
		ptzDst += UStrPrint(ptzDst,  (tMacro == 'X') ? TEXT("%#X") : TEXT("%d"), XT.hXVar);
		return ptzDst;

	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		for (PTSTR p = g_tzXVar[tMacro - '0']; *p; p++) *ptzDst++ = *p;
		return ptzDst;

	case 'A': i = CSIDL_APPDATA; break;
	case 'Y': i = CSIDL_PERSONAL; break;
	case 'S': i = CSIDL_STARTUP; break;
	case 'W': i = CSIDL_WINDOWS; break;
	case 'P': i = CSIDL_PROGRAMS; break;
	case 'M': i = CSIDL_STARTMENU; break;
	case 'V': i = CSIDL_FAVORITES; break;
	case 'F': i = CSIDL_PROGRAM_FILES; break;

	case 'R': *ptzDst++ = '\r'; return ptzDst;
	case 'N': *ptzDst++ = '\n'; return ptzDst;
	default: *ptzDst++ = tMacro; return ptzDst;
	}

	SHGetSpecialFolderPath(NULL, ptzDst, i, TRUE);
	ptzDst += UStrLen(ptzDst);
	return ptzDst;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Execute command
HRESULT Execute(PTSTR ptzCmd, CXT& XT)
{
	while ((*ptzCmd == ' ') || (*ptzCmd == '\t')) ptzCmd++;
	if (*ptzCmd == '[')
	{
		XT.hXVar = Import((PTSTR) XT.ptzCur, &XT.ptzNext);
	}
	else if (*ptzCmd == '<')
	{
		PTSTR p = (PTSTR) XT.ptzNext;
		for (; *p; p++)
		{
			if ((p[0] == '<') && (p[1] == '>'))
			{
				p[0] = 0;
				break;
			}
		}

		PTSTR ptzOut = NULL;
		XT.hXVar = DMProcessConfigXML(XT.ptzCur, CFGFLAG_PROCESS, &ptzOut);
		if (ptzOut)
		{
			delete[] ptzOut;
		}

		if (p[1] == '>')
		{
			p[0] = '<';
			p += 2;
		}

		while (*p && (*p++ != '\n'));
		XT.ptzNext = p;
	}
	else if (*ptzCmd && (*ptzCmd != ';'))
	{
		if (g_hWnd)
		{
			Log(ptzCmd);
			Log(TEXT("\r\n"));
			XT.hXVar = Dispatch(ptzCmd, XT);

			TCHAR tzStr[MAX_PATH];;
			UINT i = UStrPrint(tzStr, TEXT("     %08X "), XT.hXVar);
			i += FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, XT.hXVar, 0, tzStr + i, _NumOf(tzStr), NULL);
			UStrCopy(tzStr + i, (tzStr[i - 1] == '\n') ? TEXT("\r\n") : TEXT("\r\n\r\n"));

			Log(tzStr);
		}
		else
		{
			XT.hXVar = Dispatch(ptzCmd, XT);
		}
	}
	return XT.hXVar;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Process script
HRESULT Process(PCTSTR ptzScript, PCTSTR ptzFile)
{
	TCHAR tzCmd[4096];
	PTSTR ptzDst = tzCmd;
	CXT XT = {ptzScript, ptzScript, ptzFile};
	while (TRUE)
	{
		TCHAR t = *XT.ptzNext++;
		switch (t)
		{
		case '\r':
		case '\n':
			*ptzDst = 0;
			Execute(tzCmd, XT);
			ptzDst = tzCmd;
			XT.ptzCur = XT.ptzNext;
			break;

		case 0:
			*ptzDst = 0;
			return Execute(tzCmd, XT);

		case ',':
			*ptzDst++ = CC_SEP;
			break;

		case '%':
			t = *XT.ptzNext++;
			if (t == '#')
			{
				UINT j = UChrToHex(XT.ptzNext);
				XT.ptzNext += 2;
				j = (j << 8) | UChrToHex(XT.ptzNext);
				XT.ptzNext += 2;
				*ptzDst++ = j;
			}
			else
			{
				ptzDst = Expand(ptzDst, t, XT);
			}
			break;

		default:
			*ptzDst++ = t;
			break;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
