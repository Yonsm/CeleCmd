


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CeleCtrl 1.6.118
// Copyright (C) Yonsm 2008, All Rights Reserved.
#pragma once
#include "AppDef.h"
#include "UniBase.h"
#include "CeleUtil.h"

#include <CommCtrl.h>
#include <AygShell.h>
#include <TpcShell.h>

#include "Resource.h"
//#define IDI_Main			100
//#define IDR_DlgMenu		200
//#define IDS_AppName		800
//#define IDS_AppDesc		801
//#define IDS_Copyright		802
//#define IDS_CmdMsg		803
//#define IDS_CmdErr		804
//#define IDS_CmdAsk		805

//#define IDS_Open			900
//#define IDS_Save			901
//#define IDS_PromptFile	902

#define IDC_ListView		43295
#define IDC_StatusBar		43296
#define IDC_ClientView		43297

#ifndef DT_WORD_ELLIPSIS
#define DT_WORD_ELLIPSIS	0x00040000
#endif

#define SWP_SHOW			(SWP_NOSIZE | SWP_NOMOVE | SWP_NOREPOSITION | SWP_SHOWWINDOW)
#define SWP_HIDE			(SWP_NOSIZE | SWP_NOMOVE | SWP_NOREPOSITION | SWP_HIDEWINDOW)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CeleMenu
class CeleMenu
{
protected:
	HMENU m_hMenu;

public:
	CeleMenu(HMENU hMenu = NULL)
	{
		m_hMenu = hMenu;
	}

	HMENU operator =(HMENU hMenu)
	{
		return m_hMenu = hMenu;
	}

	operator HMENU()
	{
		return m_hMenu;
	}

	BOOL Enable(UINT uCmd, BOOL bEnable = TRUE)
	{
		return EnableMenuItem(m_hMenu, uCmd, bEnable ? MF_ENABLED : MF_GRAYED);
	}

	BOOL Check(UINT uCmd, BOOL bChecked = TRUE)
	{
		return CheckMenuItem(m_hMenu, uCmd, bChecked ? MF_CHECKED : MF_UNCHECKED);
	}

	BOOL CheckRadio(UINT uFirst, UINT uLast, UINT uCmd)
	{
		return CheckMenuRadioItem(m_hMenu, uFirst, uLast, uCmd, MF_BYCOMMAND);
	}

	BOOL IsChecked(UINT uCmd)
	{
		MENUITEMINFO mi;
		mi.cbSize = sizeof(MENUITEMINFO);
		mi.fMask = MIIM_STATE;
		return GetMenuItemInfo(m_hMenu, uCmd, FALSE, &mi) && (mi.fState & MFS_CHECKED);
	}

	BOOL IsExist(UINT uCmd)
	{
		MENUITEMINFO mi;
		mi.cbSize = sizeof(MENUITEMINFO);
		mi.fMask = 0;
		return GetMenuItemInfo(m_hMenu, uCmd, FALSE, &mi);
	}

	UINT GetItemCount()
	{
		UINT nCount = 0;
		MENUITEMINFO mi;
		mi.cbSize = sizeof(MENUITEMINFO);
		mi.fMask = 0;
		while (GetMenuItemInfo(m_hMenu, nCount, TRUE, &mi))
		{
			nCount++;
		}
		return nCount;
	}

	BOOL GetText(UINT uCmd, PTSTR ptzStr)
	{
		MENUITEMINFO mi;
		mi.cbSize = sizeof(MENUITEMINFO);
		mi.fMask = MIIM_TYPE;
		mi.cch = MAX_NAME;
		mi.dwTypeData = ptzStr;
		return GetMenuItemInfo(m_hMenu, uCmd, FALSE, &mi);
	}

	BOOL SetText(UINT uCmd, PCTSTR ptzStr)
	{
		MENUITEMINFO mi;
		mi.cbSize = sizeof(MENUITEMINFO);
		mi.fMask = MIIM_TYPE;
		mi.fType = MFT_STRING;
		mi.dwTypeData = (PTSTR) ptzStr;
		return SetMenuItemInfo(m_hMenu, uCmd, FALSE, &mi);
	}


