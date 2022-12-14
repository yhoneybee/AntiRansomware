#include "stdafx.h"

constexpr LPCTSTR SERVICE_NAME = _T("TestService");
constexpr LPCTSTR SERVICE_DISPLAY_NAME = _T("TestServiceDisplay");
constexpr LPCTSTR SERVICE_DESCRIPTION_STR = _T("It is Description");

SERVICE_STATUS service_status;
SERVICE_STATUS_HANDLE service_status_handle;
HANDLE service_event;

void WINAPI ServiceMain(DWORD argc, LPTSTR* argv);
void WINAPI ServiceControlHandler(DWORD control);
void ServiceReportStatus(DWORD current_status, DWORD win32_exit_code, DWORD wait_hint);

void ServiceInit(DWORD argc, LPTSTR* argv);
void ServiceInstall();
void ServiceDelete();
void ServiceStart();
void ServiceStop();

int APIENTRY _tmain(int argc, TCHAR* argv[])
{
	setlocale(LC_ALL, "korean");

	_tprintf_s(_T("\t _tmain\n"));

	if (argc == 1)
	{
		SERVICE_TABLE_ENTRY entry_table[] =
		{
			{(LPTSTR)SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)ServiceMain},
			{nullptr,nullptr},
		};

		if (StartServiceCtrlDispatcher(entry_table) == false)
		{
			PrintErrorMessage(GetLastError(), _T("[!]\t\t StartServiceCtrlDispatcher -> %d, %s"));
		}
	}
	else if (argc > 0)
	{
		if (lstrcmpi(argv[1], _T("-i")) == false)
		{
			ServiceInstall();
		}
		else if (lstrcmpi(argv[1], _T("-u")) == false)
		{
			ServiceDelete();
		}
		else if (lstrcmpi(argv[1], _T("-st")) == false)
		{
			ServiceStart();
		}
		else if (lstrcmpi(argv[1], _T("-sp")) == false)
		{
			ServiceStop();
		}
	}

	return 0;
}

void WINAPI ServiceMain(DWORD argc, LPTSTR* argv)
{
	_tprintf_s(_T("\t ServiceMain\n"));

	service_status_handle = RegisterServiceCtrlHandler(
		SERVICE_NAME,
		ServiceControlHandler
	);

	if (service_status_handle == nullptr)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t RegisterServiceCtrlHandler -> %d, %s"));
		return;
	}

	service_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	service_status.dwServiceSpecificExitCode = 0;

	ServiceReportStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

	if (SetServiceStatus(service_status_handle, &service_status) == false)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t SetServiceStatus -> %d, %s"));
		return;
	}

	ServiceInit(argc, argv);
}

void WINAPI ServiceControlHandler(DWORD control)
{
	_tprintf_s(_T("\t ServiceControlHandler\n"));

	switch (control)
	{
	case SERVICE_CONTROL_STOP:
	{
		ServiceReportStatus(SERVICE_STOPPED, NO_ERROR, 0);
		break;
	}
	}
}

void ServiceReportStatus(DWORD current_status, DWORD win32_exit_code, DWORD wait_hint)
{
	_tprintf_s(_T("\t ServiceReportStatus\n"));

	static DWORD check_point = 1;

	service_status.dwCurrentState = current_status;
	service_status.dwWin32ExitCode = win32_exit_code;
	service_status.dwWaitHint = wait_hint;

	service_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;

	if (current_status == SERVICE_START_PENDING)
	{
		service_status.dwControlsAccepted = 0;
	}

	if (current_status == SERVICE_RUNNING || current_status == SERVICE_STOPPED)
	{
		service_status.dwCheckPoint = 0;
	}
	else
	{
		service_status.dwCheckPoint = check_point++;
	}

	if (SetServiceStatus(service_status_handle, &service_status) == false)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t SetServiceStatus -> %d, %s"));
		return;
	}
}

void ServiceInit(DWORD argc, LPTSTR* argv)
{
	_tprintf_s(_T("\t ServiceInit\n"));

	service_event = CreateEvent(
		nullptr,
		true,
		false,
		nullptr
	);

	if (service_event == nullptr)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t CreateEvent -> %d, %s"));
		ServiceReportStatus(SERVICE_STOPPED, NO_ERROR, 0);
	}
	else
	{
		ServiceReportStatus(SERVICE_RUNNING, NO_ERROR, 0);

		WaitForSingleObject(service_event, INFINITE);
		ServiceReportStatus(SERVICE_STOPPED, NO_ERROR, 0);
	}
}

