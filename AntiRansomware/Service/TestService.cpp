#include "stdafx.h"
#include "TestService.h"

TestService::TestService(LPCTSTR service_name, LPCTSTR display_name, LPCTSTR description)
	:BaseService(service_name, display_name, description)
{
}

TestService::~TestService()
{
}

void TestService::OnStart()
{
}

void TestService::OnPause()
{
}

void TestService::OnContinue()
{
}

void TestService::OnStop()
{
}

void TestService::OnShutdown()
{
}
