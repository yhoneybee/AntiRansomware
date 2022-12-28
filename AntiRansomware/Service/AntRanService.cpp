#include "stdafx.h"
#include "AntRanService.h"

AntRanService::AntRanService()
	:BaseService(_T("AntRan"), _T("AntRan Scanner"), _T("Anti-Ransomware Service"))
{
	INFPath();
}

AntRanService::~AntRanService()
{
}

void AntRanService::OnStart()
{
}

void AntRanService::OnPause()
{
}

void AntRanService::OnContinue()
{
}

void AntRanService::OnStop()
{
}

void AntRanService::OnShutdown()
{
}
