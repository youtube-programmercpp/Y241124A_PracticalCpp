#include "Server.h"
#include "../StaticLib1/Error/WinApi.h"

int main()
{
	try {
		Server{Error_WinApi_Check(/*WINBASEAPI _Ret_maybenull_ HANDLE WINAPI*/::CreateEventW
		( /*_In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes*/nullptr
		, /*_In_     BOOL                  bManualReset     */false
		, /*_In_     BOOL                  bInitialState    */false
		, /*_In_opt_ LPCWSTR               lpName           */nullptr
		))}.run();
		return EXIT_SUCCESS;
	}
	catch (const std::exception& e) {
		OutputDebugStringA(e.what());
		OutputDebugStringA("\n");
		return EXIT_FAILURE;
	}
}
