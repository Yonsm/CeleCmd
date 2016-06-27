

/**********************************************************************************************************************/
/* Uniform Base Library 1.6.122 WIN32 */
/* Copyright (C) Yonsm 2006-2008, All Rights Reserved.*/
/* Resource Switch: -n */
/* Compiler Switch: /Ob2 */
/* Global Variable: g_hInst */
/* Global Variable: g_ptzAppName */
#pragma once

#ifdef _DEBUG
#define _TRACE
//#define _TRACE_TIME
//#define _TRACE_TO_FILE
//#define _TRACE_TO_CONSOLE
#endif

#define _WINSTR
#ifndef WINCE
//#define _SHLSTR
#endif

#if defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NON_CONFORMING_SWPRINTFS

#include <TChar.h>
#include <StdIO.h>
#include <StdLib.h>
#include <String.h>
#include <Memory.h>
#include <Windows.h>
#include <ShLwApi.h>
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* Define */
#ifndef CONST
#define CONST						const
#endif

#ifndef STATIC
#define STATIC						static
#endif

#ifndef INLINE
#define INLINE						__inline
#endif

#ifndef FINLINE
#define FINLINE						__forceinline
#endif

#ifndef ISTATIC
#define ISTATIC						__forceinline static
#endif

#define UCALL						WINAPI
#define UCALLBACK					CALLBACK
#define UAPI(x)						INLINE x UCALL

#ifdef __cplusplus
#define UDEF(x)						= x
#else
#define UDEF(x)
#endif
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* Type */
#define VOID						void
typedef void						*PVOID;
typedef void						*HANDLE;
typedef HANDLE						*PHANDLE;
typedef int							BOOL, *PBOOL;
typedef float						FLOAT, *PFLOAT;
typedef double						DOUBLE, *PDOUBLE;

typedef int							INT, *PINT;
typedef signed char					INT8, *PINT8;
typedef signed short				INT16, *PINT16;
typedef signed int					INT32, *PINT32;
typedef signed __int64				INT64, *PINT64;

typedef unsigned int				UINT, *PUINT;
typedef unsigned char				UINT8, *PUINT8;
typedef unsigned short				UINT16, *PUINT16;
typedef unsigned int				UINT32, *PUINT32;
typedef unsigned __int64			UINT64, *PUINT64;

typedef unsigned char				BYTE, *PBYTE;
typedef unsigned short				WORD, *PWORD;
typedef unsigned long				DWORD, *PDWORD;
typedef unsigned __int64			QWORD, *PQWORD;

#if defined(_WIN64)
typedef __int64						INT_PTR, *PINT_PTR;
typedef unsigned __int64			UINT_PTR, *PUINT_PTR;
#else
#if (_MSC_VER < 1300)
typedef long						INT_PTR, *PINT_PTR;
typedef unsigned long				UINT_PTR, *PUINT_PTR;
#else
typedef int							INT_PTR, *PINT_PTR;
typedef unsigned int				UINT_PTR, *PUINT_PTR;
#endif
#endif

#ifndef _WCHAR_T_DEFINED
#define _WCHAR_T_DEFINED
typedef unsigned short				wchar_t;
#endif

typedef char						CHAR, *PCHAR;
typedef wchar_t						WCHAR, *PWCHAR;
#ifdef _UNICODE
typedef WCHAR						TCHAR, *PTCHAR;
typedef WORD						UTCHAR, *PUTCHAR;
#else
typedef CHAR						TCHAR, *PTCHAR;
typedef BYTE						UTCHAR, *PUTCHAR;
#endif

typedef CHAR						*PSTR;
typedef WCHAR						*PWSTR;
typedef TCHAR						*PTSTR;
typedef CONST CHAR					*PCSTR;
typedef CONST WCHAR					*PCWSTR;
typedef CONST TCHAR					*PCTSTR;

typedef CONST VOID					*PCVOID;
typedef CONST BYTE					*PCBYTE;

#ifndef VALIST
#define VALIST						va_list
#endif

#ifndef TEXT
#ifdef _UNICODE
#define TEXT(t)						L##t
#else
#define TEXT(t)						t
#endif
#endif
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* Const */
#ifndef TRUE
#define TRUE						1
#endif

#ifndef FALSE
#define FALSE						0
#endif

#ifndef NULL
#define NULL						0
#endif

#ifndef MAX_STR
#define MAX_STR						1024
#endif

#ifndef MAX_PATH
#define MAX_PATH					260
#endif

#ifndef MAX_NAME
#define MAX_NAME					80
#endif

#define UFILE_READ					GENERIC_READ
#define UFILE_WRITE					GENERIC_WRITE
#define UFILE_READWRITE				(GENERIC_READ | GENERIC_WRITE)

#define UFILE_BEGIN					FILE_BEGIN
#define UFILE_CURRENT				FILE_CURRENT
#define UFILE_END					FILE_END

#define UCP_ANSI					CP_ACP
#define UCP_OEM						CP_OEMCP
#define UCP_MAC						CP_MACCP
#define UCP_SYMBOL					CP_SYMBOL
#define UCP_UTF7					CP_UTF7
#define UCP_UTF8					CP_UTF8
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* Macro */
#define _NumOf(s)					(sizeof(s) / sizeof(s[0]))
#define _Zero(p)					UMemSet(p, 0, sizeof(*p))

#define _SafeFree(p)				if (p) {UMemFree(p); p = NULL;}
#define _SafeDelete(p)				if (p) {delete p; p = NULL;}
#define _SafeRelease(p)				if (p) {(p)->Release(); p = NULL;}

#define _RectWidth(r)				((r).right - (r).left)
#define _RectHeight(r)				((r).bottom - (r).top)

#define _IsIntRes(r)				((((UINT_PTR) (r)) >> 16) == 0)
#define _MakeIntRes(i)				((PTSTR) ((DWORD) (i)))

#define _DibStride(w, i)			(((((w) * i) + 31) & ~31) / 8)
#define _DibSize(w, i, h)			(_DibStride((w), i) * (h))
#define _DibBits(p, w, i, x, y)		((p) + _DibStride((w), (i)) * (y) + (x) * 3)
#define _DibStride24(w)				(((w) + (w) + (w) + 3) & 0xFFFFFFFC)
#define _DibSize24(w, h)			(_DibStride24(w) * (h))
#define _DibBits24(p, w, x, y)		((p) + _DibStride24(w) * (y) + (x) * 3)
#define _DibStride32(w)				((w) * 4)
#define _DibSize32(w, h)			(_DibStride32(w) * (h))
#define _DibBits32(p, w, x, y)		((p) + _DibStride32(w) * (y) + (x) * 4)
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* Global */
__declspec(selectany) HINSTANCE g_hInst = NULL;
__declspec(selectany) PCTSTR g_ptzAppName = NULL;
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* Memory */
UAPI(PVOID) UMemAlloc(UINT uSize)
{
	return malloc(uSize);
}

UAPI(PVOID) UMemRealloc(PVOID pvMem, UINT uSize)
{
	return realloc(pvMem, uSize);
}

UAPI(VOID) UMemFree(PVOID pvMem)
{
	free(pvMem);
}

UAPI(PVOID) UMemAlignAlloc(UINT uSize, UINT16 uAlign UDEF(16))
{
	PVOID pvAlloc = UMemAlloc(uSize + sizeof(UINT16) + uAlign);
	if (pvAlloc)
	{
		UINT_PTR uMem = (UINT_PTR) pvAlloc + sizeof(UINT16);
		uMem = (uMem + (uAlign - 1)) & ~(uAlign - 1);
		((PUINT16) uMem)[-1] = (UINT16) (uMem - (UINT_PTR) pvAlloc);
		return (PVOID) uMem;
	}
	return NULL;
}

