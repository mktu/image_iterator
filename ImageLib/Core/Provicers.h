#pragma once

#include <algorithm>
#include "PointerHolder.h"
#include "GenericImage.h"


namespace image_lib
{
	typedef ptrdiff_t coord_t;

	namespace detail
	{
		template<typename ImageFormat>
		struct non_ownership_image_provider
		{
			typedef image_t<ImageFormat, tag_non_ownership> image_type;
			typedef typename image_type::source_type source_type;
			typedef typename ImageFormat::pointer pointer;
			
			static image_type create(pointer p, coord_t w, coord_t h)
			{
				return image_type( source_type(p,w,h) );
			}
		};

		template<typename ImageFormat>
		struct co_ownership_image_provider
		{
			typedef image_t<ImageFormat, tag_co_ownership> image_type;
			typedef typename image_type::source_type source_type;
			typedef typename source_type::array_type array_type;
			typedef typename source_type::shared_array_type shared_array_type;
			
			static image_type create(coord_t w, coord_t h)
			{
				shared_array_type s(new array_type(ImageFormat::size_policy::array_size(w,h)));
				return image_type( source_type(s,w,h) );
			}
		};
	}

	template<typename ImageFormat>
	inline typename detail::non_ownership_image_provider<ImageFormat>::image_type
		create_non_ownership_image(typename ImageFormat::pointer p, coord_t w, coord_t h, ImageFormat)
	{
		return detail::non_ownership_image_provider<ImageFormat>::create(p,w,h);
	}

	template<typename ImageFormat>
	inline typename detail::co_ownership_image_provider<ImageFormat>::image_type
		create_co_ownership_image(coord_t w, coord_t h, ImageFormat)
	{
		return detail::co_ownership_image_provider<ImageFormat>::create( w, h );
	}
}