#pragma once
#include "stdafx.h"

class Service
{
public:
	Service() {}
	~Service() {}
public:
	DWORD Install(LPCTSTR service_name, LPCTSTR path_name, DWORD start_type);
	DWORD Install(LPCTSTR service_name, LPCTSTR display_name, LPCTSTR path_name, DWORD start_type);
	DWORD Install(LPCTSTR service_name, LPCTSTR display_name, LPCTSTR description, LPCTSTR path_name, DWORD start_type);
	DWORD Uninstall(LPCTSTR service_name);
private:
private:
};