#pragma once
#include "BitPixelReference.h"
#include "BitLocator.h"
#include "BasicPixelIterator.h"
#include "Incremeter2D.h"
#include "ImageSize.h"

namespace image_lib
{
	template<typename T>
	struct bit_pixel_format
	{
		typedef bool bit_type;
		typedef T* pointer;
		typedef bit_pixel_locator<T> locator;
		typedef bit_pixel_iterator_reference<pointer,locator> reference;
		typedef bit_pixel_iterator_const_reference<pointer,locator> const_reference;
		typedef bit_pixel_iterator_value<pointer, locator> value_type;
		typedef typename basic_pixel_iterator<row_incremeter_2d, reference, value_type> row_iterator;
		typedef typename basic_pixel_iterator<col_incremeter_2d, reference, value_type> col_iterator;
		typedef typename basic_pixel_iterator<row_incremeter_2d, const_reference, value_type> const_row_iterator;
		typedef typename basic_pixel_iterator<col_incremeter_2d, const_reference, value_type> const_col_iterator;
		typedef row_iterator iterator;
		typedef const_row_iterator const_iterator;
		typedef bit_image_size_policy<T> size_policy;
	}; 
}