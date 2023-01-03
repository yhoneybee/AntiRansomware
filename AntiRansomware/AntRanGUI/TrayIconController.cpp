// TrayIconController.cpp : implementation file
//

#include "pch.h"
#include "AntRanGUI.h"
#include "TrayIconController.h"

constexpr UINT TRAY_FLAGS = NIF_ICON | NIF_MESSAGE | NIF_TIP;
constexpr UINT BALLOON_FLAGS = NIF_ICON | NIF_MESSAGE | NIF_INFO;


// TrayIconController

TrayIconController::TrayIconController()
	:wnd_handle{ 0 }, visible_tray{ false }, tray_nid{ 0 }
{
}

TrayIconController::~TrayIconController()
{
	DestroyTrayIcon();
}

void TrayIconController::SetHWND(HWND value)
{
	wnd_handle = value;
}

void TrayIconController::CreateTrayIcon()
{
	if (visible_tray)
	{
		return;
	}
	InitTrayNotifyIconData();
	if (Shell_NotifyIcon(NIM_ADD, &tray_nid) == false)
	{
		Shell_NotifyIcon(NIM_MODIFY, &tray_nid);
	}
	visible_tray = true;
}

void TrayIconController::DestroyTrayIcon()
{
	if (!visible_tray)
	{
		return;
	}
	InitTrayNotifyIconData();
	Shell_NotifyIcon(NIM_DELETE, &tray_nid);
	visible_tray = false;
}

void TrayIconController::CreateBalloon(DWORD info_flags, LPCTSTR title, LPCTSTR text)
{
	InitBalloonNotifyIconData(info_flags, title, text);
	if (Shell_NotifyIcon(NIM_ADD, &balloon_nid) == false)
	{
		Shell_NotifyIcon(NIM_MODIFY, &balloon_nid);
	}
	Shell_NotifyIcon(NIM_DELETE, &balloon_nid);
}

void TrayIconController::OnTrayNotification(WPARAM wparam, LPARAM lparam)
{
	switch (lparam)
	{
	case WM_RBUTTONDOWN:
	{
		CPoint point;
		GetCursorPos(&point);
		CMenu menu;
		menu.LoadMenu(IDR_MENU1);
		CMenu* sub_menu = menu.GetSubMenu(0);
		sub_menu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
	}
	break;
	case WM_LBUTTONDBLCLK:
	{
		AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
		DestroyTrayIcon();
	}
	break;
	}
}

void TrayIconController::InitTrayNotifyIconData()
{
	ZeroMemory(&tray_nid, sizeof(tray_nid));
	InitNotifyIconData(&tray_nid, TRAY_FLAGS);
	lstrcpy(tray_nid.szTip, _T("AntRan"));
}

void TrayIconController::InitBalloonNotifyIconData(DWORD info_flags, LPCTSTR title, LPCTSTR text)
{
	ZeroMemory(&balloon_nid, sizeof(balloon_nid));
	InitNotifyIconData(&balloon_nid, BALLOON_FLAGS);
	balloon_nid.dwInfoFlags = info_flags;
	lstrcpy(balloon_nid.szInfoTitle, title);
	lstrcpy(balloon_nid.szInfo, text);
}

void TrayIconController::InitNotifyIconData(NOTIFYICONDATA* nid, UINT flags)
{
	nid->cbSize = sizeof(NOTIFYICONDATA);
	nid->uID = 0;
	nid->hWnd = wnd_handle;
	nid->uFlags = flags;
	nid->hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	nid->uCallbackMessage = WM_TRAY_NOTIFYICACTION;
}


// TrayIconController member functions
