#pragma once
#include "stdafx.h"

class NAME_EXPORT BaseService
{
public:
	BaseService(
		LPCTSTR service_name,
		LPCTSTR display_name,
		LPCTSTR description
	);

	virtual ~BaseService();
protected:
	void SYSPath();
	void SYSPath(LPCTSTR sys_path);
	void INFPath();
	void INFPath(LPCTSTR inf_path);
public:
	void InstallFromSYS(DWORD service_type, DWORD start_type);
	void InstallFromINF();
	void Uninstall();
	void Start();
	void Stop();
private:
	void ServiceStart();
	void ServicePause();
	void ServiceContinue();
	void ServiceStop();
	void ServiceShutdown();
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
	LPTSTR sys_path_;
	LPTSTR inf_path_;

	SERVICE_STATUS service_status_;
	SERVICE_STATUS_HANDLE service_status_handle_;
	HANDLE service_event_;
};