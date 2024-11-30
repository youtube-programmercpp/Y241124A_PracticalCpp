#pragma once
#include "../StaticLib1/RAII/WinApi/CloseHandle.h"
class ServiceContext
{
	StaticLib1::RAII::WinApi::HANDLE<nullptr> stop_event          ;
	SERVICE_STATUS           service_status      ;
	SERVICE_STATUS_HANDLE    hServiceStatusHandle;
public:
	ServiceContext
	( _Inout_ StaticLib1::RAII::WinApi::HANDLE<nullptr> && stop_event          
	, _In_    SERVICE_STATUS                               service_status      
	, _In_    SERVICE_STATUS_HANDLE                        hServiceStatusHandle
	) noexcept;
	static DWORD WINAPI HandlerEx
	( DWORD  dwControl  
	, DWORD  dwEventType
	, LPVOID lpEventData
	, LPVOID lpContext  
	) noexcept;
	void run();
	void SetServiceStatus();
};
