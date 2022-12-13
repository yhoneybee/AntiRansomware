#include "stdafx.h"
#include "Service.h"

int _tmain(int argc, TCHAR* argv[])
{
	setlocale(LC_ALL, "korean");

	DWORD error_id = ERROR_SUCCESS;

	Service service;

	TCHAR module_name[MAX_PATH];
	GetModuleFileName(nullptr, module_name, _countof(module_name));

	error_id = service.Install(module_name, SERVICE_DEMAND_START);

	GetErrorMessage(error_id);

	error_id = service.Uninstall(_T("TestService"));

	GetErrorMessage(error_id);

	return 0;
}