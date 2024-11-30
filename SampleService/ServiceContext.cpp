#include "ServiceContext.h"
#include "../StaticLib1/Error/WinApi.h"
#include <stdexcept>
#include <string>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi")

ServiceContext::ServiceContext
( _Inout_ StaticLib1::RAII::WinApi::HANDLE<nullptr> && stop_event
, _In_    SERVICE_STATUS                               service_status      
, _In_    SERVICE_STATUS_HANDLE                        hServiceStatusHandle
) noexcept
	: stop_event{ std::move(stop_event) }
	, service_status {service_status}
	, hServiceStatusHandle{ hServiceStatusHandle }
{
}
DWORD WINAPI ServiceContext::HandlerEx
( DWORD  dwControl  
, DWORD  dwEventType
, LPVOID lpEventData
, LPVOID lpContext  
) noexcept
{
#define	this	static_cast<ServiceContext*>(lpContext)
	switch (dwControl) {
	case SERVICE_CONTROL_INTERROGATE:
		return NO_ERROR;
	case SERVICE_CONTROL_STOP       :
		if (SetEvent(this->stop_event.get()))
			return NO_ERROR;
		else
			return GetLastError();
	default                         :
		return ERROR_CALL_NOT_IMPLEMENTED;
	}
#undef	this
}
void ServiceContext::SetServiceStatus()
{
	Error_WinApi_Check(::SetServiceStatus(hServiceStatusHandle, &service_status));
}
void ServiceContext::run()
{
	SetServiceStatus();

	wchar_t szCommandLine[MAX_PATH];
	GetModuleFileNameW(nullptr, szCommandLine, _countof(szCommandLine));
	lstrcpyW(PathFindFileNameW(szCommandLine), _CRT_WIDE(_CRT_STRINGIZE(SampleServer) "." _CRT_STRINGIZE(exe)));
	PathQuoteSpacesW(szCommandLine);

	STARTUPINFOW        StartupInfo
	{ sizeof StartupInfo
	};
	PROCESS_INFORMATION ProcessInformation;

	Error_WinApi_Check(/*WINBASEAPI BOOL WINAPI*/CreateProcessW
	( /*_In_opt_    LPCWSTR               lpApplicationName   */nullptr
	, /*_Inout_opt_ LPWSTR                lpCommandLine       */szCommandLine
	, /*_In_opt_    LPSECURITY_ATTRIBUTES lpProcessAttributes */nullptr
	, /*_In_opt_    LPSECURITY_ATTRIBUTES lpThreadAttributes  */nullptr
	, /*_In_        BOOL                  bInheritHandles     */false
	, /*_In_        DWORD                 dwCreationFlags     */0UL
	, /*_In_opt_    LPVOID                lpEnvironment       */nullptr
	, /*_In_opt_    LPCWSTR               lpCurrentDirectory  */nullptr
	, /*_In_        LPSTARTUPINFOW        lpStartupInfo       */&StartupInfo
	, /*_Out_       LPPROCESS_INFORMATION lpProcessInformation*/&ProcessInformation
	));
	StaticLib1::RAII::Deleter::WinApi::CloseHandle{}(ProcessInformation.hThread);
	Error_WinApi_CheckNot(WaitForSingleObject(StaticLib1::RAII::WinApi::HANDLE<nullptr>(ProcessInformation.hProcess).get(), INFINITE), WAIT_FAILED);

	service_status.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus();
}
