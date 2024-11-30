#pragma once
#include "../../unique_obj.h"
#include "../../../Error/WinApi.h"
#include <Windows.h>
namespace StaticLib1 {
	namespace RAII {
		namespace Deleter {
			namespace WinApi {
				struct CloseServiceHandle {
					void operator()(_In_ ::SC_HANDLE hSCObject) const noexcept
					{
						if (::CloseServiceHandle(/*_In_ SC_HANDLE hSCObject*/hSCObject))
							return;
						else {
							OutputDebugStringA(Error::WinApi{ Error::Source{__FUNCTION__}, GetLastError()}.what());
							OutputDebugStringA("\n");
						}
					}
				};
			}
		}
	}
}
