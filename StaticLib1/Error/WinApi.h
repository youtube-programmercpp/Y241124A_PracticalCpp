#pragma once
#include "Source.h"
#include <exception>
namespace Error {
	class WinApi : public std::exception
	{
		Source                source     ;
		unsigned long         dwLastError;
		mutable std::string   message    ;
	public:
		WinApi(Source&& source, unsigned long dwLastError) noexcept
			: source         { std::move(source       ) }
			, dwLastError    {           dwLastError    }
			, message        {                          }
		{
		}
		const char* what() const override
		{
			if (message.empty())
				message = source.winapi_error_message(dwLastError);
			return message.c_str();
		}
		unsigned long code() const noexcept
		{
			return dwLastError;
		}
	};
}

#define	Error_WinApi_CheckEx(xpr, last_error)	\
[](auto retval)\
{\
	return retval ? retval : throw Error::WinApi{Error::Source{#xpr}, last_error};\
}(xpr)
#define	Error_WinApi_Check(xpr)	Error_WinApi_CheckEx(xpr, GetLastError())
#define	Error_WinApi_CheckNotEx(xpr, unexpected_value, last_error)	\
[](auto retval)\
{\
	return retval == unexpected_value ? throw Error::WinApi{Error::Source{#xpr}, last_error} : retval;\
}(xpr)
#define	Error_WinApi_CheckNot(xpr, unexpected_value)	Error_WinApi_CheckNotEx(xpr, unexpected_value, GetLastError())
