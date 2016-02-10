#pragma once
#include <iterator>
#include "Incremeter2d.h"
#include "SourceHolderBase.h"

namespace image_lib
{
	typedef ptrdiff_t coord_t;

	template<size_t Channels>
	struct basic_packed_pixel_locator
	{
		typedef counterable_2d counter_type;

		basic_packed_pixel_locator(const logical_coordinate_holder& source) 
			: width_( source.underlying_coordinates_.width_) 
			, offset_x_( source.superficial_coordinates_.offset_x_ )
			, offset_y_( source.superficial_coordinates_.offset_y_ ) { }

		coord_t operator()(const counter_type& counterable, coord_t offset) const 
		{
			return ((counterable.y_ +offset_y_) * width_ + (counterable.x_+offset_x_) ) * Channels + offset;
		}

	private:
		coord_t width_;
		coord_t offset_x_, offset_y_;
	};

	template<size_t Channels>
	struct basic_planar_pixel_locator
	{
		typedef counterable_2d counter_type;

		basic_planar_pixel_locator(const logical_coordinate_holder& source) 
			: channel_offset_( source.underlying_coordinates_.width_ * source.underlying_coordinates_.height_ )
			, width_( source.underlying_coordinates_.width_) 
			, offset_x_( source.superficial_coordinates_.offset_x_ )
			, offset_y_( source.superficial_coordinates_.offset_y_ ) { }

		coord_t operator()(const counter_type& counterable, coord_t offset) const 
		{
			return channel_offset_ * offset + width_ * (counterable.y_+offset_y_) + (counterable.x_+offset_x_);
		}

	private:
		coord_t channel_offset_;
		coord_t width_;
		coord_t offset_x_, offset_y_;
	};

	struct gray_locator
	{
		typedef counterable_2d counter_type;

		gray_locator(const logical_coordinate_holder& source) 
			: width_( source.underlying_coordinates_.width_) 
			, offset_x_( source.superficial_coordinates_.offset_x_ )
			, offset_y_( source.superficial_coordinates_.offset_y_ ) { }

		coord_t operator()(const counter_type& counterable) const 
		{
			return width_ * (counterable.y_+offset_y_) + (counterable.x_+offset_x_);
		}

	private:
		coord_t width_;
		coord_t offset_x_, offset_y_;
	};
	
}