UAPI(VOID) UMemAlignFree(PVOID pvMem)
{
	if (pvMem)
	{
		pvMem = (PVOID) ((UINT_PTR) pvMem - ((PUINT16) pvMem)[-1]);
		UMemFree(pvMem);
	}
}

UAPI(PVOID) UMemSet(PVOID pvMem, CHAR cVal, UINT uSize)
{
	return memset(pvMem, cVal, uSize);
}

UAPI(PVOID) UMemCopy(PVOID pvDst, PCVOID pvSrc, UINT uSize)
{
	return memcpy(pvDst, pvSrc, uSize);
}

UAPI(PVOID) UMemMove(PVOID pvDst, PCVOID pvSrc, UINT uSize)
{
	return memmove(pvDst, pvSrc, uSize);
}

UAPI(INT) UMemCmp(PCVOID pvMem1, PCVOID pvMem2, UINT uSize)
{
	return memcmp(pvMem1, pvMem2, uSize);
}
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* ASCII Char */
UAPI(BOOL) UAChrIsNum(CHAR c)
{
	return (c >= '0') && (c <= '9');
}

UAPI(BOOL) UAChrIsAlpha(CHAR c)
{
	return (c >= 'A') && (c <= 'Z') || (c >= 'a') && (c <= 'z');
}

UAPI(BOOL) UAChrIsSymbol(CHAR c)
{
	return (c > ' ') && (c < '0') || (c > '9') && (c < 'A') || (c > 'Z') && (c < 'a') || (c > 'z') && (c < 127);
}

UAPI(BOOL) UAChrIsPrintable(CHAR c)
{
	return ((c >= ' ') && (c <= '~')) || (c == '\r') || (c == '\n') || (c == '\t');
}

UAPI(CHAR) UAChrToLower(CHAR c)
{
	return ((c >= 'A') && (c <= 'Z')) ? (c - 'A' + 'a') : c;
}

UAPI(CHAR) UAChrToUpper(CHAR c)
{
	return ((c >= 'a') && (c <= 'z')) ? (c + 'A' - 'a') : c;
}

UAPI(BYTE) UAChrToHex(CONST CHAR c[2])
{
	CONST STATIC BYTE c_bHexVal[128] =
	{
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	};
	return (c_bHexVal[c[0]] << 4) | c_bHexVal[c[1]];
}

UAPI(VOID) UAChrFromHex(CHAR c[2], BYTE b)
{
	CONST STATIC CHAR c_szHexChr[] = "0123456789ABCDEF";
	c[0] = c_szHexChr[b >> 4];
	c[1] = c_szHexChr[b & 0x0F];
}

UAPI(BOOL) UAChrEqualI(CHAR c1, CHAR c2)
{
	return ((c1 == c2) || (UAChrToUpper(c1) == UAChrToUpper(c2)));
}
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* UNICODE Char */
UAPI(BOOL) UWChrIsNum(WCHAR w)
{
	return (w >= '0') && (w <= '9');
}

UAPI(BOOL) UWChrIsAlpha(WCHAR w)
{
	return (w >= 'A') && (w <= 'Z') || (w >= 'a') && (w <= 'z');
}

UAPI(BOOL) UWChrIsSymbol(WCHAR w)
{
	return (w > ' ') && (w < '0') || (w > '9') && (w < 'A') || (w > 'Z') && (w < 'a') || (w > 'z') && (w < 127);
}

UAPI(BOOL) UWChrIsPrintable(WCHAR w)
{
	return ((w >= ' ') && (w <= '~')) || (w == '\r') || (w == '\n') || (w == '\t');
}

UAPI(WCHAR) UWChrToLower(WCHAR w)
{
	return ((w >= 'A') && (w <= 'Z')) ? (w - 'A' + 'a') : w;
}

UAPI(WCHAR) UWChrToUpper(WCHAR w)
{
	return ((w >= 'a') && (w <= 'z')) ? (w + 'A' - 'a') : w;
}

UAPI(BYTE) UWChrToHex(CONST WCHAR w[2])
{
	CONST STATIC BYTE c_bHexVal[128] =
	{
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	};
	return (c_bHexVal[w[0]] << 4) | c_bHexVal[w[1]];
}

UAPI(VOID) UWChrFromHex(WCHAR w[2], BYTE b)
{
	CONST STATIC WCHAR c_wzHexChr[] = L"0123456789ABCDEF";
	w[0] = c_wzHexChr[b >> 4];
	w[1] = c_wzHexChr[b & 0x0F];
}

UAPI(BOOL) UWChrEqualI(WCHAR w1, WCHAR w2)
{
	return (w1 == w2) || (UWChrToUpper(w1) == UWChrToUpper(w2));
}
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* String Conversion */
UAPI(UINT) UAStrToWStr(PWSTR pwzDst, PCSTR pszSrc, UINT uDstLen, UINT uSrcLen UDEF(-1), UINT uCodePage UDEF(UCP_ANSI))
{
	return MultiByteToWideChar(uCodePage, 0, pszSrc, uSrcLen, pwzDst, uDstLen);
}

UAPI(UINT) UWStrToAStr(PSTR pszDst, PCWSTR pwzSrc, UINT uDstLen, UINT uSrcLen UDEF(-1), UINT uCodePage UDEF(UCP_ANSI))
{
	return WideCharToMultiByte(uCodePage, 0, pwzSrc, uSrcLen, pszDst, uDstLen, NULL, NULL);
}
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* ASCII String */
UAPI(PSTR) UAStrEnd(PCSTR pszStr)
{
	while (*pszStr)
	{
		pszStr++;
	}
	return (PSTR) pszStr;
}

UAPI(UINT) UAStrLen(PCSTR pszStr)
{
	return (UINT) (UAStrEnd(pszStr) - pszStr);
}

UAPI(UINT) UAStrCopy(PSTR pszDst, PCSTR pszSrc)
{
	PSTR p = pszDst;
	while (*p++ = *pszSrc++);
	return (UINT) (p - pszDst);
}

UAPI(UINT) UAStrCopyN(PSTR pszDst, PCSTR pszSrc, UINT uDstLen)
{
	PSTR p = pszDst;
	while (*pszSrc && ((INT) (--uDstLen) > 0))
	{
		*p++ = *pszSrc++;
	}
	*p++ = 0;
	return (UINT) (p - pszDst);
}

UAPI(UINT) UAStrCat(PSTR pszDst, PCSTR pszSrc)
{
	PSTR p = UAStrEnd(pszDst);
	return (UINT) (p - pszDst) + UAStrCopy(p, pszSrc);
}

UAPI(INT) UAStrCmp(PCSTR pszStr1, PCSTR pszStr2)
{
#ifdef _WINSTR
	return lstrcmpA(pszStr1, pszStr2);
#else
	return strcmp(pszStr1, pszStr2);
#endif
}

UAPI(INT) UAStrCmpI(PCSTR pszStr1, PCSTR pszStr2)
{
#ifdef _WINSTR
	return lstrcmpiA(pszStr1, pszStr2);
#else
	return _stricmp(pszStr1, pszStr2);
#endif
}

UAPI(INT) UAStrCmpN(PCSTR pszStr1, PCSTR pszStr2, UINT uLen)
{
#ifdef _SHLSTR
	return StrCmpNA(pszStr1, pszStr2, uLen);
#else
	return strncmp(pszStr1, pszStr2, uLen);
#endif
}