void ServiceInstall()
{
	_tprintf_s(_T("\t ServiceInstall\n"));

	SC_HANDLE manager = nullptr;
	SC_HANDLE service = nullptr;

	TCHAR module_name[MAX_PATH];
	if (GetModuleFileName(nullptr, module_name, _countof(module_name)) == 0)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t GetModuleFileName -> %d, %s"));
		return;
	}

	manager = OpenSCManager(
		nullptr,
		nullptr,
		SC_MANAGER_ALL_ACCESS
	);

	if (manager == nullptr)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t OpenSCManager -> %d, %s"));
		return;
	}

	service = CreateService(
		manager,
		SERVICE_NAME,
		SERVICE_DISPLAY_NAME,
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL,
		module_name,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr);

	if (service == nullptr)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t CreateService -> %d, %s"));
		CloseServiceHandle(manager);
		return;
	}

	CloseServiceHandle(service);
	CloseServiceHandle(manager);
}

void ServiceDelete()
{
	_tprintf_s(_T("\t ServiceDelete\n"));

	SC_HANDLE manager = nullptr;
	SC_HANDLE service = nullptr;

	manager = OpenSCManager(
		nullptr,
		nullptr,
		SC_MANAGER_ALL_ACCESS
	);

	if (manager == nullptr)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t OpenSCManager -> %d, %s"));
		return;
	}

	service = OpenService(
		manager,
		SERVICE_NAME,
		SERVICE_ALL_ACCESS
	);

	if (service == nullptr)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t OpenService -> %d, %s"));
		CloseServiceHandle(manager);
		return;
	}

	if (DeleteService(service) == false)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t DeleteService -> %d, %s"));
		return;
	}

	CloseServiceHandle(service);
	CloseServiceHandle(manager);
}

void ServiceStart()
{
	_tprintf_s(_T("\t ServiceStart\n"));

	SC_HANDLE manager = nullptr;
	SC_HANDLE service = nullptr;

	SERVICE_STATUS_PROCESS service_status_process;

	DWORD bytes_needed;

	manager = OpenSCManager(
		nullptr,
		nullptr,
		SC_MANAGER_ALL_ACCESS
	);

	if (manager == nullptr)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t OpenSCManager -> %d, %s"));
		return;
	}

	service = OpenService(
		manager,
		SERVICE_NAME,
		SERVICE_ALL_ACCESS
	);

	if (service == nullptr)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t OpenService -> %d, %s"));
		CloseServiceHandle(manager);
		return;
	}

	if (QueryServiceStatusEx(
		service,
		SC_STATUS_PROCESS_INFO,
		(PBYTE)&service_status_process,
		sizeof(SERVICE_STATUS_PROCESS),
		&bytes_needed) == false)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t QueryServiceStatusEx -> %d, %s"));
		CloseServiceHandle(service);
		CloseServiceHandle(manager);
		return;
	}

	if (service_status_process.dwCurrentState != SERVICE_STOPPED &&
		service_status_process.dwCurrentState != SERVICE_STOP_PENDING)
	{
		_tprintf_s(_T("[!]\t\t service was already running.\n"));
		return;
	}

	if (StartService(service, 0, nullptr) == false)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t StartService -> %d, %s"));
		CloseServiceHandle(service);
		CloseServiceHandle(manager);
	}

	CloseServiceHandle(service);
	CloseServiceHandle(manager);
}

void ServiceStop()
{
	_tprintf_s(_T("\t ServiceStop\n"));

	SC_HANDLE manager = nullptr;
	SC_HANDLE service = nullptr;

	SERVICE_STATUS_PROCESS service_status_process;

	DWORD bytes_needed;

	manager = OpenSCManager(
		nullptr,
		nullptr,
		SC_MANAGER_ALL_ACCESS
	);

	if (manager == nullptr)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t OpenSCManager -> %d, %s"));
		return;
	}

	service = OpenService(
		manager,
		SERVICE_NAME,
		SERVICE_ALL_ACCESS
	);

	if (service == nullptr)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t OpenService -> %d, %s"));
		CloseServiceHandle(manager);
		return;
	}

	if (QueryServiceStatusEx(
		service,
		SC_STATUS_PROCESS_INFO,
		(PBYTE)&service_status_process,
		sizeof(SERVICE_STATUS_PROCESS),
		&bytes_needed) == false)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t QueryServiceStatusEx -> %d, %s"));
		CloseServiceHandle(service);
		CloseServiceHandle(manager);
		return;
	}

	if (ControlService(
		service,
		SERVICE_CONTROL_STOP,
		(LPSERVICE_STATUS)&service_status_process) == false)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t ControlService -> %d, %s"));
		CloseServiceHandle(service);
		CloseServiceHandle(manager);
	}

	CloseServiceHandle(service);
	CloseServiceHandle(manager);
}
