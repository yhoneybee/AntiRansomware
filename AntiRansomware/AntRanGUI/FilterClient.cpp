#include "pch.h"
#include "FilterClient.h"

FilterClient::FilterClient()
	:sent{ 0 }, sent_reply{ 0 }, recv{ 0 }, recv_reply{ 0 }, recv_routine{ nullptr }, is_routine_running{ false }
{
	HRESULT result = FilterConnectCommunicationPort(PORT_NAME, 0, nullptr, 0, nullptr, &port_handle);

	if (IS_ERROR(result))
	{
		MY_MESSAGEBOX("FilterConnectCommunicationPort function was failed(0x%x).", result);
		return;
	}

	StartRecvRoutine();

	Send(L"HELLO~?");
}

FilterClient::~FilterClient()
{
	if (port_handle == nullptr)
	{
		return;
	}

	StopRecvRoutine();

	HRESULT result = FilterClose(port_handle);

	if (IS_ERROR(result))
	{
		MY_MESSAGEBOX("FilterClose function was failed(0x%x).", result);
	}
}

HRESULT FilterClient::Send(LPCWSTR msg)
{
	HRESULT result = SEVERITY_SUCCESS;

	ZeroMemory(&sent, sizeof(sent));
	StringCchCopyW(sent.data, BUF_SIZE, msg);

	ZeroMemory(&sent_reply, sizeof(sent_reply));

	DWORD returned_bytes = 0;

	result = FilterSendMessage(port_handle, &sent, sizeof(sent), &sent_reply, sizeof(sent_reply), &returned_bytes);

	if (IS_ERROR(result))
	{
		MY_MESSAGEBOX("FilterSendMessage function was failed.");
		return result;
	}

	return result;
}

void FilterClient::StartRecvRoutine()
{
	if (is_routine_running)
	{
		return;
	}

	is_routine_running = true;

	recv_routine = new std::thread([this]() { FilterClient::RecvRoutine(); });
}

void FilterClient::StopRecvRoutine()
{
	if (!is_routine_running)
	{
		return;
	}

	is_routine_running = false;

	recv_routine->join();

	delete recv_routine;
	recv_routine = nullptr;
}

void FilterClient::Recv()
{
	HRESULT result = SEVERITY_SUCCESS;

	result = FilterGetMessage(port_handle, &recv.hdr, sizeof(recv), nullptr);
	if (IS_ERROR(result))
	{
		MY_LOG("FilterGetMessage function was failed(0x%x).", result);
		return;
	}

	ZeroMemory(&recv_reply, sizeof(recv_reply));

	DWORD returned_bytes = 0;

	_wcsupr(recv.data.path);
	if (wcsstr(recv.data.path, L".BEE"))
	{
		recv_reply.data.block = true;
		MY_LOG("%ws file blocked", recv.data.path);
	}
	recv_reply.hdr.MessageId = recv.hdr.MessageId;

	result = FilterReplyMessage(port_handle, &recv_reply.hdr, sizeof(recv_reply.hdr) + sizeof(recv_reply.data));
	if (IS_ERROR(result))
	{
		MY_LOG("FilterReplyMessage function was failed(0x%x).", result);
		return;
	}
}

void FilterClient::RecvRoutine()
{
	while (is_routine_running)
	{
		Recv();
	}
}
