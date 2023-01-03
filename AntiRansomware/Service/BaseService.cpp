#include "stdafx.h"
#include "BaseService.h"

BaseService* BaseService::base_service_ = nullptr;

BaseService::BaseService(LPCTSTR service_name, LPCTSTR display_name, LPCTSTR description)
	:name_{ service_name }, display_{ display_name }, description_{ description }, sys_path_{ nullptr }, inf_path_{ nullptr }, service_status_{ 0 }, service_status_handle_{ 0 }, service_event_{ 0 }, argc_{ 0 }, argv_{ 0 }
{
}

BaseService::~BaseService()
{
	SAFE_DELETE_ARRAY(sys_path_);
}

void BaseService::SYSPath()
{
	TCHAR module_name[MAX_PATH];
	if (GetModuleFileName(nullptr, module_name, _countof(module_name)) == 0)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t GetModuleFileName -> %d, %s"));
		return;
	}

	SYSPath(module_name);
}

void BaseService::SYSPath(LPCTSTR sys_path)
{
	_tprintf_s(_T("[ ]\t SYSPath"));

	size_t length = 0;
	if (FAILED(StringCchLength(sys_path, MAX_PATH, &length)))
	{
		_tprintf_s(_T("[!]\t\t StringCchLength -> FAILED\n"));
		return;
	}

	SAFE_DELETE_ARRAY(sys_path_);
	sys_path_ = new TCHAR[MAX_PATH];
	if (FAILED(StringCchCopy(sys_path_, MAX_PATH, sys_path)))
	{
		_tprintf_s(_T("[!]\t\t StringCchCopy -> FAILED\n"));
		return;
	}

	_tprintf_s(_T("(%s)\n"), sys_path_);
}

void BaseService::INFPath()
{
	INFPath(name_);
}

void BaseService::INFPath(LPCTSTR inf_path)
{
	_tprintf_s(_T("[ ]\t INFPath"));

	TCHAR exe_path[MAX_PATH] = { 0, };

	if (GetModuleFileName(nullptr, exe_path, MAX_PATH) == false)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t GetModuleFileName -> %d, %s"));
		return;
	}

	if (PathRemoveFileSpec(exe_path) == false)
	{
		_tprintf_s(_T("[!]\t\t PathRemoveFileSpec -> FAILED\n"));
		return;
	}

	SAFE_DELETE_ARRAY(inf_path_);
	inf_path_ = new TCHAR[MAX_PATH];

	if (FAILED(StringCchPrintf(inf_path_, MAX_PATH, _T("%ws\\%ws.inf"), exe_path, inf_path)))
	{
		_tprintf_s(_T("[!]\t\t StringCchPrintf -> FAILED\n"));
		return;
	}

	_tprintf_s(_T("(%s)\n"), inf_path_);
}

void BaseService::InstallFromSYS(DWORD service_type, DWORD start_type)
{
	_tprintf_s(_T("[ ]\t InstallSYS(%s, )\n"), name_);

	if (sys_path_ == nullptr)
	{
		_tprintf_s(_T("[!]\t\t excution file path was null.\n"));
		return;
	}

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

	service = CreateService(
		manager,
		name_,
		display_,
		SERVICE_ALL_ACCESS,
		service_type,
		start_type,
		SERVICE_ERROR_NORMAL,
		sys_path_,
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

	SERVICE_DESCRIPTION service_description;
	service_description.lpDescription = (LPTSTR)description_;

	if (ChangeServiceConfig2(service, SERVICE_CONFIG_DESCRIPTION, &service_description) == false)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t ChangeServiceConfig2 -> %d, %s"));
		CloseServiceHandle(service);
		CloseServiceHandle(manager);
		return;
	}

	CloseServiceHandle(service);
	CloseServiceHandle(manager);

	_tprintf_s(_T("[ ]\t Installed.\n"));
}

void BaseService::InstallFromINF()
{
	_tprintf_s(_T("[ ]\t InstallINF(%s)\n"), name_);

	TCHAR dest[MAX_PATH + 31] = { 0, };

	if (FAILED(StringCchPrintf(dest, MAX_PATH, _T("DefaultInstall 132 %ws"), inf_path_)))
	{
		_tprintf_s(_T("[!]\t\t StringCchPrintf -> FAILED\n"));
		return;
	}

	InstallHinfSection(nullptr, nullptr, dest, 0);
}

void BaseService::Uninstall()
{
	_tprintf_s(_T("[ ]\t Uninstall(%s)\n"), name_);

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
		name_,
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

	_tprintf_s(_T("[ ]\t Uninstalled.\n"));
}

void BaseService::Start()
{
	_tprintf_s(_T("[ ]\t Start\n"));

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
		name_,
		SERVICE_ALL_ACCESS
	);

	if (service == nullptr)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t OpenService -> %d, %s"));
		CloseServiceHandle(manager);
		return;
	}

	if (StartService(service, 0, nullptr) == false)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t StartService -> %d, %s"));
		CloseServiceHandle(service);
		CloseServiceHandle(manager);
		return;
	}

	CloseServiceHandle(service);
	CloseServiceHandle(manager);

	OnStart();
}

