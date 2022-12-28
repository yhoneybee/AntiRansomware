#pragma once
class FilterClient
{
public:
	FilterClient();
	~FilterClient();

	HRESULT Send(LPCWSTR msg);
private:
	HANDLE port_handle;
};