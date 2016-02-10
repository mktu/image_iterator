#pragma once
#include "GrayPixelReference.h"
#include "BasicPixelIterator.h"
#include "Incremeter2D.h"
#include "Locator.h"
#include "ImageSize.h"

namespace image_lib
{
	template<typename T>
	struct gray_format_t
	{
		enum{ channels = 1 };
		typedef T* pointer;
		typedef gray_locator locator;
		typedef gray_pixel_iterator_reference<pointer, locator> reference;
		typedef gray_pixel_iterator_const_reference<pointer, locator> const_reference;
		typedef gray_pixel_iterator_value<pointer, locator> value_type;
		typedef typename basic_pixel_iterator<row_incremeter_2d, reference, value_type> row_iterator;
		typedef typename basic_pixel_iterator<col_incremeter_2d, reference, value_type> col_iterator;
		typedef typename basic_pixel_iterator<row_incremeter_2d, const_reference, value_type> const_row_iterator;
		typedef typename basic_pixel_iterator<col_incremeter_2d, const_reference, value_type> const_col_iterator;
		typedef row_iterator iterator;
		typedef const_row_iterator const_iterator;
		typedef image_size_policy<T,channels> size_policy;
	};
}