UAPI(INT) UAStrCmpNI(PCSTR pszStr1, PCSTR pszStr2, UINT uLen)
{
#ifdef _SHLSTR
	return StrCmpNIA(pszStr1, pszStr2, uLen);
#else
	return _strnicmp(pszStr1, pszStr2, uLen);
#endif
}

UAPI(PSTR) UAStrChr(PCSTR pszStr, CHAR cChr)
{
#ifdef _SHLSTR
	return (PSTR) StrChrA(pszStr, cChr);
#else
	return (PSTR) strchr(pszStr, cChr);
#endif
}

UAPI(PSTR) UAStrRChr(PCSTR pszStr, CHAR cChr)
{
#ifdef _SHLSTR
	return (PSTR) StrRChrA(pszStr, NULL, cChr);
#else
	return (PSTR) strrchr(pszStr, cChr);
#endif
}

UAPI(PSTR) UAStrStr(PCSTR pszStr1, PCSTR pszStr2)
{
#ifdef _SHLSTR
	return (PSTR) StrStrA(pszStr1, pszStr2);
#else
	return (PSTR) strstr(pszStr1, pszStr2);
#endif
}

UAPI(PSTR) UAStrStrI(PCSTR pszStr1, PCSTR pszStr2)
{
#ifdef _SHLSTR
	return StrStrIA(pszStr1, pszStr2);
#else
	PSTR p = (PSTR) pszStr1;
	while (*p)
	{
		PSTR s1 = p;
		PSTR s2 = (PSTR) pszStr2;

		while (*s1 && *s2 && UAChrEqualI(*s1, *s2))
		{
			s1++;
			s2++;
		}

		if (*s2 == 0)
		{
			return p;
		}

		p++;
	}
	return NULL;
#endif
}

UAPI(PSTR) UAStrRep(PSTR pszStr, CHAR cFind UDEF('|'), CHAR cRep UDEF(0))
{
	PSTR p = pszStr;
	for (; *p; p++)
	{
		if (*p == cFind)
		{
			*p = cRep;
		}
	}
	return pszStr;
}

UAPI(PSTR) UAStrTrim(PSTR pszStr, CHAR cTrim UDEF('"'))
{
	if (*pszStr == cTrim)
	{
		PSTR p = pszStr + UAStrLen(pszStr) - 1;
		if (*p == cTrim)
		{
			*p = 0;
		}
		return pszStr + 1;
	}
	return pszStr;
}

UAPI(PSTR) UAStrSplit(PSTR pszStr, CHAR cSplit)
{
	while (*pszStr)
	{
		if (*pszStr == cSplit)
		{
			*pszStr++ = 0;
			break;
		}
		pszStr++;
	}
	return pszStr;
}

UAPI(PSTR) UAStrRSplit(PSTR pszStr, CHAR cSplit)
{
	PSTR p;
	PSTR pszEnd = UAStrEnd(pszStr);
	for (p = pszEnd; p >= pszStr; p--)
	{
		if (*p == cSplit)
		{
			*p++ = 0;
			return p;
		}
	}
	return pszEnd;
}

UAPI(UINT) UAStrEqual(PCSTR pszStr1, PCSTR pszStr2)
{
	UINT i = 0;
	while (pszStr1[i] && (pszStr1[i] == pszStr2[i]))
	{
		i++;
	}
	return i;
}

UAPI(UINT) UAStrEqualI(PCSTR pszStr1, PCSTR pszStr2)
{
	UINT i = 0;
	while (pszStr1[i] && UWChrEqualI(pszStr1[i], pszStr2[i]))
	{
		i++;
	}
	return i;
}

UAPI(BOOL) UAStrMatch(PCSTR pszStr, PCSTR pszPat)
{
	PCSTR s, p;
	BOOL bStar = FALSE;

__LoopStart:
	for (s = pszStr, p = pszPat; *s; s++, p++)
	{
		switch (*p)
		{
		case '?':
			/*if (*s == '.') goto __StartCheck;*/
			break;

		case '*':
			bStar = TRUE;
			pszStr = s, pszPat = p;
			if (!*++pszPat) return TRUE;
			goto __LoopStart;

		default:
			if (*s != *p)
			{
				/*__StartCheck:*/
				if (!bStar) return FALSE;
				pszStr++;
				goto __LoopStart;
			}
			break;
		}
	}
	if (*p == '*') ++p;
	return (!*p);
}

UAPI(BOOL) UAStrMatchI(PCSTR pszStr, PCSTR pszPat)
{
	PCSTR s, p;
	BOOL bStar = FALSE;

__LoopStart:
	for (s = pszStr, p = pszPat; *s; s++, p++)
	{
		switch (*p)
		{
		case '?':
			/*if (*s == '.') goto __StartCheck;*/
			break;

		case '*':
			bStar = TRUE;
			pszStr = s, pszPat = p;
			if (!*++pszPat) return TRUE;
			goto __LoopStart;

		default:
			if (!UAChrEqualI(*s, *p))
			{
				/*__StartCheck:*/
				if (!bStar) return FALSE;
				pszStr++;
				goto __LoopStart;
			}
			break;
		}
	}
	if (*p == '*') ++p;
	return (!*p);
}

UAPI(PSTR) UAStrToUpper(PSTR pszStr)
{
#ifdef _WINSTR
	return CharUpperA(pszStr);
#else
	return _strupr(pszStr);
#endif
}

UAPI(PSTR) UAStrToLower(PSTR pszStr)
{
#ifdef _WINSTR
	return CharLowerA(pszStr);
#else
	return _strlwr(pszStr);
#endif
}

UAPI(INT) UAStrToInt(PCSTR pszStr)
{
#ifdef _SHLSTR
	INT i = 0;
	StrToIntExA(pszStr, STIF_SUPPORT_HEX, &i);
	return i;
#else
	return atoi(pszStr);
#endif
}

UAPI(PSTR) UAStrFromInt(PSTR pszDst, INT iVal, INT iRadix UDEF(10))
{
	return _itoa(iVal, pszDst, iRadix);
}

UAPI(UINT) UAStrToHex(PBYTE pbDst, PCSTR pszSrc)
{
	PBYTE pbStart = pbDst;
	while (*pszSrc)
	{
		*pbDst++ = UAChrToHex(pszSrc);
		pszSrc += 2;
	}
	return (UINT) (pbDst - pbStart);
}

UAPI(UINT) UAStrFromHex(PSTR pszDst, PCBYTE pbSrc, UINT uSize)
{
	UINT i;
	for (i = 0; i < uSize; i++)
	{
		UAChrFromHex(&pszDst[i * 2], pbSrc[i]);
	}
	pszDst[uSize * 2] = 0;
	return uSize * 2;
}

#ifdef _WINSTR
#define UAStrPrint wsprintfA
#define UAStrPrintV wvsprintfA
#else
#define UAStrPrint sprintf
#define UAStrPrintV vsprintf
#endif

UAPI(UINT) UAStrLoad(UINT uID, PSTR pszStr, UINT uMax UDEF(MAX_STR))
{
	return LoadStringA(g_hInst, uID, pszStr, uMax);
}

UAPI(PCSTR) UAStrGet(UINT uID)
{
	STATIC CHAR s_szStr[MAX_STR];
	UAStrLoad(uID, s_szStr, MAX_STR);
	return s_szStr;
}
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* UNICODE String */
UAPI(PWSTR) UWStrEnd(PCWSTR pwzStr)
{
	while (*pwzStr)
	{
		pwzStr++;
	}
	return (PWSTR) pwzStr;
}

UAPI(UINT) UWStrLen(PCWSTR pwzStr)
{
	return (UINT) (UWStrEnd(pwzStr) - pwzStr);
}

