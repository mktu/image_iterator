#pragma once
#include <assert.h>
#include <iterator>
#include "../Incremeter2d.h"

namespace image_lib
{
	typedef ptrdiff_t coord_t;

	struct bmp24_pixel_locator
	{
	public:

		typedef counterable_2d counter_type;

		template<typename SourceType> bmp24_pixel_locator(const SourceType& source)
			: padding_width_in_byte_( source.width_in_byte_ )
			, start_offset_( (source.underlying_coordinates_.height_ - 1) * source.width_in_byte_ )
			, offset_x_( source.superficial_coordinates_.offset_x_ )
			, offset_y_( source.superficial_coordinates_.offset_y_ ) { }

		coord_t operator()(const counter_type& counterable, coord_t bgr_offset) const 
		{
			return start_offset_ 
				- (counterable.y_+offset_y_) * padding_width_in_byte_
				+ (counterable.x_+offset_x_) * 3
				+ (2 - bgr_offset);
		}

	private:
		coord_t padding_width_in_byte_;
		coord_t start_offset_;
		coord_t offset_x_, offset_y_;
	};

	typedef bmp24_pixel_locator no_padded_bmp24_pixel_locator;


	struct bmp256_pixel_locator
	{
	public:

		typedef counterable_2d counter_type;

		template<typename SourceType> bmp256_pixel_locator(const SourceType& source)
			: padding_width_in_byte_( source.width_in_byte_ )
			, start_offset_( (source.underlying_coordinates_.height_ - 1) * source.width_in_byte_ )
			, offset_x_( source.superficial_coordinates_.offset_x_ )
			, offset_y_( source.superficial_coordinates_.offset_y_ ) { }

		coord_t operator()(const counter_type& counterable) const 
		{
			return start_offset_ 
				- (counterable.y_+offset_y_) * padding_width_in_byte_
				+ (counterable.x_+offset_x_);
		}

	private:
		coord_t padding_width_in_byte_;
		coord_t start_offset_;
		coord_t offset_x_, offset_y_;
	};
}