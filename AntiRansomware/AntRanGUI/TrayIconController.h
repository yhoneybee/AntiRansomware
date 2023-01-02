#pragma once

// TrayIconController command target

class TrayIconController : public CObject
{
public:
	TrayIconController();
	virtual ~TrayIconController();
public:
	void SetHWND(HWND value);
	void CreateTrayIcon();
	void DestroyTrayIcon();
	void OnTrayNotification(WPARAM wparam, LPARAM lparam);
private:
	void InitNotifyIconData();
private:
	HWND wnd_handle;
	NOTIFYICONDATA notify_icon_data;
	bool visible_tray;
};


