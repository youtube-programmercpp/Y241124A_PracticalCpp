#include "Server.h"
#include "NameOfStopEvent.h"
#include "NameOfPipe.h"
#include "../StaticLib1/RAII/WinApi/CloseHandle.h"
#include "../StaticLib1/RAII/Crt/_close.h"
#include "../StaticLib1/RAII/Crt/fclose.h"
#include "../StaticLib1/Error/WinApi.h"
#include "../StaticLib1/Error/Crt.h"
#include "../StaticLib1/Error/verify.h"
#include <stdexcept>
#include <string>
#include <fstream>
#include <process.h>
#include <io.h>
#include <fcntl.h>

Server::Server(StaticLib1::RAII::WinApi::HANDLE<nullptr>&& parameter_extract_event) noexcept
	: parameter_extract_event { std::move(parameter_extract_event) }
	, named_pipe_handle       {                                    }
{
}
unsigned Server::ThreadProc() noexcept
{
	try {
		StaticLib1::RAII::unique_file_descriptor fd{ Error_verify_not(/*_ACRTIMP int __cdecl*/_open_osfhandle
		( /*_In_ intptr_t _OSFileHandle*/intptr_t(named_pipe_handle.get())
		, /*_In_ int      _Flags       */_O_TEXT
		), -1) };
		(void)named_pipe_handle.release();
		Error_WinApi_Check(SetEvent(parameter_extract_event.get()));

		// [Microsoft]_open_osfhandle : HANDLE → int (file descriptor)
		// fdopen で int (file descriptor) → FILE* (file pointer)
		// [Microsoft]std::ifstream の変換コンストラクタ FILE* → std::ifstream

		StaticLib1::RAII::unique_file_pointer fp{ Error_Crt_Check(_fdopen(fd.get(), "r")) };
		(void)fd.release();
		//サーバー側でもパイプをファイルとして扱っている
		for (std::ifstream file{ fp.get()};;) {
			std::string s;
			if (std::getline(file, s)) {
				OutputDebugStringA(s.c_str());
				OutputDebugStringA("\n");
			}
			else
				return EXIT_SUCCESS;
		}
	}
	catch (const std::exception& e) {
		OutputDebugStringA(e.what());
		OutputDebugStringA("\n");
		return EXIT_FAILURE;
	}
}
void Server::on_connect()
{
	StaticLib1::RAII::WinApi::HANDLE<nullptr> hThread{ Error_WinApi_CheckEx(HANDLE(/*_Success_(return != 0) _ACRTIMP uintptr_t __cdecl*/_beginthreadex
	( /*_In_opt_  void                   * _Security    */nullptr
	, /*_In_      unsigned                 _StackSize   */0U
	, /*_In_      _beginthreadex_proc_type _StartAddress*/[](void* _ArgList) noexcept
		{
			return static_cast<Server*>(_ArgList)->ThreadProc();
		}
	, /*_In_opt_  void                   * _ArgList     */this
	, /*_In_      unsigned                 _InitFlag    */0U
	, /*_Out_opt_ unsigned               * _ThrdAddr    */nullptr
	)), _doserrno) };
	switch (const auto dwResult = WaitForSingleObject(parameter_extract_event.get(), INFINITE)) {
	case WAIT_OBJECT_0:
		return;
	case WAIT_FAILED:
		throw std::runtime_error{ Error::Source{ _CRT_STRINGIZE(WaitForSingleObject) }.winapi_error_message(GetLastError()) };
	default:
		throw std::runtime_error{ Error::Source{ _CRT_STRINGIZE(WaitForSingleObject) }.unexpected_value_message(dwResult) };
	}
}



