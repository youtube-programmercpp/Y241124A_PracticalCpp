#pragma once
#include "../../unique_obj.h"
#include "../../../Error/Crt.h"
#include <Windows.h>
namespace StaticLib1 {
	namespace RAII {
		namespace Deleter {
			namespace Crt {
				struct fclose {
					void operator()(_Inout_ FILE* _Stream) const noexcept
					{
						if (/*_Success_(return != -1) _Check_return_opt_ _ACRTIMP int __cdecl*/::fclose
						( /*_Inout_ FILE * _Stream*/_Stream
						) == 0)
							return;
						else
							OutputDebugStringA((Error::Source{ _CRT_STRINGIZE(fclose) }.crt_error_message(errno) + '\n').c_str());
					}
				};
			}
		}
	}
}
