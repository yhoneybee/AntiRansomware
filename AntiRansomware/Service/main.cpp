#include "stdafx.h"
#include "TestService.h"

#if _WIN64
EXTERN_C int APIENTRY _tmain(DWORD argc, LPTSTR* argv)
#else
int main(DWORD argc, LPTSTR* argv)
#endif
{
	setlocale(LC_ALL, "korean");

	_tprintf_s(_T("[ ] _tmain(%d)\n"), argc);

	TestService test_service;
	if (argc == 1)
	{
		BaseService::RunServiceMain(&test_service);
	}
	else if (argc == 2)
	{
		if (lstrcmpi(argv[1], _T("-i")) == false)
		{
			test_service.Install(SERVICE_KERNEL_DRIVER, SERVICE_BOOT_START);
		}
		else if (lstrcmpi(argv[1], _T("-u")) == false)
		{
			test_service.Uninstall();
		}
	}

	return 0;
}