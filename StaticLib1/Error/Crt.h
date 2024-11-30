#pragma once
#include "Source.h"
#include <exception>
namespace Error {
	class Crt : public std::exception
	{
		Source                source     ;
		int                   last_error ;
		mutable std::string   message    ;
	public:
		Crt(Source&& source, int last_error) noexcept
			: source     { std::move(source     ) }
			, last_error {           last_error   }
			, message    {                        }
		{
		}
		const char* what() const override
		{
			if (message.empty())
				message = source.crt_error_message(last_error);
			return message.c_str();
		}
		int code() const noexcept
		{
			return last_error;
		}
	};
}
#define	Error_Crt_Check(xpr)	\
[](auto retval)\
{\
	return retval ? retval : throw Error::Crt{Error::Source{#xpr}, errno};\
}(xpr)
#define	Error_Crt_CheckNot(xpr, unexpected_value)	\
[](auto retval)\
{\
	return retval == unexpected_value ? throw Error::Crt{Error::Source{#xpr}, errno} : retval;\
}(xpr)
