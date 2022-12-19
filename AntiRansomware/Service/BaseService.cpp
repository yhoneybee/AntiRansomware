#include "stdafx.h"
#include "BaseService.h"

BaseService* BaseService::base_service_ = nullptr;

BaseService::BaseService(LPCTSTR service_name, LPCTSTR display_name, LPCTSTR description)
	:name_{ service_name }, display_{ display_name }, description_{ description }, exe_path_{ nullptr }, service_status_{ 0 }, service_status_handle_{ 0 }, service_event_{ 0 }, argc_{ 0 }, argv_{ 0 }
{
}

BaseService::~BaseService()
{
	SAFE_DELETE_ARRAY(exe_path_);
}

void BaseService::ExePath()
{
	TCHAR module_name[MAX_PATH];
	if (GetModuleFileName(nullptr, module_name, _countof(module_name)) == 0)
	{
		PrintErrorMessage(GetLastError(), _T("[!]\t\t GetModuleFileName -> %d, %s"));
		return;
	}

	ExePath(module_name);
}

void BaseService::ExePath(LPCTSTR exe_path)
{
	_tprintf_s(_T("[ ]\t ExePath"));

	size_t length = 0;
	if (FAILED(StringCchLength(exe_path, MAX_PATH, &length)))
	{
		_tprintf_s(_T("[!]\t\t StringCchLength -> FAILED\n"));
		return;
	}

	SAFE_DELETE_ARRAY(exe_path_);
	exe_path_ = new TCHAR[MAX_PATH];
	if (FAILED(StringCchCopy(exe_path_, MAX_PATH, exe_path)))
	{
		_tprintf_s(_T("[!]\t\t StringCchCopy -> FAILED\n"));
		return;
	}

	_tprintf_s(_T("(%s)\n"), exe_path_);
}

void BaseService::Install(DWORD service_type, DWORD start_type)
{
	_tprintf_s(_T("[ ]\t Install(%s)\n"), name_);

	if (exe_path_ == nullptr)
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
		exe_path_,
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

	ServiceReportStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

	OnStart();

	ServiceReportStatus(SERVICE_RUNNING, NO_ERROR, 0);
}

void BaseService::Pause()
{
	_tprintf_s(_T("[ ]\t Pause\n"));

	ServiceReportStatus(SERVICE_PAUSE_PENDING, NO_ERROR, 0);

	OnPause();

	ServiceReportStatus(SERVICE_PAUSED, NO_ERROR, 0);
}

void BaseService::Continue()
{
	_tprintf_s(_T("[ ]\t Continue\n"));

	ServiceReportStatus(SERVICE_CONTINUE_PENDING, NO_ERROR, 0);

	OnContinue();

	ServiceReportStatus(SERVICE_RUNNING, NO_ERROR, 0);
}

void BaseService::Stop()
{
	_tprintf_s(_T("[ ]\t Stop\n"));

	ServiceReportStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);

	OnStop();

	ServiceReportStatus(SERVICE_STOPPED, NO_ERROR, 0);
}

void BaseService::Shutdown()
{
	_tprintf_s(_T("[ ]\t Shutdown\n"));

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

	base_service_->Start();
}

void BaseService::ServiceControlHandler(DWORD control)
{
	_tprintf_s(_T("[ ]\t ServiceControlHandler\n"));

	switch (control)
	{
	case SERVICE_CONTROL_PAUSE:
		base_service_->Pause();
		break;
	case SERVICE_CONTROL_CONTINUE:
		base_service_->Continue();
		break;
	case SERVICE_CONTROL_STOP:
		base_service_->Stop();
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		base_service_->Shutdown();
		break;
	}
}