#pragma once

namespace image_lib
{
	template<typename Image>
	inline size_t image_size_in_byte(const Image& image)
	{
		return Image::image_format::size_policy::image_size_in_byte( image.width(), image.height() );
	}

	template<typename Image>
	inline size_t array_size_of_image(const Image& image)
	{
		return Image::image_format::size_policy::array_size( image.width(), image.height() );
	}

	template<typename Image>
	inline size_t image_size(const Image& image)
	{
		return Image::image_format::size_policy::image_size_in_logical_coordinate( image.width(), image.height() );
	}

}