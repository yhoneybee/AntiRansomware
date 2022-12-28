#include "pch.h"
#include "FilterClient.h"
#include <SharedData.h>

FilterClient::FilterClient()
{
	if (IS_ERROR(FilterConnectCommunicationPort(PORT_NAME_W, 0, nullptr, 0, nullptr, &port_handle)))
	{
		MY_LOG("FilterConnectCommunicationPort function was failed.");
		return;
	}

	Send(L"HELLO~?");
}

FilterClient::~FilterClient()
{
}

HRESULT FilterClient::Send(LPCWSTR msg)
{
	HRESULT result = SEVERITY_SUCCESS;

	Message reply;
	ZeroMemory(&reply, sizeof(Message));

	DWORD returned_bytes = 0;

	result = FilterSendMessage(port_handle, &msg, lstrlen(msg), &reply, sizeof(reply), &returned_bytes);

	if (IS_ERROR(result))
	{
		MY_LOG("FilterSendMessage function was failed.");
		return result;
	}

	return result;
}
