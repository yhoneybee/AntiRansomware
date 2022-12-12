#pragma once
#include "stdafx.h"

class Service
{
public:
	Service() {}
	~Service() {}
public:
	DWORD Install(LPCTSTR path_name, DWORD start_type);
	DWORD Uninstall(LPCTSTR service_name);
private:
private:
};