


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CeleCtrl 1.6.30
// Copyright (C) Yonsm 2008, All Rights Reserved.
#include "CeleCtrl.h"
#include <CommDlg.h>
#include <DeviceResolutionAware.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Declaration
#if (_WIN32_WCE < 0x500)
typedef struct tagOPENFILENAMEEX
{
	// Fields which map to OPENFILENAME
	DWORD			lStructSize;
	HWND			hwndOwner;
	HINSTANCE		hInstance;
	PCTSTR			lpstrFilter;
	PTSTR			lpstrCustomFilter;
	DWORD			nMaxCustFilter;
	DWORD			nFilterIndex;
	PTSTR			lpstrFile;
	DWORD			nMaxFile;
	LPTSTR			lpstrFileTitle;
	DWORD			nMaxFileTitle;
	PCTSTR			lpstrInitialDir;
	PCTSTR			lpstrTitle;
	DWORD			Flags;
	WORD			nFileOffset;
	WORD			nFileExtension;
	PCTSTR			lpstrDefExt;
	LPARAM			lCustData;
	LPOFNHOOKPROC	lpfnHook;
	PCTSTR			lpTemplateName;

	// Extended fields
	DWORD			dwSortOrder;
	DWORD			ExFlags;
}
OPENFILENAMEEX, *LPOPENFILENAMEEX;

// Sort order
typedef enum tagOFN_SORTORDER
{
	OFN_SORTORDER_AUTO,
	OFN_SORTORDER_DATE,
	OFN_SORTORDER_NAME,
	OFN_SORTORDER_SIZE,
	OFN_SORTORDER_ASCENDING = 0x00008000

}
OFN_SORTORDER;

// Extended Flags
typedef enum tagOFN_EXFLAG
{
	OFN_EXFLAG_DETAILSVIEW			= 0x00000001,
	OFN_EXFLAG_THUMBNAILVIEW		= 0x00000002,
	OFN_EXFLAG_LOCKDIRECTORY		= 0x00000100,
	OFN_EXFLAG_NOFILECREATE			= 0x00000200,
	OFN_EXFLAG_HIDEDRMPROTECTED		= 0x00010000,
	OFN_EXFLAG_HIDEDRMFORWARDLOCKED	= 0x00020000
}
OFN_EXFLAG;
#endif

typedef BOOL (*PGETFILENAME)(LPOPENFILENAMEEX pOfn);
typedef BOOL (*TGETFILENAME)(BOOL bSave, LPOPENFILENAMEEX pOfn);