void Server::run()
{
	const StaticLib1::RAII::WinApi::HANDLE<nullptr> connect_event{Error_WinApi_Check(/*WINBASEAPI _Ret_maybenull_ HANDLE WINAPI*/::CreateEventW
	( /*_In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes*/nullptr
	, /*_In_     BOOL                  bManualReset     */false
	, /*_In_     BOOL                  bInitialState    */false
	, /*_In_opt_ LPCWSTR               lpName           */nullptr
	))};
	const StaticLib1::RAII::WinApi::HANDLE<nullptr> stop_event{Error_WinApi_Check(/*WINBASEAPI _Ret_maybenull_ HANDLE WINAPI*/::CreateEventW
	( /*_In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes*/nullptr
	, /*_In_     BOOL                  bManualReset     */false
	, /*_In_     BOOL                  bInitialState    */false
	, /*_In_opt_ LPCWSTR               lpName           */_CRT_WIDE(SampleServer_NameOfStopEvent)
	))};
	CONST HANDLE rgHandles[] =
	{ stop_event   .get()
	, connect_event.get()
	};

	SECURITY_DESCRIPTOR sd;
	Error_WinApi_Check(/*WINADVAPI BOOL WINAPI*/InitializeSecurityDescriptor
	( /*_Out_ PSECURITY_DESCRIPTOR pSecurityDescriptor*/&sd
	, /*_In_  DWORD                dwRevision         */SECURITY_DESCRIPTOR_REVISION
	));
	Error_WinApi_Check(/*WINADVAPI BOOL WINAPI*/SetSecurityDescriptorDacl
	( /*_Inout_  PSECURITY_DESCRIPTOR pSecurityDescriptor*/&sd
	, /*_In_     BOOL                 bDaclPresent       */true
	, /*_In_opt_ PACL                 pDacl              */nullptr
	, /*_In_     BOOL                 bDaclDefaulted     */false
	));

	SECURITY_ATTRIBUTES sa
	{ /*DWORD  nLength             ;*/sizeof sa
    , /*LPVOID lpSecurityDescriptor;*/&sd
    , /*BOOL   bInheritHandle      ;*/false
	};

	for (;;) {
		named_pipe_handle = { Error_WinApi_CheckNot(/*WINBASEAPI HANDLE WINAPI*/CreateNamedPipeW
		( /*_In_     LPCWSTR               lpName              */_CRT_WIDE(SampleServer_NameOfPipe)
		, /*_In_     DWORD                 dwOpenMode          */PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED
		, /*_In_     DWORD                 dwPipeMode          */PIPE_TYPE_BYTE
		, /*_In_     DWORD                 nMaxInstances       */PIPE_UNLIMITED_INSTANCES
		, /*_In_     DWORD                 nOutBufferSize      */0UL
		, /*_In_     DWORD                 nInBufferSize       */0UL
		, /*_In_     DWORD                 nDefaultTimeOut     */0UL
		, /*_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes*/&sa
		), INVALID_HANDLE_VALUE)};
		OVERLAPPED ov
		{ /*ULONG_PTR Internal    ;*/{}
		, /*ULONG_PTR InternalHigh;*/{}
		, /*union     {          };*/{}
		, /*HANDLE    hEvent      ;*/connect_event.get()
		};
		if (/*WINBASEAPI BOOL WINAPI*/ConnectNamedPipe
		( /*_In_        HANDLE       hNamedPipe  */named_pipe_handle.get()
		, /*_Inout_opt_ LPOVERLAPPED lpOverlapped*/&ov
		)) {
			on_connect();
			continue;
		}
		else {
			switch (const auto e = GetLastError()) {
			case ERROR_PIPE_CONNECTED:
				on_connect();
				continue;
			default:
				throw std::runtime_error{ Error::Source{_CRT_STRINGIZE(ConnectNamedPipe)}.winapi_error_message(e) };
			case ERROR_IO_PENDING    :
				switch (const auto dwResult = /*WINBASEAPI DWORD WINAPI*/WaitForMultipleObjects
				( /*_In_               DWORD          nCount        */_countof(rgHandles)
				, /*_In_reads_(nCount) CONST HANDLE * lpHandles     */rgHandles
				, /*_In_               BOOL           bWaitAll      */false
				, /*_In_               DWORD          dwMilliseconds*/INFINITE
				)) {
				case WAIT_OBJECT_0 + 0:
					return;
				case WAIT_OBJECT_0 + 1:
					on_connect();
					continue;
				case WAIT_FAILED      :
					throw std::runtime_error{ Error::Source{_CRT_STRINGIZE(WaitForMultipleObjects)}.winapi_error_message(GetLastError()) };
				default               :
					throw std::runtime_error{ Error::Source{_CRT_STRINGIZE(WaitForMultipleObjects)}.unexpected_value_message(dwResult) };
				}
			}
		}
	}
}
