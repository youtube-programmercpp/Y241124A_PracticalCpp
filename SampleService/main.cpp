#include "ServiceName.h"
#include "DebugService.h"
#include "ServiceContext.h"
#include "../StaticLib1/RAII/WinApi/CloseHandle.h"
#include "../SampleServer/NameOfStopEvent.h"
#include "../StaticLib1/Error/WinApi.h"
#include <stdexcept>
#include <string>


static VOID WINAPI ServiceMain
( DWORD   dwNumServicesArgs
, LPWSTR* lpServiceArgVectors
) noexcept
{
	try {
		ServiceContext context
		{ Error_WinApi_Check(CreateEventW(nullptr, true, false, _CRT_WIDE(SampleServer_NameOfStopEvent)))
		,
			{ /*DWORD dwServiceType            ;*/SERVICE_WIN32_OWN_PROCESS
			, /*DWORD dwCurrentState           ;*/SERVICE_RUNNING
			, /*DWORD dwControlsAccepted       ;*/SERVICE_ACCEPT_STOP
			, /*DWORD dwWin32ExitCode          ;*/NO_ERROR
			, /*DWORD dwServiceSpecificExitCode;*/0UL
			, /*DWORD dwCheckPoint             ;*/0UL
			, /*DWORD dwWaitHint               ;*/0UL
			}
		, Error_WinApi_Check(/*_Must_inspect_result_ WINADVAPI SERVICE_STATUS_HANDLE WINAPI*/RegisterServiceCtrlHandlerExW
			( /*_In_            LPCWSTR               lpServiceName*/_CRT_WIDE(SampleService_ServiceName)
			, /*_In_ __callback LPHANDLER_FUNCTION_EX lpHandlerProc*/ServiceContext::HandlerEx
			, /*_In_opt_        LPVOID                lpContext    */&context
			))
		};
		context.run();
	}
	catch (const std::exception& e) {
		OutputDebugStringA(e.what());
		OutputDebugStringA("\n");
	}
}
int main() noexcept
{
	try {
		if (const StaticLib1::RAII::WinApi::HANDLE<nullptr> start_event{ OpenEventW(MAXIMUM_ALLOWED, false, _CRT_WIDE(SampleService_DebugService)) }) {
			switch (const auto dwResult = WaitForSingleObject(start_event.get(), INFINITE)) {
			case WAIT_OBJECT_0:
				break;
			case WAIT_FAILED:
				throw std::runtime_error{ Error::Source{ _CRT_STRINGIZE(WaitForSingleObject) }.winapi_error_message    (GetLastError()) };
			default:
				throw std::runtime_error{ Error::Source{ _CRT_STRINGIZE(WaitForSingleObject) }.unexpected_value_message(dwResult      ) };
			}
		}
		else {
			switch (const auto e = GetLastError()) {
			case ERROR_FILE_NOT_FOUND:
				break;
			default:
				throw std::runtime_error{ Error::Source{ _CRT_STRINGIZE(OpenEventW) }.winapi_error_message(e) };
			}
		}
		static CONST SERVICE_TABLE_ENTRYW rgServiceStartTable[]
		{ { /*LPWSTR                   lpServiceName;*/const_cast<LPWSTR>(_CRT_WIDE(SampleService_ServiceName))
		  , /*LPSERVICE_MAIN_FUNCTIONW lpServiceProc;*/ServiceMain
		  }
		, {}
		};
		if (/*WINADVAPI BOOL WINAPI*/StartServiceCtrlDispatcherW
		( /*_In_ CONST  SERVICE_TABLE_ENTRYW * lpServiceStartTable*/rgServiceStartTable
		))
			return EXIT_SUCCESS;
		else {
			switch (const auto e = GetLastError()) {
			case ERROR_FAILED_SERVICE_CONTROLLER_CONNECT:
				return EXIT_FAILURE;
			default:
				throw std::runtime_error{ Error::Source{ _CRT_STRINGIZE(StartServiceCtrlDispatcherW) }.winapi_error_message(e) };
			}
		}
	}
	catch (const std::exception& e) {
		OutputDebugStringA(e.what());
		OutputDebugStringA("\n");
		return EXIT_FAILURE;
	}
}
