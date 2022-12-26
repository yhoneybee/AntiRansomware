#include "stdafx.h"
#include "TestService.h"

TestService::TestService()
	: BaseService(_T("TestService"), _T("TestServiceDisplay"), _T("It is test service."))
{
	SYSPath(_T("C:\\Windows\\System32\\drivers\\MiniFilter.sys"));
	INFPath(_T("C:\\Windows\\System32\\drivers\\MiniFilter.inf"));
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
