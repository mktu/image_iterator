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

	template<typename ImageType, typename CharType>
	inline void import_raw_image(ImageType& image, const CharType& file_name)
	{
		read_raw_image( file_name, image.address(),
				ImageType::image_format::size_policy::image_size_in_byte(image.width(),image.height()) );
	}

	template<typename ImageType, typename CharType>
	inline void export_raw_image(ImageType& image, const CharType& file_name)
	{
		if(is_subimage(image.source()))
		{
			ImageType dst = create_co_ownership_image( image.width(), image.height(), ImageType::image_format() );
			std::copy( image.begin(), image.end(), dst.begin() );
			write_raw_image( file_name, dst.address(),
				ImageType::image_format::size_policy::image_size_in_byte(dst.width(),dst.height()));
		}
		else
		{
			write_raw_image( file_name, image.address(),
				ImageType::image_format::size_policy::image_size_in_byte(image.width(),image.height()) );
		}
	}
}