#include "../StaticLib1/RAII/WinApi/CloseServiceHandle.h"
#include "../SampleService/ServiceName.h"
#include "../StaticLib1/Error/WinApi.h"
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi")
#include <stdexcept>
#include <string>
StaticLib1::RAII::WinApi::SC_HANDLE change_service_config(StaticLib1::RAII::WinApi::SC_HANDLE&& hService, _In_ LPCWSTR lpBinaryPathName)
{
	Error_WinApi_Check(/*WINADVAPI BOOL WINAPI*/ChangeServiceConfigW
	( /*_In_      SC_HANDLE hService          */hService.get()
	, /*_In_      DWORD     dwServiceType     */SERVICE_WIN32_OWN_PROCESS
	, /*_In_      DWORD     dwStartType       */SERVICE_DEMAND_START
	, /*_In_      DWORD     dwErrorControl    */SERVICE_ERROR_NORMAL
	, /*_In_opt_  LPCWSTR   lpBinaryPathName  */lpBinaryPathName
	, /*_In_opt_  LPCWSTR   lpLoadOrderGroup  */nullptr
	, /*_Out_opt_ LPDWORD   lpdwTagId         */nullptr
	, /*_In_opt_  LPCWSTR   lpDependencies    */nullptr
	, /*_In_opt_  LPCWSTR   lpServiceStartName*/nullptr
	, /*_In_opt_  LPCWSTR   lpPassword        */nullptr
	, /*_In_opt_  LPCWSTR   lpDisplayName     */nullptr
	));
	return std::move(hService);
}
StaticLib1::RAII::WinApi::SC_HANDLE open_service(_In_ SC_HANDLE hSCManager)
{
	return
	{ Error_WinApi_Check(/*_Must_inspect_result_ WINADVAPI SC_HANDLE WINAPI*/OpenServiceW
	( /*_In_ SC_HANDLE hSCManager     */hSCManager
	, /*_In_ LPCWSTR   lpServiceName  */_CRT_WIDE(SampleService_ServiceName)
	, /*_In_ DWORD     dwDesiredAccess*/MAXIMUM_ALLOWED
	))
	};
}
StaticLib1::RAII::WinApi::SC_HANDLE install_service(_In_ SC_HANDLE hSCManager)
{
	wchar_t szBinaryPathName[MAX_PATH];
	(void)GetModuleFileNameW(nullptr, szBinaryPathName, _countof(szBinaryPathName));
	lstrcpyW(PathFindFileNameW(szBinaryPathName), L"SampleService.exe");
	(void)PathQuoteSpacesW(szBinaryPathName);
	if (StaticLib1::RAII::WinApi::SC_HANDLE hService{/*_Must_inspect_result_ WINADVAPI SC_HANDLE WINAPI*/CreateServiceW
	( /*_In_     SC_HANDLE hSCManager        */hSCManager
	, /*_In_     LPCWSTR   lpServiceName     */_CRT_WIDE(SampleService_ServiceName)
	, /*_In_opt_ LPCWSTR   lpDisplayName     */nullptr
	, /*_In_     DWORD     dwDesiredAccess   */MAXIMUM_ALLOWED
	, /*_In_     DWORD     dwServiceType     */SERVICE_WIN32_OWN_PROCESS
	, /*_In_     DWORD     dwStartType       */SERVICE_DEMAND_START
	, /*_In_     DWORD     dwErrorControl    */SERVICE_ERROR_NORMAL
	, /*_In_opt_ LPCWSTR   lpBinaryPathName  */szBinaryPathName
	, /*_In_opt_ LPCWSTR   lpLoadOrderGroup  */nullptr
	, /*_Out_opt_LPDWORD   lpdwTagId         */nullptr
	, /*_In_opt_ LPCWSTR   lpDependencies    */nullptr
	, /*_In_opt_ LPCWSTR   lpServiceStartName*/nullptr
	, /*_In_opt_ LPCWSTR   lpPassword        */nullptr
	)})
		return hService;
	else {
		switch (const auto e = GetLastError()) {
		case ERROR_SERVICE_EXISTS:
			return change_service_config(open_service(hSCManager), szBinaryPathName);
		default:
			throw std::runtime_error{ Error::Source{_CRT_STRINGIZE(CreateServiceW)}.winapi_error_message(e) };
		}
	}
}
unsigned CALLBACK set_start_event(void* stop_event)
{
	Error_WinApi_Check(SetEvent(static_cast<HANDLE>(stop_event)));
	return EXIT_SUCCESS;
}
#include "../SampleService/DebugService.h"
#include "../StaticLib1/RAII/WinApi/CloseHandle.h"
#include <process.h>
int main(int argc, char* argv[])
{
	try {
		const StaticLib1::RAII::WinApi::SC_HANDLE hSCManager{ Error_WinApi_Check(/*_Must_inspect_result_ WINADVAPI SC_HANDLE WINAPI*/OpenSCManagerW
		( /*_In_opt_ LPCWSTR lpMachineName  */nullptr
		, /*_In_opt_ LPCWSTR lpDatabaseName */nullptr
		, /*_In_     DWORD   dwDesiredAccess*/MAXIMUM_ALLOWED
		)) };
		switch (argc) {
		case 2:
			if (lstrcmpiA(argv[1], "-uninstall") == 0) {
				Error_WinApi_Check(DeleteService(/*_In_ SC_HANDLE  hService*/open_service(hSCManager.get()).get()));
				return EXIT_SUCCESS;
			}
			else if (lstrcmpiA(argv[1], "-start") == 0) {
				const auto hService = install_service(hSCManager.get());
				const StaticLib1::RAII::WinApi::HANDLE<nullptr> start_event{ Error_WinApi_Check(CreateEventW(nullptr, true, false, _CRT_WIDE(SampleService_DebugService))) };
				//３０秒以内に開始イベントをセットする
				StaticLib1::RAII::WinApi::HANDLE<nullptr> hThread{ Error_WinApi_Check(HANDLE(/*_Success_(return != 0) _ACRTIMP uintptr_t __cdecl*/_beginthreadex
				( /*_In_opt_  void                   * _Security    */nullptr
				, /*_In_      unsigned                 _StackSize   */0U
				, /*_In_      _beginthreadex_proc_type _StartAddress*/set_start_event
				, /*_In_opt_  void                   * _ArgList     */start_event.get()
				, /*_In_      unsigned                 _InitFlag    */0U
				, /*_Out_opt_ unsigned               * _ThrdAddr    */nullptr
				)))};
				Error_WinApi_Check(StartServiceW(hService.get(), 0UL, nullptr));
				(void)WaitForSingleObject(hThread.get(), INFINITE);
				return EXIT_SUCCESS;
			}
			else
				throw std::runtime_error{ Error::Source{__FUNCTION__}.error_message("failed because of unexpected argv[1]") };
		case 1:
			(void)install_service(hSCManager.get());
			return EXIT_SUCCESS;
		default:
			throw std::runtime_error{ Error::Source{ __FUNCTION__ }.error_message("failed because of unexpected argc") };
		}
	}
	catch (const std::exception& e) {
		OutputDebugStringA(e.what());
		OutputDebugStringA("\n");
		return EXIT_FAILURE;
	}
}