// Module name and procedure table
enum {MP_AygShell, MP_RFileShell, MP_tGetFile, MP_CoreDll};
const struct {PCTSTR ptzModName; PCTSTR ptzOpenProc; PCTSTR ptzSaveProc;} c_sModProc[] =
{
	{TEXT("AygShell"), TEXT("GetOpenFileNameEx"), TEXT("GetSaveFileNameEx")},
	{TEXT("RFileShell"), TEXT("RShellGetOpenFileName"), TEXT("RShellGetSaveFileName")},
	{TEXT("tGetFile"), TEXT("tGetFile"), TEXT("tGetFile")},
	{TEXT("CoreDll"), TEXT("GetOpenFileNameW"), TEXT("GetSaveFileNameW")},
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Browse for file name
#define OFN_COMMON (0x11000000)
BOOL CeleComDlg::BrowseFile(HWND hParent, PTSTR ptzPath, UINT uFilterRes, BOOL bSave)
{
	// Fill structure
	OPENFILENAMEEX ofn = {0};
	ofn.hwndOwner = hParent;
	ofn.hInstance = g_hInst;
	ofn.lpstrFile = ptzPath;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = bSave ? (OFN_COMMON | OFN_OVERWRITEPROMPT) : (OFN_COMMON | OFN_PATHMUSTEXIST);

	ofn.ExFlags = OFN_EXFLAG_THUMBNAILVIEW;
	ofn.dwSortOrder = OFN_SORTORDER_NAME | OFN_SORTORDER_ASCENDING;

	// Get initialize folder from control
	TCHAR tzPath[MAX_PATH];
	if (_IsIntRes(ptzPath))
	{
		ofn.lpstrFile = tzPath;
		GetDlgItemText(hParent, (UINT) ptzPath, tzPath, MAX_PATH);
	}

#define _SAVEDIR
#ifdef _SAVEDIR
	// Read initialize folder
	CeleReg reg;
	TCHAR tzInitDir[MAX_PATH];
	if (reg.GetStr(TEXT("BrowseFile"), tzInitDir))
	{
		ofn.lpstrInitialDir = tzInitDir;
	}
	else
	{
		ofn.lpstrInitialDir = TEXT("\\");
	}
#else
	ofn.lpstrInitialDir = TEXT("\\");
	if (ofn.lpstrFile[0] == 0)
	{
		ofn.lpstrFile[0] = '\\';
		ofn.lpstrFile[1] = 0;
	}
#endif

	// Load extension filter
	TCHAR tzFilter[MAX_PATH];
	if (UStrLoad(uFilterRes, tzFilter))
	{
		ofn.lpstrFilter = UStrRep(tzFilter, '|', 0);
	}

	// Parse default extension
	TCHAR tzDefExt[MAX_NAME];
	PTSTR p = UStrChr(tzFilter, 0) + 1;
	if (p = UStrChr(p, '.'))
	{
		UStrCopy(tzDefExt, p);
		if (p = UStrChr(tzDefExt, ';'))
		{
			*p = 0;
		}
		ofn.lpstrDefExt = tzDefExt;
	}

#ifdef IDS_Open
	// Get title
	TCHAR tzTitle[MAX_NAME];
	ofn.lpstrTitle = tzTitle;
	UStrLoad(bSave ? IDS_Save : IDS_Open, tzTitle);
#endif

	// Try all library
	UINT i = 0;
	BOOL bResult = FALSE;
	for (; i < _NumOf(c_sModProc); i++)
	{
		HMODULE hModule = LoadLibrary(c_sModProc[i].ptzModName);
		if ((hModule == NULL) && (i == MP_RFileShell))
		{
			// Lookup RESCO Explorer install folder
			TCHAR tzModule[MAX_PATH];
			CeleReg rfs(TEXT("SOFTWARE\\Apps\\Resco Explorer"), HKEY_LOCAL_MACHINE);
			if (rfs.GetStr(TEXT("InstallDir"), tzModule))
			{
				UPathMake(tzModule, c_sModProc[i].ptzModName);
				hModule = LoadLibrary(tzModule);
			}
		}
		if (hModule)
		{
			PGETFILENAME pGetFileName = (PGETFILENAME) GetProcAddress(hModule, bSave ? c_sModProc[i].ptzSaveProc : c_sModProc[i].ptzOpenProc);
			if (pGetFileName)
			{
				ofn.lStructSize = (i == MP_AygShell) ? sizeof(OPENFILENAMEEX) : sizeof(OPENFILENAME);
				bResult = (i == MP_tGetFile) ? ((TGETFILENAME) pGetFileName)(bSave, &ofn) : pGetFileName(&ofn);
				FreeLibrary(hModule);
				break;
			}
			FreeLibrary(hModule);
		}
	}

	// Prompt for library
	if ((i == MP_CoreDll) && !CeleUtil::IsPPC() && !CeleUtil::IsWM5() && !bResult)
	{
		UStrPrint(ofn.lpstrFile, TEXT("\\My Documents\\%s%s"), STR_AppName, ofn.lpstrDefExt);
#ifdef IDS_PromptFile
		TCHAR tzInfo[MAX_STR];
		UStrPrint(tzInfo, UStrGet(IDS_PromptFile), ofn.lpstrFile);
		bResult = (MessageBox(hParent, tzInfo, tzTitle, MB_ICONQUESTION | MB_YESNO) == IDYES);
#else
		bResult = TRUE;
#endif
	}

	if (bResult)
	{
		// Set control text
		if (_IsIntRes(ptzPath))
		{
			SetFocus(GetDlgItem(hParent, (UINT) ptzPath));
			SetDlgItemText(hParent, (UINT) ptzPath, ofn.lpstrFile);
		}

#ifdef _SAVEDIR
		// Save initialize folder
		p = UStrRChr(ofn.lpstrFile, '\\');
		if (p++)
		{
			TCHAR c = *p;
			*p = 0;
			reg.SetStr(TEXT("BrowseFile"), ofn.lpstrFile);
			*p = c;
		}
#endif
	}

	return bResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Scroll dialog handler
BOOL CeleScroll::HandleScroll(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	//case WM_CREATE:
	case WM_INITDIALOG:
		POINT ptMin, ptMax;
		ptMax.x = ptMax.y = 0;
		ptMin.x = ptMin.y = 0x7FFFFFFF;
		for (HWND hCtrl = GetWindow(hWnd, GW_CHILD); hCtrl; hCtrl = GetWindow(hCtrl, GW_HWNDNEXT))
		{
			RECT rt;
			GetWindowRect(hCtrl, &rt);
			MapWindowPoints(NULL, hWnd, (PPOINT) &rt, 2);
			if (rt.left < ptMin.x) ptMin.x = rt.left;
			if (rt.top < ptMin.y) ptMin.y = rt.top;
			if (rt.right > ptMax.x) ptMax.x = rt.right;
			if (rt.bottom > ptMax.y) ptMax.y = rt.bottom;
		}
		m_szAll.cx = ptMax.x + ptMin.x - GetSystemMetrics(SM_CXVSCROLL);
		m_szAll.cy = ptMax.y + ptMin.y;
		m_szClient.cx = m_szClient.cy = 0;
		m_ptOffset.x = m_ptOffset.y = 0;
		UpdateScroll(hWnd);

		_Zero(&m_saInfo);
		m_saInfo.cbSize = sizeof(SHACTIVATEINFO);
		return TRUE;

	case WM_ACTIVATE:
		SHHandleWMActivate(hWnd, wParam, lParam, &m_saInfo, FALSE);
		break;

	case WM_SETTINGCHANGE:
		SHHandleWMSettingChange(hWnd, wParam, lParam, &m_saInfo);
		break;

	case WM_HSCROLL:
		ScrollWindow(hWnd, SB_HORZ, LOWORD(wParam), (INT&) m_ptOffset.x, m_szAll.cx, m_szClient.cx);
		break;

	case WM_VSCROLL:
		ScrollWindow(hWnd, SB_VERT, LOWORD(wParam), (INT&) m_ptOffset.y, m_szAll.cy, m_szClient.cy);
		break;

	case WM_SIZE:
		m_szClient.cx = LOWORD(lParam);
		m_szClient.cy = HIWORD(lParam);
		UpdateScroll(hWnd);
		break;

	case WM_KEYUP:
		if (wParam == VK_LEFT || wParam == VK_RIGHT)
		{
			ScrollWindow(hWnd, SB_HORZ, (wParam == VK_LEFT ? SB_PAGELEFT : SB_PAGERIGHT), (INT&) m_ptOffset.x, m_szAll.cx, m_szClient.cx);
		}
		break;
	}
	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Update scroll information
INT CeleScroll::UpdateScroll(HWND hWnd)
{
	ScrollWindowEx(hWnd, m_ptOffset.x, m_ptOffset.y, NULL, NULL, NULL, NULL, SW_ERASE | SW_INVALIDATE | SW_SCROLLCHILDREN);
	m_ptOffset.x = m_ptOffset.y = 0;

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = 0;
	si.nMax = m_szAll.cx - 1;
	si.nPage = m_szClient.cx;
	si.nPos = m_ptOffset.x;
	si.nTrackPos = 0;
	SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);

	//si.cbSize = sizeof(SCROLLINFO);
	//si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	//si.nMin = 0;
	//si.nTrackPos = 0;
	si.nMax = m_szAll.cy - 1;
	si.nPage = m_szClient.cy;
	si.nPos = m_ptOffset.y;
	return SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Scroll window
INT CeleScroll::ScrollWindow(HWND hWnd, INT nType, INT nScrollCode, INT& cxyOffset, INT cxySizeAll, INT cxyClient)
{
	INT cxyMax = cxySizeAll - cxyClient;
	if (cxyMax < 0)   // Can't scroll, client area is bigger
	{
		return ERROR;
	}

	INT cxySizeLine = cxySizeAll / 100;
	if (cxySizeLine < 1)
	{
		cxySizeLine = 1;
	}
	INT cxySizePage = cxySizeAll / 10;
	if (cxySizePage < 1)
	{
		cxySizePage = 1;
	}

	INT cxyScroll = 0;
	BOOL bUpdate = TRUE;

	switch (nScrollCode)
	{
	case SB_TOP:		// top or all left
		cxyScroll = cxyOffset;
		cxyOffset = 0;
		break;

	case SB_BOTTOM:		// bottom or all right
		cxyScroll = cxyOffset - cxyMax;
		cxyOffset = cxyMax;
		break;

	case SB_LINEUP:		// line up or line left
		if (cxyOffset >= cxySizeLine)
		{
			cxyScroll = cxySizeLine;
			cxyOffset -= cxySizeLine;
		}
		else
		{
			cxyScroll = cxyOffset;
			cxyOffset = 0;
		}
		break;

	case SB_LINEDOWN:	// line down or line right
		if (cxyOffset < cxyMax - cxySizeLine)
		{
			cxyScroll = -cxySizeLine;
			cxyOffset += cxySizeLine;
		}
		else
		{
			cxyScroll = cxyOffset - cxyMax;
			cxyOffset = cxyMax;
		}
		break;

	case SB_PAGEUP:		// page up or page left
		if (cxyOffset >= cxySizePage)
		{
			cxyScroll = cxySizePage;
			cxyOffset -= cxySizePage;
		}
		else
		{
			cxyScroll = cxyOffset;
			cxyOffset = 0;
		}
		break;

	case SB_PAGEDOWN:	// page down or page right
		if (cxyOffset < cxyMax - cxySizePage)
		{
			cxyScroll = -cxySizePage;
			cxyOffset += cxySizePage;
		}
		else
		{
			cxyScroll = cxyOffset - cxyMax;
			cxyOffset = cxyMax;
		}
		break;

	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		SCROLLINFO si;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_TRACKPOS;
		if (GetScrollInfo(hWnd, nType, &si))
		{
			cxyScroll = cxyOffset - si.nTrackPos;
			cxyOffset = si.nTrackPos;
		}
		break;

	case SB_ENDSCROLL:
	default:
		return ERROR;
	}

	if (!cxyScroll)
	{
		return ERROR;
	}

	SetScrollPos(hWnd, nType, cxyOffset, TRUE);
	if (nType == SB_VERT)
	{
		return ScrollWindowEx(hWnd, 0, cxyScroll, NULL, NULL, NULL, NULL, SW_ERASE | SW_INVALIDATE | SW_SCROLLCHILDREN);
	}
	else
	{
		return ScrollWindowEx(hWnd, cxyScroll, 0, NULL, NULL, NULL, NULL, SW_ERASE | SW_INVALIDATE | SW_SCROLLCHILDREN);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// About dialog call back function
UINT CeleAbout::m_uCurLink = -1;
CeleAbout::HYPERLINK CeleAbout::m_cLinks[] =
{
#ifdef STR_PhoneCmd
	{STR_Phone, STR_PhoneCmd},
#endif
#ifdef STR_MailCmd
	{STR_Mail, STR_MailCmd},
#endif
#ifdef STR_WebCmd
	{STR_Web, STR_WebCmd},
#endif
};
CONST UINT CeleAbout::m_cLinkCount = _NumOf(m_cLinks);
BOOL CALLBACK CeleAbout::AboutDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		if (TRUE)
		{
			CeleMenuBar mb;
#ifdef IDR_DlgMenu
			mb.Create(hWnd, IDR_DlgMenu, SHCMBF_HIDESIPBUTTON);
#else
			mb.Create(hWnd, 0, SHCMBF_HIDESIPBUTTON | SHCMBF_EMPTYBAR);
#endif
			mb.OverrideKey();
			mb.SetButtonState(IDCANCEL, TBSTATE_HIDDEN);
			CeleCtrl::InitDlg(hWnd, SHIDIF_SIZEDLGFULLSCREEN | SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN);
		}
		break;

	case WM_HOTKEY:
		if (HIWORD(lParam) == VK_TBACK)
		{
			EndDialog(hWnd, 0);
		}
		break;

	case WM_PAINT:
		OnPaint(hWnd);
		break;

	case WM_LBUTTONUP:
		if (LinkFromPoint(lParam) == m_uCurLink)
		{
			OnKeyDown(hWnd, VK_RETURN);
		}
		break;

	case WM_LBUTTONDOWN:
		UINT uCurLink;
		uCurLink = LinkFromPoint(lParam);
		if (m_uCurLink != uCurLink)
		{
			m_uCurLink = uCurLink;
			DrawHyperLink(hWnd);
		}
		break;

	case WM_KEYDOWN:
		OnKeyDown(hWnd, wParam);
		break;

	case WM_GETDLGCODE:
		if (PMSG pMsg = (PMSG) lParam)
		{
			if ((pMsg->message == WM_KEYDOWN) &&
				((pMsg->wParam != VK_RETURN) || (m_uCurLink < m_cLinkCount)))
			{
				SetWindowLong(hWnd, DWL_MSGRESULT, DLGC_WANTALLKEYS);
				return DLGC_WANTALLKEYS;
			}
		}
		break;

	case WM_COMMAND:
		if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
		{
			return EndDialog(hWnd, wParam);
		}
		break;
	}

	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Paint about dialog
//#define _COLOR_OfficeXP
#ifdef _COLOR_OfficeXP
#define COLOR_LineGroup1	0x00FF9966
#define COLOR_LineGroup2	0x00FF9966
#define COLOR_SolidRect1	0x00FF9966
#define COLOR_SolidRect2	0x00CC3333
#define COLOR_LogoRect1		0x00CC3333
#define COLOR_LogoRect2		0x00DDFFFF
#define COLOR_VersionText	0x00BBBBBB
#define COLOR_CopyrightText	0x00000000
#define COLOR_AppNameText	0x00111111
#else
#define COLOR_LineGroup1	GetSysColor(COLOR_ACTIVECAPTION)
#define COLOR_LineGroup2	GetSysColor(COLOR_ACTIVECAPTION)
#define COLOR_SolidRect1	GetSysColor(COLOR_ACTIVECAPTION)
#define COLOR_SolidRect2	GetSysColor(COLOR_MENUTEXT)
#define COLOR_LogoRect1		GetSysColor(COLOR_MENUTEXT)
#define COLOR_LogoRect2		GetSysColor(COLOR_INFOBK)
#define COLOR_VersionText	GetSysColor(COLOR_GRAYTEXT)
#define COLOR_CopyrightText	GetSysColor(COLOR_WINDOWTEXT)
#define COLOR_AppNameText	GetSysColor(COLOR_WINDOWTEXT)
#endif
#ifdef IDS_Copyright
#define ABOUT_AppDesc		UStrGet(IDS_AppDesc)
#define ABOUT_Copyright		UStrGet(IDS_Copyright)
#else
#define ABOUT_AppDesc		STR_AppDesc
#define ABOUT_Copyright		STR_Copyright
#endif
BOOL CeleAbout::OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hWnd, &ps);

	// Calculate reference point
	RECT rtDlg;
	POINT ptRef;
	GetClientRect(hWnd, &rtDlg);
	ptRef.x = rtDlg.right / (5 - (rtDlg.right >= DRA::SCALEX(200)));
	ptRef.y = rtDlg.bottom / (5 - (rtDlg.bottom >= DRA::SCALEY(200)));

	// Group line 1
	DrawLineGroup(hDC, 0, 0, ptRef.x, ptRef.y, COLOR_LineGroup1, DRA::SCALEY(2));

	// Background rect
	RECT rt;
	SetRect(rt, 0, ptRef.y, rtDlg.right, ptRef.y + DRA::SCALEY(36));
	DrawSolidRect(hDC, rt, COLOR_SolidRect1);
	//DrawGlassRect(hDC, rt, 0x00D1AE7A, 0x00B98835, 0x00975F00, 0x00C6A46A);
	rt.top = rt.bottom;
	rt.bottom += DRA::SCALEY(12);
	DrawSolidRect(hDC, rt, COLOR_SolidRect2);

	// Group line 2
	DrawLineGroup(hDC, ptRef.x - DRA::SCALEX(28), rt.bottom + 1, rtDlg.right, rt.bottom + DRA::SCALEY(14), COLOR_LineGroup2, DRA::SCALEY(2));

	// Version text
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, COLOR_VersionText);
	if (rtDlg.bottom >= DRA::SCALEY((200)))
	{
		SetRect(rt, ptRef.x - DRA::SCALEX(28), rt.bottom + DRA::SCALEY(16), rtDlg.right - DRA::SCALEX(2), rt.bottom + DRA::SCALEY(32));
		DrawText(hDC, STR_VersionStamp, -1, &rt, DT_SINGLELINE | DT_VCENTER);
		DrawText(hDC, STR_BuildStamp, -1, &rt, DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
		//DrawLineGroup(hDC, ptRef.x - DRA::SCALEX(28), rt1.bottom + DRA::SCALEY(32), rtDlg.right, rt1.bottom + DRA::SCALEY(33), 0x00000000);
	}
	else
	{
		SetRect(rt, ptRef.x + DRA::SCALEX(30), ptRef.y, rtDlg.right + DRA::SCALEX(2), ptRef.y + DRA::SCALEY(18));
		DrawText(hDC, STR_VersionStamp, -1, &rt, DT_SINGLELINE | DT_VCENTER);
		rt.top = rt.bottom;
		rt.bottom += DRA::SCALEY(18);
		DrawText(hDC, STR_BuildStamp, -1, &rt, DT_SINGLELINE | DT_VCENTER);
	}

	// Hyper link text
	DrawHyperLink(hDC, ptRef.x - DRA::SCALEX(28), rtDlg.bottom - DRA::SCALEY(34) - GetSystemMetrics(SM_CYSMICON) * m_cLinkCount);

	// Copyright text
	SetTextColor(hDC, COLOR_CopyrightText);
	SetRect(rt, ptRef.x - DRA::SCALEX(28), rtDlg.bottom - DRA::SCALEY(16), rtDlg.right, rtDlg.bottom);
	DrawText(hDC, ABOUT_Copyright, -1, &rt, DT_SINGLELINE | DT_VCENTER);
	rt.bottom = rt.top;
	rt.top -= DRA::SCALEY(16);
	DrawText(hDC, ABOUT_AppDesc, -1, &rt, DT_SINGLELINE | DT_VCENTER);

	// Frame rect
	SetRect(rt, ptRef.x - DRA::SCALEX(28), ptRef.y - DRA::SCALEY(28), ptRef.x + DRA::SCALEX(28), ptRef.y + DRA::SCALEY(28));
	DrawSolidRect(hDC, rt, COLOR_LogoRect1);
	InflateRect(&rt, DRA::SCALEX(-4), DRA::SCALEY(-4));
	DrawSolidRect(hDC, rt, COLOR_LogoRect2);

	// Logo icon
	DrawIconEx(hDC, ptRef.x - DRA::SCALEX(16), ptRef.y - DRA::SCALEY(16), LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_Main)),
		GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);

	// Name text
	LOGFONT lf;
	GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &lf);
	lf.lfHeight *= 2;
	lf.lfWeight = FW_BOLD;
	lf.lfQuality = ANTIALIASED_QUALITY;

	SetRect(rt, ptRef.x + DRA::SCALEX(30), ptRef.y - DRA::SCALEY(30), rtDlg.right, ptRef.y);
	Draw3DText(hDC, g_ptzAppName, lf, rt, COLOR_AppNameText);

	EndPaint(hWnd, &ps);
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Key down handler
BOOL CeleAbout::OnKeyDown(HWND hWnd, UINT nVirtKey)
{
	switch (nVirtKey)
	{
	case VK_UP:
		if (m_uCurLink == 0)
		{
			return FALSE;
		}
		else if (m_uCurLink >= m_cLinkCount)
		{
			m_uCurLink = m_cLinkCount - 1;
		}
		else
		{
			m_uCurLink--;
		}
		break;

	case VK_TAB:
	case VK_DOWN:
		if (m_uCurLink == m_cLinkCount - 1)
		{
			return FALSE;
		}
		else if (m_uCurLink >= m_cLinkCount)
		{
			m_uCurLink = 0;
		}
		else
		{
			m_uCurLink++;
		}
		break;

	case VK_RETURN:
		if (m_uCurLink < m_cLinkCount)
		{
			return CeleUtil::ShellOpen(m_cLinks[m_uCurLink].ptzOpen, m_cLinks[m_uCurLink].ptzParam);
		}

	default:
		return FALSE;
	}

	DrawHyperLink(hWnd);
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
