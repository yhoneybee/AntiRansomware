#pragma once
#include "BaseService.h"
class TestService : public BaseService
{
public:
	TestService(
		LPCTSTR service_name,
		LPCTSTR display_name,
		LPCTSTR description);

	TestService(
		LPCTSTR service_name,
		LPCTSTR display_name,
		LPCTSTR description,
		LPCTSTR exe_path
	);

	~TestService();
public:
	virtual void OnStart() override;
	virtual void OnPause() override;
	virtual void OnContinue() override;
	virtual void OnStop() override;
	virtual void OnShutdown() override;
};

