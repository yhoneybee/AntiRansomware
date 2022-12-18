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

	TestService test_service(_T("TestService"), _T("TestServiceDisplay"), _T("Desc"));
	if (argc == 1)
	{
		BaseService::RunServiceMain(&test_service);
	}
	else if (argc > 0)
	{
		if (argc == 4)
		{
			if (lstrcmpi(argv[2], _T("-p")) == false)
			{
				test_service.ExePath(argv[3]);
			}
		}

		if (argc == 2 || argc == 4)
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
	}

	return 0;
}