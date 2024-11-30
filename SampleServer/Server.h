#pragma once
#include "../StaticLib1/RAII/WinApi/CloseHandle.h"
class Server
{
	StaticLib1::RAII::WinApi::HANDLE<nullptr             >  parameter_extract_event;
	StaticLib1::RAII::WinApi::HANDLE<INVALID_HANDLE_VALUE>  named_pipe_handle      ;
	unsigned ThreadProc() noexcept;
	void on_connect();
public:
	Server(StaticLib1::RAII::WinApi::HANDLE<nullptr>&& parameter_extract_event) noexcept;
	void run();
};
