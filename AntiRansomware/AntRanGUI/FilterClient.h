#pragma once
class FilterClient
{
public:
	FilterClient();
	~FilterClient();

	void Connect();
	void Disconnect();
	HRESULT Send(LPCWSTR msg);

	void StartRecvRoutine();
	void StopRecvRoutine();

	void SetBlockedExtend(LPCWSTR extend);
private:
	void RecvRoutine();
	void Recv();
private:
	HANDLE port_handle;
	Message sent;
	Message sent_reply;
	Filter2UserWrapper recv;
	User2FilterWrapper recv_reply;

	TCHAR blocked_extend[MAX_PATH];

	std::thread* recv_routine;
	bool is_routine_running;
};