UAPI(UINT) UWStrCopy(PWSTR pwzDst, PCWSTR pwzSrc)
{
	PWSTR p = pwzDst;
	while (*p++ = *pwzSrc++);
	return (UINT) (p - pwzDst);
}

UAPI(UINT) UWStrCopyN(PWSTR pwzDst, PCWSTR pwzSrc, UINT uDstLen)
{
	PWSTR p = pwzDst;
	while (*pwzSrc && ((INT) (--uDstLen) > 0))
	{
		*p++ = *pwzSrc++;
	}
	*p++ = 0;
	return (UINT) (p - pwzDst);
}

UAPI(UINT) UWStrCat(PWSTR pwzDst, PCWSTR pwzSrc)
{
	PWSTR p = UWStrEnd(pwzDst);
	return (UINT) (p - pwzDst) + UWStrCopy(p, pwzSrc);
}

UAPI(INT) UWStrCmp(PCWSTR pwzStr1, PCWSTR pwzStr2)
{
#ifdef _WINSTR
	return lstrcmpW(pwzStr1, pwzStr2);
#else
	return wcscmp(pwzStr1, pwzStr2);
#endif
}

UAPI(INT) UWStrCmpI(PCWSTR pwzStr1, PCWSTR pwzStr2)
{
#ifdef _WINSTR
	return lstrcmpiW(pwzStr1, pwzStr2);
#else
	return _wcsicmp(pwzStr1, pwzStr2);
#endif
}

UAPI(INT) UWStrCmpN(PCWSTR pwzStr1, PCWSTR pwzStr2, UINT uLen)
{
#ifdef _SHLSTR
	return StrCmpNW(pwzStr1, pwzStr2, uLen);
#else
	return wcsncmp(pwzStr1, pwzStr2, uLen);
#endif
}

UAPI(INT) UWStrCmpNI(PCWSTR pwzStr1, PCWSTR pwzStr2, UINT uLen)
{
#ifdef _SHLSTR
	return StrCmpNIW(pwzStr1, pwzStr2, uLen);
#else
	return _wcsnicmp(pwzStr1, pwzStr2, uLen);
#endif
}

UAPI(PWSTR) UWStrChr(PCWSTR pwzStr, WCHAR wChr)
{
#ifdef _SHLSTR
	return (PWSTR) StrChrW(pwzStr, wChr);
#else
	return (PWSTR) wcschr(pwzStr, wChr);
#endif
}

UAPI(PWSTR) UWStrRChr(PCWSTR pwzStr, WCHAR wChr)
{
#ifdef _SHLSTR
	return (PWSTR) StrRChrW(pwzStr, NULL, wChr);
#else
	return (PWSTR) wcsrchr(pwzStr, wChr);
#endif
}

UAPI(PWSTR) UWStrStr(PCWSTR pwzStr1, PCWSTR pwzStr2)
{
#ifdef _SHLSTR
	return (PWSTR) StrStrW(pwzStr1, pwzStr2);
#else
	return (PWSTR) wcsstr(pwzStr1, pwzStr2);
#endif
}

UAPI(PWSTR) UWStrStrI(PCWSTR pwzStr1, PCWSTR pwzStr2)
{
#ifdef _SHLSTR
	return (PWSTR) StrStrIW(pwzStr1, pwzStr2);
#else
	PWSTR p = (PWSTR) pwzStr1;
	while (*p)
	{
		PWSTR s1 = p;
		PWSTR s2 = (PWSTR) pwzStr2;

		while (*s1 && *s2 && UWChrEqualI((TCHAR) *s1, (TCHAR) *s2))
		{
			s1++;
			s2++;
		}

		if (*s2 == 0)
		{
			return p;
		}

		p++;
	}
	return NULL;
#endif
}

UAPI(PWSTR) UWStrRep(PWSTR pwzStr, WCHAR wFind UDEF('|'), WCHAR wRep UDEF(0))
{
	PWSTR p = pwzStr;
	for (; *p; p++)
	{
		if (*p == wFind)
		{
			*p = wRep;
		}
	}
	return pwzStr;
}

UAPI(PWSTR) UWStrTrim(PWSTR pwzStr, WCHAR wTrim UDEF('"'))
{
	if (*pwzStr == wTrim)
	{
		PWSTR p = pwzStr + UWStrLen(pwzStr) - 1;
		if (*p == wTrim)
		{
			*p = 0;
		}
		return pwzStr + 1;
	}
	return pwzStr;
}

UAPI(PWSTR) UWStrSplit(PWSTR pwzStr, WCHAR wSplit)
{
	while (*pwzStr)
	{
		if (*pwzStr == wSplit)
		{
			*pwzStr++ = 0;
			break;
		}
		pwzStr++;
	}
	return pwzStr;
}

UAPI(PWSTR) UWStrRSplit(PWSTR pwzStr, WCHAR wSplit)
{
	PWSTR p;
	PWSTR pwzEnd = UWStrEnd(pwzStr);
	for (p = pwzEnd; p >= pwzStr; p--)
	{
		if (*p == wSplit)
		{
			*p++ = 0;
			return p;
		}
	}
	return pwzEnd;
}

UAPI(UINT) UWStrEqual(PCWSTR pwzStr1, PCWSTR pwzStr2)
{
	UINT i = 0;
	while (pwzStr1[i] && (pwzStr1[i] == pwzStr2[i]))
	{
		i++;
	}
	return i;
}

UAPI(UINT) UWStrEqualI(PCWSTR pwzStr1, PCWSTR pwzStr2)
{
	UINT i = 0;
	while (pwzStr1[i] && UWChrEqualI(pwzStr1[i], pwzStr2[i]))
	{
		i++;
	}
	return i;
}

UAPI(BOOL) UWStrMatch(PCWSTR pwzStr, PCWSTR pwzPat)
{
	PCWSTR s, p;
	BOOL bStar = FALSE;

__LoopStart:
	for (s = pwzStr, p = pwzPat; *s; s++, p++)
	{
		switch (*p)
		{
		case '?':
			/*if (*s == '.') goto __StartCheck;*/
			break;

		case '*':
			bStar = TRUE;
			pwzStr = s, pwzPat = p;
			if (!*++pwzPat) return TRUE;
			goto __LoopStart;

		default:
			if (*s != *p)
			{
				/*__StartCheck:*/
				if (!bStar) return FALSE;
				pwzStr++;
				goto __LoopStart;
			}
			break;
		}
	}
	if (*p == '*') ++p;
	return (!*p);
}

UAPI(BOOL) UWStrMatchI(PCWSTR pwzStr, PCWSTR pwzPat)
{
	PCWSTR s, p;
	BOOL bStar = FALSE;

__LoopStart:
	for (s = pwzStr, p = pwzPat; *s; s++, p++)
	{
		switch (*p)
		{
		case '?':
			/*if (*s == '.') goto __StartCheck;*/
			break;

		case '*':
			bStar = TRUE;
			pwzStr = s, pwzPat = p;
			if (!*++pwzPat) return TRUE;
			goto __LoopStart;

		default:
			if (!UWChrEqualI(*s, *p))
			{
				/*__StartCheck:*/
				if (!bStar) return FALSE;
				pwzStr++;
				goto __LoopStart;
			}
			break;
		}
	}
	if (*p == '*') ++p;
	return (!*p);
}

UAPI(PWSTR) UWStrToUpper(PWSTR pwzStr)
{
#ifdef _WINSTR
	return CharUpperW(pwzStr);
#else
	return _wcsupr(pwzStr);
#endif
}