	BOOL Popup(HWND hParent, INT iLeft, INT iTop, UINT uFlags = 0)
	{
		return TrackPopupMenuEx(m_hMenu, uFlags, iLeft, iTop, hParent, NULL);
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CeleMenuBar
class CeleMenuBar
{
protected:
	HWND m_hMenuBar;

public:
	CeleMenuBar(HWND hMenuBar = NULL)
	{
		m_hMenuBar = hMenuBar;
	}

	operator HWND()
	{
		return m_hMenuBar;
	}

	HWND operator =(HWND hMenuBar)
	{
		return m_hMenuBar = hMenuBar;
	}

	HWND Create(HWND hParent, UINT uBarRes = 0, DWORD dwFlags = 0)
	{
		SHMENUBARINFO mb = {0};
		mb.cbSize = sizeof(SHMENUBARINFO);
		mb.hwndParent = hParent;
		mb.hInstRes = g_hInst;
		mb.nToolBarId = uBarRes;
		mb.dwFlags = dwFlags;
		SHCreateMenuBar(&mb);
		_Assert(mb.hwndMB);
		return m_hMenuBar = mb.hwndMB;
	}

	HMENU GetMenu()
	{
		// Return NULL on PPC 5/6
		return (HMENU) SendMessage(m_hMenuBar, SHCMBM_GETMENU, 0, 0);
	}

	HMENU GetMenu(UINT uCmd)
	{
		return (HMENU) SendMessage(m_hMenuBar, SHCMBM_GETSUBMENU, 0, uCmd);
	}

	HMENU GetButtonMenu(UINT uIndex = 0)
	{
		TBBUTTON tb;
		tb.dwData = NULL;
		SendMessage(m_hMenuBar, TB_GETBUTTON, uIndex, (LPARAM) &tb);
		return tb.dwData ? (HMENU) tb.dwData : GetSubMenu(GetMenu(), uIndex);
	}

	DWORD OverrideKey(WPARAM wParam = VK_TBACK)
	{
		return SendMessage(m_hMenuBar, SHCMBM_OVERRIDEKEY, wParam, MAKELPARAM(SHMBOF_NODEFAULT | SHMBOF_NOTIFY, SHMBOF_NODEFAULT | SHMBOF_NOTIFY));
	}

	BOOL SetButtonText(UINT uCmd, PTSTR ptzStr)
	{
		TBBUTTONINFO tb;
		tb.cbSize = sizeof(TBBUTTONINFO);
		tb.dwMask = TBIF_TEXT;
		tb.pszText = ptzStr;
		return SendMessage(m_hMenuBar, TB_SETBUTTONINFO, uCmd, (LPARAM) &tb);
	}

	BOOL SetButtonState(UINT uCmd, BYTE bState = TBSTATE_ENABLED)
	{
		TBBUTTONINFO tb;
		tb.cbSize = sizeof(TBBUTTONINFO);
		tb.dwMask = TBIF_STATE;
		tb.fsState = bState;
		return SendMessage(m_hMenuBar, TB_SETBUTTONINFO, uCmd, (LPARAM) &tb);
	}

	BOOL EnableButton(UINT uCmd, BOOL bEnable = TRUE)
	{
#ifdef WIN32_PLATFORM_WFSP
		return SetButtonState(uCmd, bEnable ? TBSTATE_ENABLED : 0);
#else
		return SendMessage(m_hMenuBar, TB_ENABLEBUTTON, uCmd, bEnable);
#endif
	}

	BOOL HideButton(UINT uCmd, BOOL bHide = TRUE)
	{
#ifdef WIN32_PLATFORM_WFSP
		return SetButtonState(uCmd, bHide ? TBSTATE_HIDDEN : 0);
#else
		return SendMessage(m_hMenuBar, TB_HIDEBUTTON, uCmd, bHide);
#endif
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CeleListBox
class CeleListBox
{
protected:
	HWND m_hListBox;

public:
	CeleListBox(HWND hListBox)
	{
		m_hListBox = hListBox;
	}

	operator HWND()
	{
		return m_hListBox;
	}

	HWND operator =(HWND hListBox)
	{
		return m_hListBox = hListBox;
	}

public:
	INT InitList(HWND hWnd, UINT uCtrlID, PTSTR ptzStr, UINT iSel = 0)
	{
		for (PTSTR p = ptzStr, q = ptzStr; *q; q++)
		{
			if (*q == '|')
			{
				*q = 0;
				SendMessage(m_hListBox, LB_ADDSTRING, 0, (LPARAM) p);
				p = q + 1;
			}
		}

		return SendMessage(m_hListBox, LB_SETCURSEL, iSel, 0);
	}

	INT InitList(HWND hWnd, UINT uCtrlID, UINT uStr, UINT iSel = 0)
	{
		TCHAR tzStr[MAX_STR];
		UStrLoad(uStr, tzStr);
		return InitList(hWnd, uCtrlID, tzStr, iSel);
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CeleListView
#ifndef LVS_EX_TRANSPARENTBKGND
#define LVS_EX_TRANSPARENTBKGND		0x00400000
#endif
#define LVS_DEFAULT					(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | WS_TABSTOP)
#define LVS_EX_DEFAULT				(LVS_EX_GRADIENT | LVS_EX_NOHSCROLL | LVS_EX_FULLROWSELECT/* | LVS_EX_ONECLICKACTIVATE*/)
class CeleListView
{
protected:
	UINT m_uColRes;
	HWND m_hListView;

public:
	operator HWND()
	{
		return m_hListView;
	}

	HWND operator =(HWND hListView)
	{
		return m_hListView = hListView;
	}

	HWND Create(HWND hParent, DWORD dwStyle = LVS_DEFAULT, DWORD dwExStyle = LVS_EX_DEFAULT)
	{
		m_hListView = CreateWindow(WC_LISTVIEW, NULL, dwStyle, 0, 0, 0, 0, hParent, (HMENU) IDC_ListView, g_hInst, NULL);
		SetExStyle(dwExStyle);
		return m_hListView;
	}

	HWND Create(UINT uColRes, HWND hParent, DWORD dwStyle = LVS_DEFAULT, DWORD dwExStyle = LVS_EX_DEFAULT)
	{
		Create(hParent, dwStyle, dwExStyle);
		SetColText(uColRes);
		return m_hListView;
	}

	VOID SetColText(UINT uColRes)
	{
		INT i = 0;
		LVCOLUMN lvc;
		TCHAR tzStr[MAX_STR];
		lvc.mask = LVCF_TEXT;
		UStrLoad(uColRes, tzStr);
		for (PTSTR p = tzStr, q = tzStr; *q; q++)
		{
			if (*q == '|')
			{
				*q = 0;
				lvc.pszText = UStrChr(p, ',');
				lvc.pszText = lvc.pszText ? (lvc.pszText + 1) : p;
				ListView_InsertColumn(m_hListView, i++, &lvc);
				p = q + 1;
			}
		}
		m_uColRes = uColRes;
	}

	VOID SetColWidth(INT iWidth)
	{
		INT i = 0;
		TCHAR tzStr[MAX_STR];
		UStrLoad(m_uColRes, tzStr);
		for (PTSTR p = tzStr, q = tzStr; *q; q++)
		{
			if (*q == '|')
			{
				*q = 0;
				INT w = UStrToInt(p);
				p = UStrChr(p, ',');
				if (p && (p[-1] == '%'))
				{
					w = (w * iWidth) / 100;
				}
				ListView_SetColumnWidth(m_hListView, i++, w);
				p = q + 1;
			}
		}
	}

	BOOL Resize(INT iLeft, INT iTop, INT iWidth, INT iHeight)
	{
		SetColWidth(iWidth);
		return MoveWindow(m_hListView, iLeft, iTop, iWidth, iHeight, FALSE);
	}

	INT GetSel(INT iIndex = -1)
	{
		return ListView_GetNextItem(m_hListView, iIndex, LVIS_SELECTED);
	}

	INT GetCur()
	{
		return ListView_GetNextItem(m_hListView, -1, LVIS_FOCUSED | LVIS_SELECTED);
	}

	VOID SetCur(INT i = 0)
	{
		ListView_SetItemState(m_hListView, i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
		ListView_EnsureVisible(m_hListView, i, FALSE);
	}

	VOID SetExStyle(DWORD dwExStyle = LVS_EX_DEFAULT)
	{
		ListView_SetExtendedListViewStyle(m_hListView, dwExStyle);
	}

	VOID SetExStyle(DWORD dwExStyle, DWORD dwMask)
	{
		ListView_SetExtendedListViewStyleEx(m_hListView, dwMask, dwExStyle);
	}

	UINT GetCount()
	{
		return ListView_GetItemCount(m_hListView);
	}

	VOID SetCount(UINT nCount = 0)
	{
		ListView_SetItemCount(m_hListView, nCount);
	}

	VOID SetCountEx(UINT nCount = 0, DWORD dwFlags = LVSICF_NOINVALIDATEALL)
	{
		ListView_SetItemCountEx(m_hListView, nCount, dwFlags);
	}

	BOOL Clear()
	{
		return ListView_DeleteAllItems(m_hListView);
	}

	BOOL Delete(INT iIndex)
	{
		return ListView_DeleteItem(m_hListView, iIndex);
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CeleMsgBox
class CeleMsgBox
{
public:
	ISTATIC INT ShowMsg(PCTSTR ptzMsg, HWND hParent = GetActiveWindow(), PCTSTR ptzCaption = g_ptzAppName, UINT uType = MB_ICONINFORMATION)
	{
		return MessageBox(hParent, ptzMsg, ptzCaption, uType);
	}

	ISTATIC INT ShowErr(PCTSTR ptzMsg, HWND hParent = GetActiveWindow(), PCTSTR ptzCaption = g_ptzAppName)
	{
		return ShowMsg(ptzMsg, hParent, ptzCaption, MB_ICONERROR);
	}

	ISTATIC BOOL ShowAsk(PCTSTR ptzMsg, HWND hParent = GetActiveWindow(), PCTSTR ptzCaption = g_ptzAppName)
	{
		return ShowMsg(ptzMsg, hParent, ptzCaption, MB_ICONQUESTION | MB_YESNO) == IDYES;
	}

public:
	ISTATIC INT ShowMsg(UINT uText, HWND hParent = GetActiveWindow(), PCTSTR ptzCaption = g_ptzAppName, UINT uType = MB_ICONINFORMATION)
	{
		return ShowMsg(UStrGet(uText), hParent, ptzCaption, uType);
	}

	ISTATIC INT ShowErr(UINT uText, HWND hParent = GetActiveWindow(), PCTSTR ptzCaption = g_ptzAppName)
	{
		return ShowMsg(uText, hParent, ptzCaption, MB_ICONERROR);
	}

	ISTATIC BOOL ShowAsk(UINT uText, HWND hParent = GetActiveWindow(), PCTSTR ptzCaption = g_ptzAppName)
	{
		return ShowMsg(uText, hParent, ptzCaption, MB_ICONQUESTION | MB_YESNO) == IDYES;
	}

#ifdef IDS_CmdMsg
public:
	STATIC INT ShowCmdMsg(UINT uCmd = IDS_AppName, HWND hParent = GetActiveWindow(), PCTSTR ptzMsg = _MakeIntRes(IDS_CmdMsg), UINT uType = MB_ICONINFORMATION)
	{
		CeleMenuBar mb(SHFindMenuBar(hParent));
		_Assert(mb);

		TCHAR tzCmd[MAX_NAME];
		CeleMenu m(mb.GetButtonMenu(1));
		if (!m.GetText(uCmd, tzCmd))
		{
			m = mb.GetButtonMenu(0);
			if (!m.GetText(uCmd, tzCmd))
			{
				UStrLoad(IDS_AppName, tzCmd);
			}
		}

		TCHAR tzText[MAX_STR];
		if (_IsIntRes(ptzMsg))
		{
			UStrPrint(tzText, UStrGet((UINT) ptzMsg), tzCmd);
			ptzMsg = tzText;
		}

		return ShowMsg(ptzMsg, hParent, tzCmd, uType);
	}

	ISTATIC INT ShowCmdErr(UINT uCmd = IDS_AppName, HWND hParent = GetActiveWindow(), PCTSTR ptzMsg = _MakeIntRes(IDS_CmdErr))
	{
		return ShowCmdMsg(uCmd, hParent, ptzMsg, MB_ICONERROR);
	}

	ISTATIC BOOL ShowCmdAsk(UINT uCmd = IDS_AppName, HWND hParent = GetActiveWindow(), PCTSTR ptzMsg = _MakeIntRes(IDS_CmdAsk))
	{
		return ShowCmdMsg(uCmd, hParent, ptzMsg, MB_ICONQUESTION | MB_YESNO) == IDYES;
	}
#endif
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CeleDialog
#define DS_DEFAULT (DS_CENTER | WS_POPUP | WS_BORDER | WS_VISIBLE | DS_SETFOREGROUND)
struct CeleTempDlg
{
	DLGTEMPLATE dtDlg;
	WCHAR wzMenu[1];
	WCHAR wzClass[1];
	WCHAR wzCaption[MAX_NAME];
	WORD wFontSize;
	WCHAR wzFontName[1];

	FINLINE CeleTempDlg(PCTSTR ptzCaption = g_ptzAppName, DWORD dwStyle = DS_DEFAULT)
	{
		_Zero(this);
		dtDlg.style = dwStyle;
		UStrToWStr(wzCaption, ptzCaption, MAX_NAME);
	}

	FINLINE INT_PTR Show(HWND hParent, DLGPROC pProc, PVOID pvParam = NULL)
	{
		return DialogBoxIndirectParam(g_hInst, &dtDlg, hParent, pProc, (LPARAM) pvParam);
	}
};

class CeleDialog
{
public:
	ISTATIC INT_PTR DlgBox(HWND hParent, DLGPROC pProc, PVOID pvParam = NULL, PCTSTR ptzCaption = g_ptzAppName, DWORD dwStyle = DS_DEFAULT)
	{
		CeleTempDlg dlg(ptzCaption, dwStyle);
		return dlg.Show(hParent, pProc, pvParam);
	}

	ISTATIC BOOL InitDlg(HWND hWnd, DWORD dwFlags = SHIDIF_SIZEDLGFULLSCREEN | SHIDIF_DONEBUTTON)
	{
		SHINITDLGINFO di;
		di.hDlg = hWnd;
		di.dwMask = SHIDIM_FLAGS;
		di.dwFlags = dwFlags;
		return SHInitDialog(&di);
	}

	ISTATIC HWND InitDlgBar(HWND hWnd, UINT uMenuBar, BOOL bOverBack = FALSE, BYTE bCancelState = TBSTATE_ENABLED, BYTE bOKState = TBSTATE_ENABLED)
	{
		CeleMenuBar mb;
		mb.Create(hWnd, uMenuBar);

		if (bOverBack)
		{
			mb.OverrideKey(VK_TBACK);
		}
		if (bOKState != TBSTATE_ENABLED)
		{
			mb.SetButtonState(IDOK, bOKState);
		}
		if (bCancelState != TBSTATE_ENABLED)
		{
			mb.SetButtonState(IDCANCEL, bCancelState);
		}

		InitDlg(hWnd);
		return mb;
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CeleComDlg
class CeleComDlg
{
public:
	STATIC BOOL BrowseFile(HWND hParent, PTSTR ptzPath, UINT uFilterRes = 0, BOOL bSave = FALSE);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CeleCtrl
class CeleCtrl: public CeleMsgBox, public CeleDialog, public CeleComDlg
{
public:
	ISTATIC INT GetCtrlRect(HWND hWnd, HWND hCtrl, RECT& rtRect)
	{
		GetWindowRect(hCtrl, &rtRect);
		return MapWindowPoints(NULL, hWnd, (PPOINT) &rtRect, 2);
	}

	ISTATIC INT GetCtrlRect(HWND hWnd, UINT uCtrlID, RECT& rtRect)
	{
		return GetCtrlRect(hWnd, GetDlgItem(hWnd, uCtrlID), rtRect);
	}

	//#define PIX_PAD 10
	#define PIX_PAD GetSystemMetrics(SM_CXVSCROLL)
	ISTATIC UINT ReWidth(HWND hWnd, UINT uCtrlID, UINT uWidth)
	{
		RECT rt;
		HWND hCtrl = GetDlgItem(hWnd, uCtrlID);
		GetCtrlRect(hWnd, hCtrl, rt);
		uWidth -= (rt.left + PIX_PAD);
		MoveWindow(hCtrl, rt.left, rt.top, uWidth, _RectHeight(rt), TRUE);
		return uWidth;
	}

	ISTATIC UINT ReLeft(HWND hWnd, UINT uCtrlID, UINT uWidth)
	{
		RECT rt;
		HWND hCtrl = GetDlgItem(hWnd, uCtrlID);
		GetCtrlRect(hWnd, hCtrl, rt);
		uWidth -= (_RectWidth(rt) + PIX_PAD);
		MoveWindow(hCtrl, uWidth, rt.top, _RectWidth(rt), _RectHeight(rt), TRUE);
		return uWidth;
	}

	ISTATIC UINT ReSize(HWND hWnd, UINT uCtrlID, LPARAM lParam)
	{
		RECT rt;
		HWND hCtrl = GetDlgItem(hWnd, uCtrlID);
		GetCtrlRect(hWnd, hCtrl, rt);
		UINT uWidth = LOWORD(lParam) - (rt.left + PIX_PAD);
		UINT uHeight = HIWORD(lParam) - (rt.top + PIX_PAD);
		MoveWindow(hCtrl, rt.left, rt.top, uWidth, uHeight, TRUE);
		return MAKELONG(uWidth, uHeight);
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CeleActivate
#define _HandleActivate(hWnd, uMsg, wParam, lParam)	\
	STATIC SHACTIVATEINFO s_saInfo = {sizeof(SHACTIVATEINFO)};	\
	switch (uMsg)	\
	{	\
	case WM_ACTIVATE: SHHandleWMActivate(hWnd, wParam, lParam, &s_saInfo, FALSE); break;	\
	case WM_SETTINGCHANGE: SHHandleWMSettingChange(hWnd, wParam, lParam, &s_saInfo);  break;	\
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CeleScroll
struct CeleScroll
{
	//protected:
	SIZE m_szAll;
	SIZE m_szClient;
	POINT m_ptOffset;
	SHACTIVATEINFO m_saInfo;

	//public:
	BOOL HandleScroll(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL HandleScrollEx(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (CeleUtil::IsPPC())
		{
			return HandleScroll(hWnd, uMsg, wParam, lParam);
		}
		else if (uMsg == WM_INITDIALOG)
		{
			SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) | WS_VSCROLL);
			return TRUE;
		}
		return FALSE;
	}

	//protected:
	INT UpdateScroll(HWND hWnd);
	INT ScrollWindow(HWND hWnd, INT nType, INT nScrollCode, INT& cxyOffset, INT cxySizeAll, INT cxyClient);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CeleAbout
class CeleAbout
{
public:
	ISTATIC INT Show(HWND hParent = GetActiveWindow())
	{
		return CeleCtrl::DlgBox(hParent, AboutDlgProc);
	}

	STATIC BOOL CALLBACK AboutDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	STATIC BOOL OnPaint(HWND hWnd);
	STATIC BOOL OnKeyDown(HWND hWnd, UINT nVirtKey);

public:
	ISTATIC VOID SetRect(RECT& rtRect, INT iLeft, INT iTop, INT iRight, INT iBottom)
	{
		rtRect.left = iLeft;
		rtRect.top = iTop;
		rtRect.right = iRight;
		rtRect.bottom = iBottom;
	}

	ISTATIC VOID Draw3DText(HDC hDC, PCTSTR ptzText, LOGFONT& lfFont, RECT& rtRect, COLORREF crColor)
	{
		HFONT hFont = (HFONT) SelectObject(hDC, CreateFontIndirect(&lfFont));

		SetTextColor(hDC, 0x00AAAAAA);
		DrawText(hDC, ptzText, -1, &rtRect, DT_SINGLELINE | DT_VCENTER);

		OffsetRect(&rtRect, -2, -2);
		SetTextColor(hDC, crColor);
		DrawText(hDC, ptzText, -1, &rtRect, DT_SINGLELINE | DT_VCENTER);

		DeleteObject(SelectObject(hDC, hFont));
	}

	ISTATIC VOID DrawLineGroup(HDC hDC, INT iLeft, INT iTop, INT iRight, INT iBottom, COLORREF crColor, UINT nDelta = 2)
	{
		HPEN hPen = (HPEN) SelectObject(hDC, CreatePen(PS_SOLID, 0, crColor));
		for (INT j = iTop; j < iBottom; j += nDelta)
		{
			MoveToEx(hDC, iLeft, j, NULL);
			LineTo(hDC, iRight, j);
			if (nDelta > 2)
			{
				MoveToEx(hDC, iLeft, j + 1, NULL);
				LineTo(hDC, iRight, j + 1);
			}
		}
		DeleteObject(SelectObject(hDC, hPen));
	}

	ISTATIC VOID DrawSolidRect(HDC hDC, CONST RECT& rtRect, COLORREF crColor)
	{
		SetBkColor(hDC, crColor);
		ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rtRect, NULL, 0, NULL);
	}

	ISTATIC VOID DrawGradientRect(HDC hDC, CONST RECT& rtRect, COLORREF crStart, COLORREF crEnd, ULONG ulMode = GRADIENT_FILL_RECT_V)
	{
		TRIVERTEX vt[2];
		vt[0].x = rtRect.left;
		vt[0].y = rtRect.top;
		vt[0].Red = GetRValue(crStart) << 8;
		vt[0].Green = GetGValue(crStart) << 8;
		vt[0].Blue = GetBValue(crStart) << 8;
		vt[0].Alpha = ((BYTE) (crStart >> 24)) << 8;

		vt[1].x = rtRect.right;
		vt[1].y = rtRect.bottom;
		vt[1].Red = GetRValue(crEnd) << 8;
		vt[1].Green = GetGValue(crEnd) << 8;
		vt[1].Blue = GetBValue(crEnd) << 8;
		vt[1].Alpha = ((BYTE) (crStart >> 24)) << 8;

		GRADIENT_RECT rt[1] = {{0, 1}};
		GradientFill(hDC, vt, _NumOf(vt), rt, _NumOf(rt), ulMode);
	}

	ISTATIC VOID DrawGlassRect(HDC hDC, /*CONST */RECT& rtRect, COLORREF crStart1, COLORREF crEnd1, COLORREF crStart2, COLORREF crEnd2)
	{
		INT iHalfHeight = _RectHeight(rtRect) / 2;
		rtRect.bottom -= iHalfHeight;
		DrawGradientRect(hDC, rtRect, crStart1, crEnd1);
		rtRect.top = rtRect.bottom;
		rtRect.bottom += iHalfHeight;
		DrawGradientRect(hDC, rtRect, crStart2, crEnd2);
		rtRect.top -= iHalfHeight;
	}

private:
	ISTATIC VOID DrawHyperLink(HDC hDC, INT iLeft, INT iTop)
	{
		UINT h = GetSystemMetrics(SM_CYSMICON);
		for (UINT i = 0; i < m_cLinkCount; i++)
		{
			m_cLinks[i].rtBound.left = iLeft;
			m_cLinks[i].rtBound.top = iTop + (i * h);
			m_cLinks[i].rtBound.right = 1024;
			m_cLinks[i].rtBound.bottom = m_cLinks[i].rtBound.top + h;
			SetTextColor(hDC, (i == m_uCurLink) ? 0x000000FF : 0x00FF0000);
			DrawText(hDC, m_cLinks[i].ptzText, -1, &m_cLinks[i].rtBound, DT_SINGLELINE | DT_VCENTER | DT_CALCRECT);
			DrawText(hDC, m_cLinks[i].ptzText, -1, &m_cLinks[i].rtBound, DT_SINGLELINE | DT_VCENTER);
		}
	}

	ISTATIC VOID DrawHyperLink(HWND hWnd)
	{
		if (m_cLinkCount)
		{
			HDC hDC = GetDC(hWnd);
			DrawHyperLink(hDC, m_cLinks[0].rtBound.left, m_cLinks[0].rtBound.top);
			ReleaseDC(hWnd, hDC);
		}
	}

	ISTATIC UINT LinkFromPoint(LPARAM lParam)
	{
		POINT pt = {LOWORD(lParam), HIWORD(lParam)};
		for (UINT i = 0; i < m_cLinkCount; i++)
		{
			if (PtInRect(&m_cLinks[i].rtBound, pt))
			{
				return i;
			}
		}
		return -1;
	}

private:
	typedef struct _HYPERLINK
	{
		PCTSTR ptzText;
		PCTSTR ptzOpen;
		PCTSTR ptzParam;
		RECT rtBound;
	}
	HYPERLINK, *PHYPERLINK;

	STATIC UINT m_uCurLink;
	STATIC HYPERLINK m_cLinks[];
	STATIC CONST UINT m_cLinkCount;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
