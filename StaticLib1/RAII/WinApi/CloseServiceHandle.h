#pragma once
#include "../unique_obj.h"
#include "../Deleter/WinApi/CloseServiceHandle.h"
namespace StaticLib1 {
	namespace RAII {
		namespace WinApi {
			using SC_HANDLE = unique_obj<::SC_HANDLE, nullptr, Deleter::WinApi::CloseServiceHandle>;
		}
	}
}
