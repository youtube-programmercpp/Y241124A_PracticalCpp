#pragma once
#include "../../unique_obj.h"
#include "../../../Error/Crt.h"
#include <io.h>
#include <Windows.h>
namespace StaticLib1 {
	namespace RAII {
		namespace Deleter {
			namespace Crt {
				struct _close {
					void operator()(_In_ int _FileHandle) const noexcept
					{
						if (/*_Check_return_opt_ _ACRTIMP int __cdecl*/::_close
						( /*_In_ int _FileHandle*/_FileHandle
						) == -1)
							OutputDebugStringA((Error::Source{ _CRT_STRINGIZE(_close) }.crt_error_message(errno) + '\n').c_str());
						else
							return;
					}
				};
			}
		}
	}
}
