// TrayIconController.cpp : implementation file
//

#include "pch.h"
#include "AntRanGUI.h"
#include "TrayIconController.h"


// TrayIconController

TrayIconController::TrayIconController()
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
	InitNotifyIconData();
	Shell_NotifyIcon(NIM_ADD, &notify_icon_data);
}

void TrayIconController::DestroyTrayIcon()
{
	if (!visible_tray) 
	{
		return;
	}
	InitNotifyIconData();
	Shell_NotifyIcon(NIM_DELETE, &notify_icon_data);
}

void TrayIconController::OnTrayNotification(WPARAM wparam, LPARAM lparam)
{
	SetForegroundWindow(wnd_handle);

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
		DestroyTrayIcon();
	}
		break;
	}
}

void TrayIconController::InitNotifyIconData()
{
	ZeroMemory(&notify_icon_data, sizeof(notify_icon_data));
	notify_icon_data.cbSize = sizeof(notify_icon_data);
	notify_icon_data.uID = 0;
	notify_icon_data.hWnd = wnd_handle;
	notify_icon_data.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	notify_icon_data.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	lstrcpy(notify_icon_data.szTip, _T("hi."));
	notify_icon_data.uCallbackMessage = WM_TRAY_NOTIFYICACTION;
}


// TrayIconController member functions
