#pragma once
#include <utility>
namespace StaticLib1 {
	namespace RAII {
		template<class T, T invalid, class Deleter>class unique_obj {
			T obj;
		public:
			unique_obj(T obj = invalid) noexcept
				: obj{obj}
			{
			}
			~unique_obj()
			{
				if (obj != invalid)
					Deleter{}(obj);
			}
			explicit operator bool() const noexcept
			{
				return obj != invalid;
			}
			unique_obj(unique_obj&& r) noexcept
				: obj{ r.obj }
			{
				r.obj = invalid;
			}
			unique_obj& operator=(unique_obj&& r) noexcept
			{
				std::swap(obj, r.obj);
				return *this;
			}
			T get() const noexcept
			{
				return obj;
			}
			void reset(T obj = invalid)
			{
				*this = { obj };
			}
			T release() noexcept
			{
				T retval = obj;
				obj = invalid;
				return retval;
			}
		};
	}
}