UAPI(PWSTR) UWStrToLower(PWSTR pwzStr)
{
#ifdef _WINSTR
	return CharLowerW(pwzStr);
#else
	return _wcslwr(pwzStr);
#endif
}

UAPI(INT) UWStrToInt(PCWSTR pwzStr)
{
#ifdef _SHLSTR
	INT i = 0;
	StrToIntExW(pwzStr, STIF_SUPPORT_HEX, &i);
	return i;
#else
	return _wtoi(pwzStr);
#endif
}

UAPI(PWSTR) UWStrFromInt(PWSTR pwzDst, INT iVal, INT iRadix UDEF(10))
{
	return _itow(iVal, pwzDst, 10);
}

UAPI(UINT) UWStrToHex(PBYTE pbDst, PCWSTR pwzSrc)
{
	PBYTE pbStart = pbDst;
	while (*pwzSrc)
	{
		*pbDst++ = UWChrToHex(pwzSrc);
		pwzSrc += 2;
	}
	return (UINT) (pbDst - pbStart);
}

UAPI(UINT) UWStrFromHex(PWSTR pwzDst, PCBYTE pbSrc, UINT uSize)
{
	UINT i;
	for (i = 0; i < uSize; i++)
	{
		UWChrFromHex(&pwzDst[i * 2], pbSrc[i]);
	}
	pwzDst[uSize * 2] = 0;
	return uSize * 2;
}

#ifdef _WINSTR
#define UWStrPrint wsprintfW
#define UWStrPrintV wvsprintfW
#else
#define UWStrPrint swprintf
#define UWStrPrintV vswprintf
#endif

UAPI(UINT) UWStrLoad(UINT uID, PWSTR pwzStr, UINT uMax UDEF(MAX_STR))
{
	return LoadStringW(g_hInst, uID, pwzStr, uMax);
}

UAPI(PCWSTR) UWStrGet(UINT uID)
{
#ifdef WINCE
	return (PCWSTR) UWStrLoad(uID, NULL, 0);
#else
	HRSRC hRsrc = FindResource(g_hInst, MAKEINTRESOURCE((uID / 16) + 1), RT_STRING);
	if (hRsrc)
	{
		PCWSTR pwzStr = (PCWSTR) LoadResource(g_hInst, hRsrc);
		if (pwzStr)
		{
			UINT i = uID % 16;
			while (i--)
			{
				pwzStr += *pwzStr + 1;
			}
			return pwzStr + 1;
		}
	}
	return NULL;
#endif
}
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* Transformable String */
#ifdef _UNICODE

#define UChrIsNum					UWChrIsNum
#define UChrIsAlpha					UWChrIsAlpha
#define UChrIsSymbol				UWChrIsSymbol
#define UChrIsPrintable				UWChrIsPrintable
#define UChrToLower					UWChrToLower
#define UChrToUpper					UWChrToUpper
#define UChrToHex					UWChrToHex
#define UChrFromHex					UWChrFromHex
#define UChrEqualI					UWChrEqualI

#define UStrEnd						UWStrEnd
#define UStrLen						UWStrLen
#define UStrCopy					UWStrCopy
#define UStrCopyN					UWStrCopyN
#define UStrCat						UWStrCat
#define UStrCmp						UWStrCmp
#define UStrCmpI					UWStrCmpI
#define UStrCmpN					UWStrCmpN
#define UStrCmpNI					UWStrCmpNI

#define UStrChr						UWStrChr
#define UStrRChr					UWStrRChr
#define UStrStr						UWStrStr
#define UStrStrI					UWStrStrI

#define UStrRep						UWStrRep
#define UStrTrim					UWStrTrim
#define UStrEqual					UWStrEqual
#define UStrEqualI					UWStrEqualI
#define UStrSplit					UWStrSplit
#define UStrRSplit					UWStrRSplit
#define UStrMatch					UWStrMatch
#define UStrMatchI					UWStrMatchI
#define UStrToUpper					UWStrToUpper
#define UStrToLower					UWStrToLower
#define UStrToInt					UWStrToInt
#define UStrFromInt					UWStrFromInt
#define UStrToHex					UWStrToHex
#define UStrFromHex					UWStrFromHex
#define UStrPrint					UWStrPrint
#define UStrPrintV					UWStrPrintV

#define UStrLoad					UWStrLoad
#define UStrGet						UWStrGet

#define UStrToAStr					UWStrToAStr
#define UStrToWStr					UWStrCopyN
#define UAStrToStr					UAStrToWStr
#define UWStrToStr					UWStrCopyN

#else

#define UChrIsNum					UAChrIsNum
#define UChrIsAlpha					UAChrIsAlpha
#define UChrIsSymbol				UAChrIsSymbol
#define UChrIsPrintable				UAChrIsPrintable
#define UChrToLower					UAChrToLower
#define UChrToUpper					UAChrToUpper
#define UChrToHex					UAChrToHex
#define UChrFromHex					UAChrFromHex
#define UChrEqualI					UAChrEqualI

#define UStrEnd						UAStrEnd
#define UStrLen						UAStrLen
#define UStrCopy					UAStrCopy
#define UStrCopyN					UAStrCopyN
#define UStrCat						UAStrCat
#define UStrCmp						UAStrCmp
#define UStrCmpI					UAStrCmpI
#define UStrCmpN					UAStrCmpN
#define UStrCmpNI					UAStrCmpNI

#define UStrChr						UAStrChr
#define UStrRChr					UAStrRChr
#define UStrStr						UAStrStr
#define UStrStrI					UAStrStrI

#define UStrRep						UAStrRep
#define UStrTrim					UAStrTrim
#define UStrEqual					UAStrEqual
#define UStrEqualI					UAStrEqualI
#define UStrSplit					UAStrSplit
#define UStrRSplit					UAStrRSplit
#define UStrMatch					UAStrMatch
#define UStrMatchI					UAStrMatchI
#define UStrToUpper					UAStrToUpper
#define UStrToLower					UAStrToLower
#define UStrToInt					UAStrToInt
#define UStrFromInt					UAStrFromInt
#define UStrToHex					UAStrToHex
#define UStrFromHex					UAStrFromHex
#define UStrPrint					UAStrPrint
#define UStrPrintV					UAStrPrintV

#define UStrLoad					UAStrLoad
#define UStrGet						UAStrGet

#define UStrToAStr					UAStrCopyN
#define UStrToWStr					UAStrToWStr
#define UAStrToStr					UAStrCopyN
#define UWStrToStr					UWStrToAStr

#endif
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* File */
UAPI(HANDLE) UFileOpen(PCTSTR ptzPath, DWORD dwAccess UDEF(UFILE_READ))
{
	DWORD dwCreate = (dwAccess == UFILE_WRITE) ? CREATE_ALWAYS :
		((dwAccess == UFILE_READWRITE) ? OPEN_ALWAYS : OPEN_EXISTING);
	HANDLE hFile = CreateFile(ptzPath, dwAccess, FILE_SHARE_READ, NULL, dwCreate, 0, NULL);
	return (hFile == INVALID_HANDLE_VALUE) ? NULL : hFile;
}

UAPI(BOOL) UFileClose(HANDLE hFile)
{
	return CloseHandle(hFile);
}

UAPI(UINT) UFileRead(HANDLE hFile, PVOID pvData, UINT uSize)
{
	DWORD dwRead = 0;
	ReadFile(hFile, pvData, uSize, &dwRead, NULL);
	return dwRead;
}

UAPI(UINT) UFileWrite(HANDLE hFile, PCVOID pvData, UINT uSize)
{
	DWORD dwWrite = 0;
	WriteFile(hFile, pvData, uSize, &dwWrite, NULL);
	return dwWrite;
}

