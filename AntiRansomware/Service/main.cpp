#include "stdafx.h"
#include "TestService.h"

int APIENTRY _tmain(DWORD argc, LPTSTR* argv)
{
	setlocale(LC_ALL, "korean");

	_tprintf_s(_T("[ ] _tmain\n"));

	TestService test_service(_T("TestService"), _T("TestServiceDisplay"), _T("Desc"));
	if (argc == 1)
	{
		BaseService::RunServiceMain(&test_service);
	}
	else if (argc > 0)
	{
		if (lstrcmpi(argv[1], _T("-i")) == false)
		{
			test_service.Install();
		}
		else if (lstrcmpi(argv[1], _T("-u")) == false)
		{
			test_service.Uninstall();
		}
	}

	return 0;
}