#pragma once

#include <algorithm>
#include "Utility.h"
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

		template<
			size_t channels,
			typename Pointer
		> 
		struct retrieve_image_format
		{
			typedef typename remove_pointer<Pointer>::type value_type;
			template<
				template<size_t, class T> class FormatType
			> 
			struct dispatcher
			{
				typedef FormatType<channels,value_type> format_type;
				typedef typename non_ownership_image_provider<format_type>::image_type image_type;
			};

		};
		template<typename Pointer> 
		struct retrieve_image_format<1,Pointer>
		{
			typedef typename remove_pointer<Pointer>::type value_type;
			template<
				template<class T> class FormatType
			> 
			struct dispatcher
			{
				typedef FormatType<value_type> format_type;
				typedef typename non_ownership_image_provider<format_type>::image_type image_type;
			};
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