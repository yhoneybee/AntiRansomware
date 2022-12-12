#include "stdafx.h"
#include "Service.h"

int _tmain(int argc, TCHAR* argv[])
{
	DWORD error = ERROR_SUCCESS;

	Service service;

	TCHAR module_name[MAX_PATH];
	GetModuleFileName(nullptr, module_name, _countof(module_name));
	error = service.Install(module_name, SERVICE_DEMAND_START);

	std::cout << error << "\n";

	error = service.Uninstall(_T("TestService"));

	std::cout << error << "\n";

	return 0;
}