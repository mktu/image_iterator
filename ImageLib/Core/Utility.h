#pragma once

namespace image_lib
{
	template<typename T>
	struct remove_pointer
	{
		typedef T* type;
	};

	template<typename T>
	struct remove_pointer<T*>
	{
		typedef T type;
	};
}