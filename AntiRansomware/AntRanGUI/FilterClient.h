#pragma once
class FilterClient
{
public:
	FilterClient();
	~FilterClient();

	HRESULT Send(LPCWSTR msg);
	void Recv();
private:
	HANDLE port_handle;
	Message sent;
	Message sent_reply;
	Filter2UserWrapper recv;
	User2FilterWrapper recv_reply;
};