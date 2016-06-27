


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header
#include "Main.h"
#include <HtmlCtrl.h>

#pragma comment(lib, "AygShell.lib")
#pragma comment(lib, "HtmlView.lib")
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HELP dialog
INT_PTR CALLBACK HELP(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_INITDIALOG)
	{
		InitHTMLControl(g_hInst);
		CeleCtrl::InitDlgBar(hWnd, IDR_DlgMenu, FALSE, TBSTATE_HIDDEN);

		HWND hHtml = CreateWindow(WC_HTML, NULL, WS_CHILD | WS_VISIBLE | HS_CONTEXTMENU, 0, 0, 0, 0, hWnd, (HMENU) IDC_Browse, g_hInst, NULL);

		if (lParam && *((PTSTR) lParam))
		{
			if ((UStrEqualI((PTSTR) lParam, TEXT("http://")) == 7) || (UStrEqualI((PTSTR) lParam, TEXT("file://")) == 7))
			{
				SendMessage(hHtml, DTM_NAVIGATE, NAVIGATEFLAG_ENTERED, lParam);
				return TRUE;
			}

			PTSTR ptzData;
			if (UFileExist((PTSTR) lParam) && (ptzData = UFileToStr((PTSTR) lParam)))
			{
				SendMessage(hHtml, DTM_ADDTEXTW, FALSE, (LPARAM) ptzData);
				SendMessage(hHtml, DTM_ENDOFSOURCE, 0, 0);
				UMemFree(ptzData);
				return TRUE;
			}
		}
		else
		{
			TCHAR tzPath[512];
			UDirGetAppExt(tzPath, TEXT("htm"));
			if (UFileExist(tzPath))
			{
				TCHAR tzUrl[MAX_PATH];
				UStrPrint(tzUrl, TEXT("file://%s"), tzPath);
				SendMessage(hHtml, DTM_NAVIGATE, NAVIGATEFLAG_ENTERED, (LPARAM) tzUrl);
				return TRUE;
			}
			lParam = (LPARAM) UStrGet(IDS_Help);
		}
		SendMessage(hHtml, DTM_ADDTEXTW, FALSE, lParam);
		SendMessage(hHtml, DTM_ENDOFSOURCE, 0, 0);
		return TRUE;
	}
	else if (uMsg == WM_SIZE)
	{
		MoveWindow(GetDlgItem(hWnd, IDC_Browse), 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
	}
	else if (uMsg == WM_COMMAND)
	{
		if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
		{
			EndDialog(hWnd, S_OK);
		}
	}

	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCUI dialog
INT_PTR CALLBACK CCUI(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR tzStr[MAX_STR];
	STATIC CeleMenuBar s_mb;

	_HandleActivate(hWnd, uMsg, wParam, lParam);
	switch (uMsg)
	{
	case WM_INITDIALOG:
		g_hWnd = hWnd;
		s_mb = CeleCtrl::InitDlgBar(hWnd, IDR_MainMenu, TRUE);
		SetDlgItemText(hWnd, IDC_Path, (PCTSTR) lParam);
		return TRUE;

	case WM_SIZE:
		CeleCtrl::ReSize(hWnd, IDC_Log, lParam);
		CeleCtrl::ReWidth(hWnd, IDC_Tip, LOWORD(lParam));
		CeleCtrl::ReWidth(hWnd, IDC_Path, CeleCtrl::ReLeft(hWnd, IDC_Browse, LOWORD(lParam)));
		break;

	case WM_HOTKEY:
		if (HIWORD(lParam) == VK_TBACK)
		{
			HWND hCtrl = GetDlgItem(hWnd, IDC_Path);
			if (hCtrl != GetFocus())
			{
				SetFocus(hCtrl);
				SetWindowText(hCtrl, NULL);
			}
			else
			{
				SHSendBackToFocusWindow(uMsg, wParam, lParam);
			}
		}
		break;

	case WM_INITMENUPOPUP:
		if (LOWORD(lParam) == IDCANCEL)
		{
			CeleMenu m;
			HKEY hKey;
			m = (HMENU) wParam;
			if (RegOpenKeyEx(HKEY_CLASSES_ROOT, STR_AppName, 0, KEY_ALL_ACCESS, &hKey) == S_OK)
			{
				RegCloseKey(hKey);
				m.Check(IDM_Assoc, TRUE);
			}
			else
			{
				m.Check(IDM_Assoc, FALSE);
			}
		}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_Browse:
			CeleCtrl::BrowseFile(hWnd, _MakeIntRes(IDC_Path), IDS_Filter);
			break;

		case IDC_Path:
			if (HIWORD(wParam) == EN_CHANGE)
			{
				s_mb.EnableButton(IDOK, GetWindowTextLength(GetDlgItem(hWnd, IDC_Path)));
			}
			else if (HIWORD(wParam) == EN_SETFOCUS)
			{
				DWORD dwConversion, dwSentence;
				ImmGetConversionStatus(NULL, &dwConversion, &dwSentence);
				ImmSetConversionStatus(NULL, dwConversion & ~IME_CMODE_NATIVE, NULL);
			}
			break;

		case IDM_Clear:
			SetDlgItemText(g_hWnd, IDC_Log, NULL);
			break;

		case IDM_Assoc:
			if (TRUE)
			{
				CeleMenu m;
				m = s_mb.GetMenu(IDCANCEL);
				if (m.IsChecked(IDM_Assoc))
				{
					ASOC(TEXT("!"));
					ASOC(TEXT("!.reg"));
					ASOC(TEXT("!.ccs"));
					ASOC(TEXT("!.provxml"));
					CeleCtrl::ShowCmdMsg(IDM_Assoc, hWnd, _MakeIntRes(IDS_UnAssoc));
				}
				else
				{
					ASOC(TEXT(".ccs"));
					ASOC(TEXT(".reg"));
					ASOC(TEXT(".provxml"));
					CeleCtrl::ShowCmdMsg(IDM_Assoc, hWnd, _MakeIntRes(IDS_Assoc));
				}
			}
			break;

		case IDM_Help:
			return CeleCtrl::DlgBox(hWnd, HELP);

		case IDM_About:
			CeleAbout::Show(hWnd);
			break;

		case IDOK:
			if (HIWORD(wParam) == 0)
			{
				GetDlgItemText(hWnd, IDC_Path, tzStr, MAX_PATH);
				Process(tzStr, g_ptzCurFile);
				break;
			}

		case IDCANCEL:
		case IDM_Exit:
			EndDialog(hWnd, S_OK);
			break;
		}
		break;
	}

	return FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EXE Entry
INT APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PTSTR ptzCmdLine, INT iCmdShow)
{
	g_hInst = hInstance;
	g_ptzAppName = UStrGet(IDS_AppName);

	TCHAR tzPath[MAX_PATH];
	GetModuleFileName(NULL, tzPath, MAX_PATH);
	if (*ptzCmdLine)
	{
		return Process(UStrTrim(ptzCmdLine), tzPath);
	}
	else
	{
		HRSRC hRsrc = FindResource(g_hInst, STR_AppName, RT_RCDATA);
		if (hRsrc)
		{
			HGLOBAL hGlobal = LoadResource(g_hInst, hRsrc);
			if (hGlobal)
			{
				return Process((PTSTR) LockResource(hGlobal), tzPath);
			}
		}
		return Process(TEXT("CCUI"), tzPath);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
