#pragma once
#include "../unique_obj.h"
#include "../Deleter/Crt/_close.h"
namespace StaticLib1 {
	namespace RAII {
		using unique_file_descriptor = unique_obj<int, -1, Deleter::Crt::_close>;
	}
}
