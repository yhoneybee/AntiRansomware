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

	manager = OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);

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
		SERVICE_DESCRIPTION service_description;
		service_description.lpDescription = (LPTSTR)description;

		if (ChangeServiceConfig2(service, SERVICE_CONFIG_DESCRIPTION, &service_description) == FALSE)
		{
			// 설명 설정 실패에 대한 처리
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

	service = OpenService(manager, service_name, SERVICE_ALL_ACCESS);

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