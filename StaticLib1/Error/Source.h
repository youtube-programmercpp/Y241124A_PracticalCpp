#pragma once
#include <sstream>
#include <source_location>
namespace Error {
	class Source
	{
		const char           * xpr;
		std::source_location    sl;
		template<class T>std::string msg(const T& fn) const
		{
			std::ostringstream ostm;
			ostm
				<< sl.file_name()
				<< '('
				<< sl.line()
				<< ','
				<< sl.column()
				<< "): "
				<< xpr
				;
			fn(ostm);
			ostm
				<< " at "
				<< sl.function_name()
				;
			return ostm.str();
		}
	public:
		Source
		( _In_ const char* xpr
		, _In_ const std::source_location& sl = std::source_location::current()
		) noexcept
			: xpr{xpr}
			, sl {sl }
		{
		}
		std::string error_message(_In_opt_ const char* pszText = nullptr) const
		{
			return msg([pszText](std::ostream& ostm)
				{
					ostm << ' ' << (pszText ? pszText : "failed");
				});
		}
		std::string crt_error_message(_In_ errno_t e) const
		{
			return msg([e](std::ostream& ostm)
				{
					ostm
						<< " failed with "
						<< e
						;
				});
		}
		std::string winapi_error_message(_In_ unsigned long dwLastError) const
		{
			return msg([dwLastError](std::ostream& ostm)
				{
					ostm
						<< " failed with "
						<< dwLastError
						;
				});
		}
		template<class T>std::string unexpected_value_message(_In_ T value) const
		{
			return msg([value](std::ostream& ostm)
				{
					ostm
						<< " returned unexpected value "
						<< value
						;
				});
		}
	};
}
