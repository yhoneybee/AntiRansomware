#pragma once
#include "stdafx.h"

class BaseService
{
public:
	BaseService(
		LPCTSTR service_name,
		LPCTSTR display_name,
		LPCTSTR description
	);

	BaseService(
		LPCTSTR service_name,
		LPCTSTR display_name,
		LPCTSTR description,
		LPCTSTR exe_path
	);

	virtual ~BaseService();
public:
	void Install();
	void Uninstall();

	void Start();
	void Pause();
	void Continue();
	void Stop();
	void Shutdown();
private:
	void ServiceReportStatus(DWORD current_status, DWORD win32_exit_code, DWORD wait_hint);
public:
	static bool RunServiceMain(BaseService* service);
private:
	static void ServiceMain(DWORD argc, LPTSTR* argv);
	static void ServiceControlHandler(DWORD control);
protected:
	virtual void OnStart() = 0;
	virtual void OnPause() = 0;
	virtual void OnContinue() = 0;
	virtual void OnStop() = 0;
	virtual void OnShutdown() = 0;
protected:
	static BaseService* base_service_;

	DWORD argc_;
	LPTSTR* argv_;
private:
	LPCTSTR name_;
	LPCTSTR display_;
	LPCTSTR description_;
	LPTSTR exe_path_;

	SERVICE_STATUS service_status_;
	SERVICE_STATUS_HANDLE service_status_handle_;
	HANDLE service_event_;
};