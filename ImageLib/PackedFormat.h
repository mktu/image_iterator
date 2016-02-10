#pragma once
#include "BasicPixelReference.h"
#include "BasicPixelIterator.h"
#include "Incremeter2D.h"
#include "Locator.h"
#include "ImageSize.h"

namespace image_lib
{
	template<size_t t, typename T>
	struct packed_format_t
	{
		enum{ channels = t };
		typedef T* pointer;
		typedef basic_packed_pixel_locator<channels> locator;
		typedef basic_pixel_iterator_reference<channels, pointer, locator> reference;
		typedef basic_pixel_iterator_const_reference<channels, pointer, locator> const_reference;
		typedef basic_pixel_iterator_value<channels, pointer, locator> value_type;
		typedef typename basic_pixel_iterator<row_incremeter_2d, reference, value_type> row_iterator;
		typedef typename basic_pixel_iterator<col_incremeter_2d, reference, value_type> col_iterator;
		typedef typename basic_pixel_iterator<row_incremeter_2d, const_reference, value_type> const_row_iterator;
		typedef typename basic_pixel_iterator<col_incremeter_2d, const_reference, value_type> const_col_iterator;
		typedef row_iterator iterator;
		typedef const_row_iterator const_iterator;
		typedef image_size_policy<T,channels> size_policy;
	};
}