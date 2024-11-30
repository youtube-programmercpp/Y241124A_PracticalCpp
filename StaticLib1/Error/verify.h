#pragma once
#include "Source.h"
#include <stdexcept>
#define	Error_verify(xpr)	\
[](auto retval)\
{\
	return retval ? retval : throw std::runtime_error{Error::Source{#xpr}.error_message()};\
}(xpr)
#define	Error_verify_not(xpr, unexpected_value)	\
[](auto retval)\
{\
	return retval == unexpected_value ? throw std::runtime_error{Error::Source{#xpr}.error_message()} : retval;\
}(xpr)
