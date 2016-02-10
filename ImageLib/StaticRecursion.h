#pragma once
#include <iterator>

namespace image_lib
{
	namespace detail
	{
		template<size_t t>
		struct static_recursion
		{
			template<typename Pix1, typename Pix2>static inline void copy(Pix1& dst, const Pix2& src)
			{ dst[t-1]= src[t-1]; static_recursion<t-1>::copy(dst,src); }

			template<typename Pix1, typename Pix2>static inline bool equals(const Pix1& lhs, const Pix2& rhs)
			{ return lhs[t-1] == rhs[t-1] && static_recursion<t-1>::equals(lhs,rhs); }
		};
		template<>
		struct static_recursion<1>
		{
			template<typename Pix1, typename Pix2>static inline void copy(Pix1& dst, const Pix2& src)
			{ dst[0]= src[0]; }

			template<typename Pix1, typename Pix2>static inline bool equals(const Pix1& lhs, const Pix2& rhs)
			{ return lhs[0] == rhs[0]; }
		};
	}
}