#pragma once
#include "../unique_obj.h"
#include "../Deleter/WinApi/CloseHandle.h"
namespace StaticLib1 {
	namespace RAII {
		namespace WinApi {
			template<::HANDLE invalid>using HANDLE = unique_obj<::HANDLE, invalid, Deleter::WinApi::CloseHandle>;
		}
	}
}
