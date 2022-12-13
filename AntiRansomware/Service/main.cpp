#include "stdafx.h"
#include "Service.h"

void Test()
{
	DWORD error_id = ERROR_SUCCESS;

	Service service;

	TCHAR module_name[MAX_PATH];
	GetModuleFileName(nullptr, module_name, _countof(module_name));

	error_id = service.Install(_T("TestService"), _T("TestServiceDisplay"), _T("It is Description"), module_name, SERVICE_DEMAND_START);

	PrintErrorMessage(error_id);

	error_id = service.Uninstall(_T("TestService"));

	PrintErrorMessage(error_id);
}

int _tmain(int argc, TCHAR* argv[])
{
	setlocale(LC_ALL, "korean");

	Test();

	return 0;
}