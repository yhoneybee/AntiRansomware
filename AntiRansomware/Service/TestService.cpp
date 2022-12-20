#include "stdafx.h"
#include "TestService.h"

TestService::TestService()
	: BaseService(_T("TestService"), _T("TestServiceDisplay"), _T("It is test service."))
{
	ExePath(_T("C:\\Windows\\System32\\drivers\\KeyboardFilterDriver.sys"));
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