void BaseService::Stop()
{
	_tprintf_s(_T("[ ]\t Stop\n"));

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
		name_,
		SERVICE_ALL_ACCESS
	);

	if (service == nullptr)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t OpenService -> %d, %s"));
		CloseServiceHandle(manager);
		return;
	}

	if (QueryServiceStatus(service, &service_status_) == false)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t QueryServiceStatus -> %d, %s"));
		CloseServiceHandle(service);
		CloseServiceHandle(manager);
		return;
	}

	if (service_status_.dwCurrentState != SERVICE_STOPPED)
	{
		if (ControlService(service, SERVICE_CONTROL_STOP, &service_status_) == false)
		{
			PrintErrorMessage(GetLastError(), _T("[!]\t\t ControlService -> %d, %s"));
			CloseServiceHandle(service);
			CloseServiceHandle(manager);
			return;
		}
		else
		{
			Sleep(2000);
		}
	}

	CloseServiceHandle(service);
	CloseServiceHandle(manager);

	OnStop();
}

void BaseService::ServiceStart()
{
	_tprintf_s(_T("[ ]\t ServiceStart\n"));

	ServiceReportStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

	OnStart();

	ServiceReportStatus(SERVICE_RUNNING, NO_ERROR, 0);
}

void BaseService::ServicePause()
{
	_tprintf_s(_T("[ ]\t ServicePause\n"));

	ServiceReportStatus(SERVICE_PAUSE_PENDING, NO_ERROR, 0);

	OnPause();

	ServiceReportStatus(SERVICE_PAUSED, NO_ERROR, 0);
}

void BaseService::ServiceContinue()
{
	_tprintf_s(_T("[ ]\t ServiceContinue\n"));

	ServiceReportStatus(SERVICE_CONTINUE_PENDING, NO_ERROR, 0);

	OnContinue();

	ServiceReportStatus(SERVICE_RUNNING, NO_ERROR, 0);
}

void BaseService::ServiceStop()
{
	_tprintf_s(_T("[ ]\t ServiceStop\n"));

	ServiceReportStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);

	OnStop();

	ServiceReportStatus(SERVICE_STOPPED, NO_ERROR, 0);
}

void BaseService::ServiceShutdown()
{
	_tprintf_s(_T("[ ]\t ServiceShutdown\n"));

	OnShutdown();

	ServiceReportStatus(SERVICE_STOPPED, NO_ERROR, 0);
}

void BaseService::ServiceReportStatus(DWORD current_status, DWORD win32_exit_code, DWORD wait_hint)
{
	_tprintf_s(_T("[ ]\t ServiceReportStatus\n"));

	static DWORD check_point = 1;

	service_status_.dwCurrentState = current_status;
	service_status_.dwWin32ExitCode = win32_exit_code;
	service_status_.dwWaitHint = wait_hint;

	service_status_.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	service_status_.dwControlsAccepted = SERVICE_ACCEPT_STOP;

	if (current_status == SERVICE_START_PENDING)
	{
		service_status_.dwControlsAccepted = 0;
	}

	if (current_status == SERVICE_RUNNING || current_status == SERVICE_STOPPED)
	{
		service_status_.dwCheckPoint = 0;
	}
	else
	{
		service_status_.dwCheckPoint = check_point++;
	}

	if (SetServiceStatus(service_status_handle_, &service_status_) == false)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t SetServiceStatus -> %d, %s"));
		return;
	}
}

bool BaseService::RunServiceMain(BaseService* service)
{
	_tprintf_s(_T("[ ]\t RunServiceMain\n"));

	base_service_ = service;

	SERVICE_TABLE_ENTRY entry_table[] =
	{
		{(LPTSTR)base_service_->name_, (LPSERVICE_MAIN_FUNCTION)ServiceMain},
		{nullptr,nullptr},
	};

	bool result = StartServiceCtrlDispatcher(entry_table);
	if (result == false)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t StartServiceCtrlDispatcher -> %d, %s"));
	}

	return result;
}

void BaseService::ServiceMain(DWORD argc, LPTSTR* argv)
{
	_tprintf_s(_T("[ ]\t ServiceMain\n"));

	base_service_->service_status_handle_ = RegisterServiceCtrlHandler(
		base_service_->name_,
		(LPHANDLER_FUNCTION)ServiceControlHandler
	);

	if (base_service_->service_status_handle_ == nullptr)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t RegisterServiceCtrlHandler -> %d, %s"));
		return;
	}

	base_service_->argc_ = argc;
	base_service_->argv_ = argv;

	base_service_->ServiceStart();
}

void BaseService::ServiceControlHandler(DWORD control)
{
	_tprintf_s(_T("[ ]\t ServiceControlHandler\n"));

	switch (control)
	{
	case SERVICE_CONTROL_PAUSE:
		base_service_->ServicePause();
		break;
	case SERVICE_CONTROL_CONTINUE:
		base_service_->ServiceContinue();
		break;
	case SERVICE_CONTROL_STOP:
		base_service_->ServiceStop();
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		base_service_->ServiceShutdown();
		break;
	}
}