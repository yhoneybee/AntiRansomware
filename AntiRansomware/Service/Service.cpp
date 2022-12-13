#include "stdafx.h"
#include "Service.h"

DWORD Service::Install(LPCTSTR path_name, DWORD start_type)
{
	DWORD error_id = ERROR_SUCCESS;

	SC_HANDLE manager = nullptr;
	SC_HANDLE service = nullptr;

	manager = OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);

	if (manager == nullptr)
	{
		return GetLastError();
	}

	service = CreateService(
		manager,
		_T("TestService"),
		_T("TestServiceDisplay"),
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		start_type,
		SERVICE_ERROR_CRITICAL,
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
		if (CloseServiceHandle(service) == FALSE)
		{
			return GetLastError();
		}
	}

	if (CloseServiceHandle(manager) == FALSE)
	{
		return GetLastError();
	}

	return error_id;
}

DWORD Service::Uninstall(LPCTSTR service_name)
{
	DWORD error_id = ERROR_SUCCESS;

	SC_HANDLE manager = nullptr;
	SC_HANDLE service = nullptr;

	SERVICE_STATUS service_status;

	manager = OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);

	if (manager == nullptr)
	{
		return GetLastError();
	}

	service = OpenService(manager, _T("TestService"), SERVICE_ALL_ACCESS);

	if (service == nullptr)
	{
		error_id = GetLastError();
	}
	else
	{
		if (QueryServiceStatus(service, &service_status) == FALSE)
		{
			return GetLastError();
		}

		if (service_status.dwCurrentState != SERVICE_STOPPED)
		{
			if (ControlService(service, SERVICE_CONTROL_STOP, &service_status) == FALSE)
			{
				return GetLastError();
			}

			Sleep(500);
		}

		if (DeleteService(service) == FALSE)
		{
			return GetLastError();
		}

		if (CloseServiceHandle(service) == FALSE)
		{
			return GetLastError();
		}
	}

	if (CloseServiceHandle(manager) == FALSE)
	{
		return GetLastError();
	}

	return error_id;
}