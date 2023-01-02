#pragma once
class FilterClient
{
public:
	FilterClient();
	~FilterClient();

	HRESULT Send(LPCWSTR msg);
	void StartRecvRoutine();
	void StopRecvRoutine();
private:
	void RecvRoutine();
	void Recv();
private:
	HANDLE port_handle;
	Message sent;
	Message sent_reply;
	Filter2UserWrapper recv;
	User2FilterWrapper recv_reply;

	std::thread* recv_routine;
	bool is_routine_running;
};