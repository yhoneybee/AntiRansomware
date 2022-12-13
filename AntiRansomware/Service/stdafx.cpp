#include "stdafx.h"

DWORD GetErrorMessage(DWORD error)
{
	return GetErrorMessage(error, _T("(%d) %s"));
}

DWORD GetErrorMessage(DWORD error, LPCTSTR format)
{
	DWORD error_id = ERROR_SUCCESS;

	LPVOID buffer = nullptr;

	error_id = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		error,
		MAKELANGID(
			LANG_NEUTRAL,
			SUBLANG_DEFAULT
		),
		(LPTSTR)&buffer,
		0,
		nullptr);

	if (error_id == FALSE)
	{
		return GetLastError();
	}

	size_t format_length = 0;
	size_t buffer_length = 0;

	HRESULT res_format_length = StringCchLength(format, STRSAFE_MAX_CCH, &format_length);
	if (FAILED(res_format_length))
	{
		return res_format_length;
	}

	HRESULT res_buffer_length = StringCchLength((LPTSTR)buffer, STRSAFE_MAX_CCH, &buffer_length);
	if (FAILED(res_buffer_length))
	{
		return res_buffer_length;
	}

	LPTSTR message = new TCHAR[format_length + buffer_length + INT_MAXIMUM_POWER + 1 + NULL_SPACE];
	HRESULT result = StringCchPrintf(message, STRSAFE_MAX_CCH, format, error_id, (LPTSTR)buffer);
	_tprintf_s(message);
	SAFE_DELETE_ARRAY(message);

	if (LocalFree(buffer) != nullptr)
	{
		return GetLastError();
	}

	if (FAILED(result))
	{
		return result;
	}

	return ERROR_SUCCESS;
}
