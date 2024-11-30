#pragma once
#include "../unique_obj.h"
#include "../Deleter/Crt/fclose.h"
namespace StaticLib1 {
	namespace RAII {
		using unique_file_pointer = unique_obj<FILE*, nullptr, Deleter::Crt::fclose>;
	}
}
