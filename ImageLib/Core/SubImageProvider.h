#pragma once
#include <cstddef>

namespace image_lib
{
	template<typename ImageType>
	inline ImageType
	sub_image(const ImageType& image, size_t w, size_t h, size_t start_x, size_t start_y)
	{
		typename ImageType::source_type source = image.source();
		source.superficial_coordinates_.width_ = w;
		source.superficial_coordinates_.height_ = h;
		source.superficial_coordinates_.offset_x_ += start_x;
		source.superficial_coordinates_.offset_y_ += start_y;
		return ImageType( source );
	}
}