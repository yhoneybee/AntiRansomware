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
	void CreateBalloon(DWORD info_flags, LPCTSTR title, LPCTSTR text);
	void OnTrayNotification(WPARAM wparam, LPARAM lparam);
private:
	void InitNotifyIconData(NOTIFYICONDATA* nid, UINT flags);
	void InitTrayNotifyIconData();
	void InitBalloonNotifyIconData(DWORD info_flags, LPCTSTR title, LPCTSTR text);
private:
	HWND wnd_handle;
	NOTIFYICONDATA tray_nid;
	NOTIFYICONDATA balloon_nid;
	bool visible_tray;
};


