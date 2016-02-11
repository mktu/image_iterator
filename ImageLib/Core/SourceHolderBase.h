#pragma once

#include <iterator>

namespace image_lib
{
	typedef ptrdiff_t coord_t;

	struct superficial_coordinates
	{
		superficial_coordinates(coord_t offset_x, coord_t offset_y, coord_t width, coord_t height)
			: offset_x_( offset_x ), offset_y_( offset_y ), width_( width ), height_( height )
		{  }

		superficial_coordinates(coord_t width, coord_t height)
			: offset_x_( 0 ), offset_y_( 0 ), width_( width ), height_( height )
		{  }

		coord_t offset_x_, offset_y_;
		coord_t width_, height_;
	};

	struct underlying_coordinates
	{
		underlying_coordinates(coord_t width, coord_t height)
			: width_( width ), height_( height )
		{  }

		coord_t width_, height_;
	};

	struct logical_coordinate_holder
	{
		logical_coordinate_holder(coord_t w, coord_t h)
			: superficial_coordinates_(w, h), underlying_coordinates_(w, h) { }

		superficial_coordinates superficial_coordinates_;
		underlying_coordinates underlying_coordinates_;
	};

	inline bool is_subimage(const logical_coordinate_holder& coordinate)
	{
		return 
			coordinate.underlying_coordinates_.height_ != coordinate.superficial_coordinates_.height_ ||
			coordinate.underlying_coordinates_.width_  != coordinate.superficial_coordinates_.width_;
	}

	template<typename Pointer>
	struct source_holder_base : public logical_coordinate_holder
	{
		typedef Pointer pointer;
		typedef typename std::iterator_traits<pointer> iterator_traits;
		typedef typename iterator_traits::value_type value_type ;
		typedef logical_coordinate_holder base_type;

		source_holder_base(pointer p, coord_t w, coord_t h) : 
			 base_type(w, h), p_( p ){ }

		pointer p_;
	};
	
}