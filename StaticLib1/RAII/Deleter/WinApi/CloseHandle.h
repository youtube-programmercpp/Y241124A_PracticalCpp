#pragma once
#include "../../unique_obj.h"
#include "../../../Error/WinApi.h"
#include <Windows.h>
namespace StaticLib1 {
	namespace RAII {
		namespace Deleter {
			namespace WinApi {
				struct CloseHandle {
					void operator()(_In_ _Post_ptr_invalid_::HANDLE hObject) const noexcept
					{
						if (::CloseHandle(/*_In_ _Post_ptr_invalid_ HANDLE hObject*/hObject))
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
