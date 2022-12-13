#pragma once
#include "stdafx.h"

class Service
{
public:
	Service()
		:svc_status_handle_{ 0 }, svc_status_{ 0 }
	{}
	~Service() {}
public:
	DWORD Install(LPCTSTR service_name, LPCTSTR path_name, DWORD start_type);
	DWORD Install(LPCTSTR service_name, LPCTSTR display_name, LPCTSTR path_name, DWORD start_type);
	DWORD Install(LPCTSTR service_name, LPCTSTR display_name, LPCTSTR description, LPCTSTR path_name, DWORD start_type);

	DWORD Start(LPCTSTR service_name);

	DWORD Uninstall(LPCTSTR service_name);

	void ServiceStatusHandle(SERVICE_STATUS_HANDLE svc_status_handle) { svc_status_handle_ = svc_status_handle; }
	DWORD ServiceStatus(DWORD status);
private:
	DWORD Termination(LPCTSTR service_name, bool do_destroy);
private:
	SERVICE_STATUS_HANDLE svc_status_handle_;
	SERVICE_STATUS svc_status_;
};