UAPI(UINT) UFileSeek(HANDLE hFile, INT iOffset, DWORD dwOrigin UDEF(UFILE_BEGIN))
{
	return SetFilePointer(hFile, iOffset, NULL, dwOrigin);
}

UAPI(UINT) UFileTell(HANDLE hFile)
{
	return UFileSeek(hFile, 0, UFILE_CURRENT);
}

UAPI(UINT) UFileGetSize(HANDLE hFile)
{
	DWORD dwSize = GetFileSize(hFile, NULL);
	return (dwSize == INVALID_FILE_SIZE) ? 0 : dwSize;
}

UAPI(BOOL) UFileSetSize(HANDLE hFile, UINT uSize UDEF(0))
{
	UFileSeek(hFile, uSize, UFILE_BEGIN);
	return SetEndOfFile(hFile);
}

UAPI(PTSTR) UFileToStr(PCTSTR ptzPath, PUINT puSize UDEF(NULL))
{
	UINT uSize;
	PTSTR ptzData;
	HANDLE hFile = UFileOpen(ptzPath, UFILE_READ);
	if (hFile == NULL)
	{
		return NULL;
	}

	uSize = UFileGetSize(hFile);
	if (puSize && (uSize > *puSize))
	{
		uSize = *puSize;
	}
	ptzData = (PTSTR) UMemAlloc(uSize + 16);
	if (ptzData)
	{
		UINT uRead = UFileRead(hFile, ptzData, 2);
		if (uRead == 2)
		{
			WORD wBom = *((PWORD) ptzData);
			if (wBom == 0xFEFF)
			{
				uRead = 0;
			}
			else if (wBom == 0xBBEF)
			{
				uRead = 0;
				UFileRead(hFile, ptzData, 1);
			}
			uRead += UFileRead(hFile, (PBYTE) ptzData + uRead, uSize - 2);

#ifdef _UNICODE
			if ((uRead >= 2) && (wBom != 0xFEFF) && (((PBYTE) ptzData)[1] != 0))
			{
				PSTR pszTemp = (PSTR) ptzData;
				ptzData = (PTSTR) UMemAlloc((uRead + 16) * sizeof(WCHAR));
				if (ptzData)
				{
					uRead = sizeof(WCHAR) * UAStrToWStr(ptzData, pszTemp, uRead + 16, uRead, (wBom == 0xBBEF) ? UCP_UTF8 : UCP_ANSI);
				}
				UMemFree(pszTemp);
			}
#else
			if ((uRead >= 2) && ((wBom == 0xFEFF) || (wBom == 0xBBEF) || ((PBYTE) ptzData)[1] == 0))
			{
				PWSTR pwzTemp = (PWSTR) ptzData;
				ptzData = (PTSTR) UMemAlloc((uRead + 16) * sizeof(WCHAR));
				if (ptzData)
				{
					if (wBom == 0xBBEF)
					{
						PSTR pszUTF8 = (PSTR) pwzTemp;
						pwzTemp = (PWSTR) ptzData;
						ptzData = (PTSTR) pszUTF8;
						uRead = sizeof(WCHAR) * UAStrToWStr(pwzTemp, pszUTF8, uRead, uRead, UCP_UTF8);
					}
					uRead = sizeof(CHAR) * UWStrToAStr(ptzData, pwzTemp, uSize, uRead / sizeof(WCHAR), UCP_ANSI);
				}
				UMemFree(pwzTemp);
			}
#endif
		}

		if (ptzData)
		{
			if (puSize)
			{
				*puSize = uRead;
			}
			((PBYTE) ptzData)[uRead] = 0;
			((PBYTE) ptzData)[uRead + 1] = 0;
			((PBYTE) ptzData)[uRead + 2] = 0;
		}
	}
	UFileClose(hFile);
	return ptzData;
}

UAPI(PVOID) UFileLoad(PCTSTR ptzPath, PUINT puSize UDEF(NULL), PVOID pvData UDEF(NULL))
{
	UINT uSize;
	HANDLE hFile = UFileOpen(ptzPath, UFILE_READ);
	if (hFile == NULL)
	{
		return NULL;
	}

	uSize = UFileGetSize(hFile);
	if (puSize && (uSize > *puSize))
	{
		uSize = *puSize;
	}

	if (pvData == NULL)
	{
		pvData = (PBYTE) UMemAlloc(uSize + 16);
	}

	if (pvData)
	{
		uSize = UFileRead(hFile, pvData, uSize);
		((PBYTE) pvData)[uSize] = 0;
		((PBYTE) pvData)[uSize + 1] = 0;
		if (puSize)
		{
			*puSize = uSize;
		}
	}

	UFileClose(hFile);
	return pvData;
}

UAPI(UINT) UFileSave(PCTSTR ptzPath, PCVOID pvData, UINT uSize, BOOL fAppend UDEF(FALSE))
{
	UINT uRet;
	HANDLE hFile = UFileOpen(ptzPath, fAppend ? UFILE_READWRITE : UFILE_WRITE);
	if (hFile == NULL)
	{
		return 0;
	}

	if (fAppend)
	{
		UFileSeek(hFile, 0, UFILE_END);
	}
	uRet = UFileWrite(hFile, pvData, uSize);
	UFileClose(hFile);
	return uRet;
}
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* File Management */
UAPI(UINT) UPathMake(PTSTR ptzDir, PCTSTR ptzSub)
{
	PTSTR p = UStrEnd(ptzDir);
	if (p[-1] != '\\')
	{
		*p++ = '\\';
	}
	return (UINT) (p - ptzDir) + UStrCopy(p, ptzSub);
}

UAPI(PTSTR) UPathSplit(PTSTR* pptzPath)
{
	PTSTR p;
	PTSTR ptzEnd = UStrEnd(*pptzPath);
	for (p = ptzEnd; p >= *pptzPath; p--)
	{
		if (*p == '\\')
		{
			*p++ = 0;
			return p;
		}
	}
	p = *pptzPath;
	*pptzPath = ptzEnd;
	return p;
}

UAPI(BOOL) UFileDelete(PCTSTR ptzPath)
{
	return DeleteFile(ptzPath);
}

UAPI(BOOL) UFileCopy(PCTSTR ptzPath, PCTSTR ptzNewPath)
{
	return CopyFile(ptzPath, ptzNewPath, FALSE);
}

UAPI(BOOL) UFileMove(PCTSTR ptzPath, PCTSTR ptzNewPath)
{
	return MoveFile(ptzPath, ptzNewPath);
}

typedef HRESULT (UCALLBACK *UFOUNDFILEPROC)(PCVOID pvParam, PCTSTR ptzDir, CONST PWIN32_FIND_DATA pFind);
UAPI(HRESULT) UFileFind(UFOUNDFILEPROC pFoundFile, PCVOID pvParam, PCTSTR ptzDir UDEF(TEXT("")), PCTSTR ptzWildcard UDEF(TEXT("*")))
{
	HANDLE hFind;
	WIN32_FIND_DATA fd;
	TCHAR tzPath[MAX_PATH];

	HRESULT hResult = ERROR_FILE_NOT_FOUND;
	UStrPrint(tzPath, TEXT("%s\\%s"), ptzDir, ptzWildcard);
	hFind = FindFirstFile(tzPath, &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			hResult = pFoundFile(pvParam, ptzDir, &fd);
		}
		while ((hResult != E_ABORT) && FindNextFile(hFind, &fd));
		FindClose(hFind);
	}

	return hResult;
}

