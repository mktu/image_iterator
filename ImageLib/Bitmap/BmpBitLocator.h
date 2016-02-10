#pragma once

#include"BmpLocator.h"

namespace image_lib
{
	struct bmp1_pixel_locator
	{
		typedef unsigned char type;
		typedef counterable_2d counter_type;

		enum
		{
			bit_size = sizeof(type) * 8,
			bit_offset = bit_size - 1,
			bit_mask = bit_offset,
			log_bit_size = 3,
		};

		static coord_t get_pos_in_bit_coordinate(coord_t d){ return bit_offset - d; }
		
		bool get_bit(const type* p, const counter_type& counterable) const
		{
			const coord_t x_offset = (counterable.x_+offset_x_);
			const coord_t pos_in_byte_coordinate = // (start_offset) - (offset for y in byte) + (offset for x in byte)
				start_offset_ - (counterable.y_+offset_y_) * padding_width_in_byte_+
				(x_offset >> log_bit_size); // offset for x in byte = offset in logical coordinate / 8
			const coord_t pos_in_bit_coordinate = get_pos_in_bit_coordinate(x_offset&bit_mask);//[0,7]
			return !!(*(p+pos_in_byte_coordinate) & (0x01 << pos_in_bit_coordinate));
		}

		void set_bit(type* p, const counter_type& counterable, bool value) const
		{
			const coord_t x_offset = (counterable.x_+offset_x_);
			const coord_t pos_in_byte_coordinate = // (start_offset) - (offset for y in byte) + (offset for x in byte)
				start_offset_ - (counterable.y_+offset_y_) * padding_width_in_byte_+
				(x_offset >> log_bit_size); // offset for x in byte = offset in logical coordinate / 8
			const coord_t pos_in_bit_coordinate = get_pos_in_bit_coordinate(x_offset&bit_mask);//[0,7]
			if(value) *(p+ pos_in_byte_coordinate) |= (0x01 << pos_in_bit_coordinate);
			else *(p+ pos_in_byte_coordinate) &= ~(0x01 << pos_in_bit_coordinate);
		}

		template<typename SourceType>
		bmp1_pixel_locator(const SourceType& source)
			: padding_width_in_byte_( source.width_in_byte_ )
			, start_offset_( (source.underlying_coordinates_.height_ - 1) * source.width_in_byte_ )
			, offset_x_( source.superficial_coordinates_.offset_x_ )
			, offset_y_( source.superficial_coordinates_.offset_y_ ) { }

	private:
		coord_t padding_width_in_byte_;
		coord_t start_offset_;
		coord_t offset_x_, offset_y_;
	};
}