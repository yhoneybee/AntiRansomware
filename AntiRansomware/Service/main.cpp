#include "stdafx.h"
#include "Service.h"

constexpr LPCTSTR SERVICE_NAME = _T("TestService");
constexpr LPCTSTR SERVICE_DISPLAY_NAME = _T("TestServiceDisplay");
constexpr LPCTSTR SERVICE_DESCRIPTION_STR = _T("It is Description");

Service service;
HANDLE stop_event;

VOID WINAPI ServiceCtrlHandler(DWORD ctrl)
{
	switch (ctrl)
	{
	case SERVICE_CONTROL_PAUSE:
		service.ServiceStatus(SERVICE_PAUSED);
		break;
	case SERVICE_CONTROL_CONTINUE:
		service.ServiceStatus(SERVICE_RUNNING);
		break;
	case SERVICE_CONTROL_STOP:
		SetEvent(stop_event);
		service.ServiceStatus(SERVICE_STOPPED);
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	}
}

VOID WINAPI ServiceMain(DWORD argc, LPTSTR* argv)
{
	SERVICE_STATUS_HANDLE svc_status_handle;

	svc_status_handle = RegisterServiceCtrlHandler(
		SERVICE_NAME,
		ServiceCtrlHandler
	);

	if (svc_status_handle == nullptr)
	{
		PrintErrorMessage(GetLastError());
		return;
	}

	service.ServiceStatusHandle(svc_status_handle);

	service.ServiceStatus(SERVICE_RUNNING);

	stop_event = CreateEvent(
		nullptr,
		true,
		false,
		nullptr);

	if (stop_event == nullptr)
	{
		PrintErrorMessage(GetLastError());
		return;
	}

	WaitForSingleObject(stop_event, INFINITE);

	service.ServiceStatus(SERVICE_STOPPED);

	if (CloseHandle(stop_event) == false)
	{
		PrintErrorMessage(GetLastError());
	}
}

int APIENTRY _tmain(int argc, TCHAR* argv[])
{
	setlocale(LC_ALL, "korean");

	TCHAR module_name[MAX_PATH];
	GetModuleFileName(nullptr, module_name, _countof(module_name));

	if (argc > 0)
	{
		if (lstrcmpi(argv[1], _T("-i")) == false)
		{
			PrintErrorMessage(service.Install(SERVICE_NAME, SERVICE_DISPLAY_NAME, SERVICE_DESCRIPTION_STR, module_name, SERVICE_DEMAND_START), _T("Install -> %d, %s"));
		}
		else if (lstrcmpi(argv[1], _T("-u")) == false)
		{
			PrintErrorMessage(service.Uninstall(SERVICE_NAME), _T("Uninstall -> %d, %s"));
		}
		else if (lstrcmpi(argv[1], _T("-s")) == false)
		{
			PrintErrorMessage(service.Start(SERVICE_NAME), _T("Start -> %d, %s"));
		}
	}

	SERVICE_TABLE_ENTRY svc_table[] =
	{
		{(PTCH)SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)ServiceMain},
		{nullptr, nullptr}
	};

	if (StartServiceCtrlDispatcher(svc_table) == false)
	{
		PrintErrorMessage(GetLastError(), _T("SSCD -> %d, %s"));
	}

	return 0;
}