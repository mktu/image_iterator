#pragma once

#include "Incremeter2d.h"

namespace image_lib
{
	typedef ptrdiff_t coord_t;

	namespace detail
	{
		template<int n>
		struct log_2 
		{
			enum 
			{
				value = log_2<n / 2>::value + 1
			};
		};
		
		template<>
		struct log_2<0>
		{
			enum 
			{
				value = -1
			};
		};
	}

	template<typename T>
	struct bit_pixel_locator
	{
		typedef T type;
		typedef counterable_2d counter_type;

		enum
		{
			bit_size = sizeof(type) * 8,
			bit_offset = bit_size - 1,
			bit_mask = bit_offset,
			log_bit_size = detail::log_2<bit_size>::value,
		};

		static coord_t get_pos_in_bit_coordinate(coord_t d){ return bit_offset - d; }
		
		bool get_bit(const type* p, const counter_type& counterable) const
		{
			const coord_t pos_in_logical_coordinate = (counterable.y_+offset_y_) * width_ + (counterable.x_+offset_x_);
			const coord_t pos_in_byte_coordinate = pos_in_logical_coordinate >> log_bit_size;
			const coord_t pos_in_bit_coordinate = get_pos_in_bit_coordinate(pos_in_logical_coordinate&bit_mask);
			return !!(*(p+pos_in_byte_coordinate) & (0x01 << pos_in_bit_coordinate));
		}

		void set_bit(type* p, const counter_type& counterable, bool value) const
		{
			const coord_t pos_in_logical_coordinate = (counterable.y_+offset_y_) * width_ + (counterable.x_+offset_x_);
			const coord_t pos_in_byte_coordinate = pos_in_logical_coordinate >> log_bit_size;
			const coord_t pos_in_bit_coordinate = get_pos_in_bit_coordinate(pos_in_logical_coordinate&bit_mask);
			if(value) *(p+ pos_in_byte_coordinate) |= (0x01 << pos_in_bit_coordinate);
			else *(p+ pos_in_byte_coordinate) &= ~(0x01 << pos_in_bit_coordinate);
		}

		template<typename SourceType>
		bit_pixel_locator(const SourceType& source)
			: width_( source.underlying_coordinates_.width_)
			, offset_x_( source.superficial_coordinates_.offset_x_ )
			, offset_y_( source.superficial_coordinates_.offset_y_ ) { }

	private:
		coord_t width_;
		coord_t offset_x_, offset_y_;
	};
}