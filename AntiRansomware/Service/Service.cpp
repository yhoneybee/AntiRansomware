#include "stdafx.h"
#include "Service.h"

DWORD Service::Install(LPCTSTR service_name, LPCTSTR path_name, DWORD start_type)
{
	return Install(service_name, service_name, _T(""), path_name, start_type);
}

DWORD Service::Install(LPCTSTR service_name, LPCTSTR display_name, LPCTSTR path_name, DWORD start_type)
{
	return Install(service_name, display_name, _T(""), path_name, start_type);
}

DWORD Service::Install(LPCTSTR service_name, LPCTSTR display_name, LPCTSTR description, LPCTSTR path_name, DWORD start_type)
{
	DWORD error_id = ERROR_SUCCESS;

	SC_HANDLE manager = nullptr;
	SC_HANDLE service = nullptr;

	manager = OpenSCManager(
		nullptr,
		nullptr,
		SC_MANAGER_ALL_ACCESS);

	if (manager == nullptr)
	{
		return GetLastError();
	}

	service = CreateService(
		manager,
		service_name,
		display_name,
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		start_type,
		SERVICE_ERROR_NORMAL,
		path_name,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr);

	if (service == nullptr)
	{
		error_id = GetLastError();
	}
	else
	{
		SERVICE_DESCRIPTION service_description;
		service_description.lpDescription = (LPTSTR)description;

		if (ChangeServiceConfig2(service, SERVICE_CONFIG_DESCRIPTION, &service_description) == false)
		{
			error_id = GetLastError();
		}

		if (CloseServiceHandle(service) == false)
		{
			error_id = GetLastError();
		}
	}

	if (CloseServiceHandle(manager) == false)
	{
		error_id = GetLastError();
	}

	return error_id;
}

DWORD Service::Start(LPCTSTR service_name)
{
	DWORD error_id = ERROR_SUCCESS;

	SC_HANDLE manager = nullptr;
	SC_HANDLE service = nullptr;

	manager = OpenSCManager(
		nullptr,
		nullptr,
		SC_MANAGER_ALL_ACCESS);

	if (manager == nullptr)
	{
		return GetLastError();
	}

	service = OpenService(
		manager,
		service_name,
		SERVICE_ALL_ACCESS);

	if (service == nullptr)
	{
		error_id = GetLastError();
	}
	else
	{
		if (StartService(service, 0, nullptr) == false)
		{
			error_id = GetLastError();
		}

		if (CloseServiceHandle(service) == false)
		{
			error_id = GetLastError();
		}
	}

	if (CloseServiceHandle(manager) == false)
	{
		error_id = GetLastError();
	}

	return error_id;
}

DWORD Service::Uninstall(LPCTSTR service_name)
{
	return Termination(service_name, true);
}

DWORD Service::ServiceStatus(DWORD status)
{
	switch (status)
	{
	case SERVICE_PAUSED:
		svc_status_.dwCurrentState = SERVICE_PAUSED;
		break;

	case SERVICE_RUNNING:
		svc_status_.dwCurrentState = SERVICE_RUNNING;
		break;

	case SERVICE_START_PENDING:
		svc_status_.dwWin32ExitCode = 0;
		svc_status_.dwCurrentState = SERVICE_START_PENDING;
		svc_status_.dwCheckPoint = 0;
		svc_status_.dwWaitHint = 6000;
		break;

	case SERVICE_STOP_PENDING:
		svc_status_.dwWin32ExitCode = 0;
		svc_status_.dwCurrentState = SERVICE_STOP_PENDING;
		svc_status_.dwCheckPoint = 0;
		svc_status_.dwWaitHint = 6000;
		break;

	case SERVICE_STOPPED:
		svc_status_.dwWin32ExitCode = 0;
		svc_status_.dwCurrentState = SERVICE_STOPPED;
		svc_status_.dwCheckPoint = 0;
		svc_status_.dwWaitHint = 0;
		svc_status_.dwControlsAccepted = SERVICE_ACCEPT_STOP;
		break;
	}

	if (SetServiceStatus(svc_status_handle_, &svc_status_) == false)
	{
		return GetLastError();
	}

	return ERROR_SUCCESS;
}

DWORD Service::Termination(LPCTSTR service_name, bool do_destroy)
{
	DWORD error_id = ERROR_SUCCESS;

	SC_HANDLE manager = nullptr;
	SC_HANDLE service = nullptr;

	SERVICE_STATUS service_status;

	manager = OpenSCManager(
		nullptr,
		nullptr,
		SC_MANAGER_ALL_ACCESS);

	if (manager == nullptr)
	{
		return GetLastError();
	}

	service = OpenService(
		manager,
		service_name,
		SERVICE_ALL_ACCESS);

	if (service == nullptr)
	{
		error_id = GetLastError();
	}
	else
	{
		if (ControlService(service, SERVICE_CONTROL_STOP, &service_status) == false)
		{
			error_id = GetLastError();
		}

		if (do_destroy && DeleteService(service) == false)
		{
			error_id = GetLastError();
		}

		if (CloseServiceHandle(service) == false)
		{
			error_id = GetLastError();
		}
	}

	if (CloseServiceHandle(manager) == false)
	{
		error_id = GetLastError();
	}

	return error_id;
}