UAPI(BOOL) UFileNameValid(PCTSTR ptzName)
{
	UINT i;
	PCTSTR p;
	CONST STATIC TCHAR c_tzInvalidChar[] = TEXT("\\/:*?\"<>|");
	for (p = ptzName; *p; p++)
	{
		for (i = 0; i < _NumOf(c_tzInvalidChar) - 1; i++)
		{
			if (*p == c_tzInvalidChar[i])
			{
				return FALSE;
			}
		}
	}
	return (*ptzName != 0);
}

UAPI(BOOL) UFileExist(PCTSTR ptzPath)
{
	WIN32_FILE_ATTRIBUTE_DATA a;
	return GetFileAttributesEx(ptzPath, GetFileExInfoStandard, &a) && !(a.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

UAPI(BOOL) UDirExist(PCTSTR ptzDir)
{
	WIN32_FILE_ATTRIBUTE_DATA a;
	return GetFileAttributesEx(ptzDir, GetFileExInfoStandard, &a) && (a.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

UAPI(BOOL) UDirCreate(PCTSTR ptzDir)
{
	PTSTR p;
	for (p = (PTSTR) ptzDir; p = UStrChr(p, '\\'); *p++ = '\\')
	{
		*p = 0;
		if (!UDirExist(ptzDir))
		{
			CreateDirectory(ptzDir, NULL);
		}
	}
	return TRUE;
}

UAPI(BOOL) UDirDelete(PCTSTR ptzDir)
{
	return RemoveDirectory(ptzDir);
}

UAPI(UINT) UDirGetAppPath(PTSTR ptzPath)
{
	return GetModuleFileName(g_hInst, ptzPath, MAX_PATH);
}

UAPI(UINT) UDirGetAppFile(PTSTR ptzPath, PCTSTR ptzFile)
{
	PTSTR p = ptzPath + UDirGetAppPath(ptzPath);
	for (; p >= ptzPath; p--)
	{
		if (*p == '\\')
		{
			p++;
			break;
		}
	}
	return (UINT) (p - ptzPath) + UStrCopy(p, ptzFile);
}

UAPI(UINT) UDirGetAppExt(PTSTR ptzPath, PCTSTR ptzExt)
{
	PTSTR p;
	PTSTR ptzEnd = ptzPath + UDirGetAppPath(ptzPath);
	for (p = ptzEnd; p >= ptzPath; p--)
	{
		if (*p == '.')
		{
			p++;
			break;
		}
	}
	if (p == ptzPath)
	{
		p = ptzEnd;
		*p++ = '.';
	}
	return (UINT) (p - ptzPath) + UStrCopy(p, ptzExt);
}

UAPI(UINT) UDirGetCurrent(PTSTR ptzDir)
{
#ifdef GetCurrentDirectory
	return GetCurrentDirectory(MAX_PATH, ptzDir);
#else
	UDirGetAppPath(ptzDir);
	return (UINT) (UPathSplit(&ptzDir) - ptzDir - 1);
#endif
}

UAPI(UINT) UDirGetTemp(PTSTR ptzDir)
{
	return GetTempPath(MAX_PATH, ptzDir);
}

UAPI(UINT) UFileGetTemp(PTSTR ptzPath)
{
	UDirGetTemp(ptzPath);
	return GetTempFileName(ptzPath, TEXT("UNI"), 0, ptzPath);
}
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* Thread */
typedef DWORD (UCALLBACK* UPROC)(PVOID pvParam);
UAPI(HANDLE) UThreadCreate(UPROC upProc, PVOID pvParam UDEF(NULL))
{
	DWORD dwThread;
	return CreateThread(NULL, 0, upProc, pvParam, 0, &dwThread);
}

UAPI(BOOL) UThreadClose(HANDLE hThread)
{
	return CloseHandle(hThread);
}

UAPI(BOOL) UThreadCall(UPROC upProc, PVOID pvParam UDEF(NULL))
{
	return UThreadClose(UThreadCreate(upProc, pvParam));
}

UAPI(BOOL) UThreadTerminate(HANDLE hThread)
{
	return TerminateThread(hThread, 0);
}

UAPI(BOOL) UThreadSuspend(HANDLE hThread)
{
	return SuspendThread(hThread);
}

UAPI(BOOL) UThreadResume(HANDLE hThread)
{
	return ResumeThread(hThread);
}
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* Misc */
UAPI(UINT) UGetRandom()
{
	return rand();
}

typedef SYSTEMTIME UTIME, *PUTIME;
UAPI(VOID) UGetTime(PUTIME pTime)
{
	GetLocalTime(pTime);
}

UAPI(UINT) UGetTimeStamp()
{
	return GetTickCount();
}

UAPI(VOID) UTrace(PCTSTR ptzFormat, ...)
{
	DWORD i;
	va_list va;
	TCHAR tz[MAX_STR];

#ifdef _TRACE_TIME
	UTIME ut;
	UGetTime(&ut);
	i = UStrPrint(tz, TEXT("%02u:%02u:%02u\t"), (UINT) ut.wHour, (UINT) ut.wMinute, (UINT) ut.wSecond);
#else
	i = 0;
#endif

	va_start(va, ptzFormat);
	i += UStrPrintV(tz + i, ptzFormat, va);
	va_end(va);

	tz[i++] = '\r';
	tz[i++] = '\n';
	tz[i] = 0;

#if defined(_TRACE_TO_FILE)
	UFileSave(TEXT("UniDebug.txt"), tz, i * sizeof(TCHAR), TRUE);
#elif defined(_TRACE_TO_CONSOLE)
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), tz, i, &i, NULL);
#else
	OutputDebugString(tz);
#endif
}

UAPI(VOID) UAssert(PCTSTR ptzExp, PCTSTR ptzFile, UINT uLine)
{
	UINT i;
	TCHAR tzText[MAX_STR];
	TCHAR tzModule[MAX_PATH];

	UDirGetAppPath(tzModule);
	UStrPrint(tzText,
		TEXT("Assertion failed!\n\n")
		TEXT("Program: %s\n")
		TEXT("File: %s\n")
		TEXT("Line: %d\n\n")
		TEXT("Expression: %s\n\n")
		TEXT("Press Retry to debug the application - JIT must be enabled"),
		tzModule, ptzFile, uLine, ptzExp);

	i = MessageBox(NULL, tzText, TEXT("UniDebug"), MB_ICONERROR | MB_ABORTRETRYIGNORE);
#ifdef WIN32_PLATFORM_WFSP
	if (i == 0)
	{
		i = MessageBox(NULL, tzText, TEXT("UniDebug"), MB_ICONERROR | MB_RETRYCANCEL);
	}
#endif
	if (i == IDABORT)
	{
		ExitProcess(0);
	}
	else if (i == IDRETRY)
	{
		DebugBreak();
	}
}
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* UAutoBase */
#ifdef __cplusplus
template<typename M> class UAutoBase
{
protected:
	M m;

public:
	template<typename T> operator T()
	{
		return (T) m;
	}

	template<typename T> M operator +(T t)
	{
		return m + t;
	}

	template<typename T> M operator -(T t)
	{
		return m - t;
	}

	template<typename T> M operator ++()
	{
		return m++;
	}

	template<typename T> M operator --()
	{
		return m--;
	}

	BOOL operator ==(M t)
	{
		return m == t;
	}

	M operator =(M t)
	{
		return m = t;
	}

	BOOL operator !()
	{
		return !m;
	}
};

/* UAutoMem */
class UAutoMem: public UAutoBase<PBYTE>
{
public:
	PBYTE operator =(PVOID pvMem)
	{
		return m = (PBYTE) pvMem;
	}

	UAutoMem(PVOID pvMem = NULL)
	{
		m = (PBYTE) pvMem;
	}

	UAutoMem(UINT uSize)
	{
		m = (PBYTE) UMemAlloc(uSize);
	}

	~UAutoMem()
	{
		if (m)
		{
			UMemFree(m);
		}
	}
};

/* UAutoHandle */
class UAutoHandle: public UAutoBase<HANDLE>
{
public:
	HANDLE operator =(HANDLE hHaldle)
	{
		return m = hHaldle;
	}

	UAutoHandle(HANDLE hHaldle = NULL)
	{
		m = hHaldle;
	}

	~UAutoHandle()
	{
		if (m)
		{
			CloseHandle(m);
		}
	}
};

/* UAutoPtr */
template<class M> class UAutoPtr
{
protected:
	M* m;

public:
	M* operator =(M* t)
	{
		return m = t;
	}

	UAutoPtr(M* t = NULL)
	{
		m = t;
	}

	~UAutoPtr()
	{
		if (m)
		{
			m->Release();
		}
	}

	template<class T> operator T*()
	{
		T* p;
		m->QueryInterface(__uuidof(T), (PVOID*) &p);
		return p;
	}
};
#endif
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* UMemFile */
#ifdef __cplusplus
#ifdef WINCE
#define _CreateFileForMapping CreateFileForMapping
#else
#define _CreateFileForMapping CreateFile
#endif
class UMemFile
{
protected:
	HANDLE m_hFile;
	HANDLE m_hMap;
	PBYTE m_pbFile;
	DWORD m_dwSize;

public:
	BOOL operator !() {return !m_pbFile;}
	operator DWORD() CONST {return m_dwSize;}
	operator PVOID() CONST {return m_pbFile;}
	operator PBYTE() CONST {return m_pbFile;}
	operator PCHAR() CONST {return (PCHAR) m_pbFile;}
	operator PWCHAR() CONST {return (PWCHAR) m_pbFile;}

public:
	UMemFile(PCTSTR ptzPath, BOOL bRead = TRUE, DWORD dwSize = 0)
	{
		m_dwSize = 0;
		m_pbFile = NULL;
		m_hMap = NULL;
		m_hFile = _CreateFileForMapping(ptzPath,
			(bRead ? GENERIC_READ : (GENERIC_READ | GENERIC_WRITE)), FILE_SHARE_READ, NULL,
			((bRead || (dwSize == 0)) ? OPEN_EXISTING : OPEN_ALWAYS), FILE_ATTRIBUTE_NORMAL, NULL);
		if (m_hFile != INVALID_HANDLE_VALUE)
		{
			m_dwSize = dwSize ? dwSize : GetFileSize(m_hFile, NULL);
			if (m_dwSize)
			{
				m_hMap = CreateFileMapping(m_hFile, NULL, bRead ? PAGE_READONLY : PAGE_READWRITE, 0, m_dwSize, NULL);
				if (m_hMap)
				{
					m_pbFile = (PBYTE) MapViewOfFile(m_hMap, bRead ? FILE_MAP_READ : FILE_MAP_ALL_ACCESS, 0, 0, m_dwSize);
				}
			}
		}
	}

	~UMemFile()
	{
		if (m_pbFile) UnmapViewOfFile(m_pbFile);
		if (m_hMap) CloseHandle(m_hMap);
		if (m_hFile != INVALID_HANDLE_VALUE) CloseHandle(m_hFile);
	}
};
#endif
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* UThunk */
#ifdef __cplusplus
#pragma pack(push, 1)
#if defined(_M_IX86)
template<typename TPARAM> struct UThunk
{
	DWORD m_dwMovEaxEsp4;	// MOV EAX, [ESP + 4]
	WORD m_wMovParamEax;	// MOV DS:[m_tParam], EAX
	TPARAM* m_ptParam;

	DWORD m_dwMovEsp4;		// MOV [ESP + 4], pThis
	PVOID m_pThis;
	BYTE m_bJump;			// JMP pProc
	DWORD m_dwDelta;

	TPARAM m_tParam;
	operator TPARAM() {return m_tParam;}
	TPARAM operator =(TPARAM tParam) {return m_tParam = tParam;}

	template<typename TPROC> PROC Init(PVOID pThis, TPROC tProc)
	{
		m_dwMovEaxEsp4 = 0x0424448B;
		m_wMovParamEax = 0xA33E;
		m_ptParam = &m_tParam;
		m_dwMovEsp4 = 0x042444C7;
		m_pThis = pThis;
		m_bJump = 0xE9;
		m_dwDelta = (DWORD) (*((INT_PTR*) &tProc) - ((INT_PTR) &m_dwDelta + sizeof(m_dwDelta)));
		FlushInstructionCache(GetCurrentProcess(), this, sizeof(UThunk));
		m_tParam = (TPARAM) 0;
		return (PROC) this;
	}
};
#elif defined(_ARM_)
template<typename TPARAM> struct UThunk
{
	DWORD m_dwStrR0Param;	// STR R0, [PC,#12]
	DWORD m_dwMovR0This;	// LDR R0, [PC]
	DWORD m_dwMovPcProc;	// LDR PC, [PC]
	PVOID m_pThis;
	PVOID m_pProc;

	TPARAM m_tParam;
	operator TPARAM() {return m_tParam;}
	TPARAM operator =(TPARAM tParam) {return m_tParam = tParam;}

	template<typename TPROC> PROC Init(PVOID pThis, TPROC tProc)
	{
		m_dwStrR0Param = 0xE58F000C;
		m_dwMovR0This = 0xE59F0000;
		m_dwMovPcProc = 0xE59FF000;
		m_pThis = pThis;
		m_pProc = *((PVOID*) &tProc);
		FlushInstructionCache(GetCurrentProcess(), this, sizeof(UThunk));
		m_tParam = (TPARAM) 0;
		return (PROC) this;
	}
};
#else
#error UThunk does not support this platform!
#endif
#pragma pack(pop)
#endif
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* UAutoTrace */
#ifdef __cplusplus
class UAutoTrace
{
private:
	UINT m_uLine;
	PCTSTR m_ptzName;

public:
	UAutoTrace(PCTSTR ptzName, UINT uLine): m_uLine(uLine), m_ptzName(ptzName)
	{
		UTrace(TEXT("Enter %s:%u"), ptzName, uLine);
	}

	~UAutoTrace()
	{
		UTrace(TEXT("Leave %s:%u"), m_ptzName, m_uLine);
	}
};
#endif
/**********************************************************************************************************************/



/**********************************************************************************************************************/
/* Debug */
#ifdef __FUNCTION__
#define __FUNCFILE__				TEXT(__FUNCTION__)
#else
#define __FUNCFILE__				TEXT(__FILE__)
#endif

#ifdef _TRACE
#define _Trace						UTrace
#define _LineTrace()				UTrace(TEXT("Trace %s:%u"), __FUNCFILE__, __LINE__)
#ifdef __cplusplus
#define _AutoTrace()				UAutoTrace at(__FUNCFILE__, __LINE__)
#else
#define _AutoTrace()				_LineTrace()
#endif
#else
#define _Trace
#define _LineTrace()
#define _AutoTrace()
#endif

#ifdef _DEBUG
#define _Assert(e)					(VOID) ((e) || (UAssert(TEXT(#e), TEXT(__FILE__), __LINE__), 0))
#define _Verify(e)					_Assert(e)
#else
#define _Assert(e)					((VOID) 0)
#define _Verify(e)					((VOID) (e))
#endif
/**********************************************************************************************************************/
