#pragma once
#include "../Core/BasicPixelIterator.h"
#include "../Core/Incremeter2d.h"
#include "BmpLocator.h"
#include "BmpBitLocator.h"
#include "BitmapFileCommon.h"
#include "BmpPixelReference.h"
#include "BmpBitPixelReference.h"
#include "BitmapImageSize.h"

namespace image_lib
{
	template<typename T>
	struct bmp_rgb24_format_t
	{
		typedef T* pointer;
		enum{ channels = 3 };
		static const bitmap_type::bit_count bit_count = bitmap_type::BITMAP_24;
		typedef bmp24_pixel_locator locator;
		typedef bmp_pixel_iterator_reference<channels,pointer,locator> reference;
		typedef bmp_pixel_iterator_const_reference<channels,pointer,locator> const_reference;
		typedef bmp_pixel_iterator_value<channels, pointer, locator> value_type;
		typedef basic_pixel_iterator<row_incremeter_2d, reference, value_type> row_iterator;
		typedef basic_pixel_iterator<col_incremeter_2d, reference, value_type> col_iterator;
		typedef basic_pixel_iterator<row_incremeter_2d, const_reference, value_type> const_row_iterator;
		typedef basic_pixel_iterator<col_incremeter_2d, const_reference, value_type> const_col_iterator;
		typedef row_iterator iterator;
		typedef const_row_iterator const_iterator;
		typedef bitmap24_image_size_policy<T,channels> size_policy;
	};

	template<typename T>
	struct bmp_rgb24_no_padded_format_t
	{
		typedef T* pointer;
		enum{ channels = 3 };
		static const bitmap_type::bit_count bit_count = bitmap_type::BITMAP_24;
		typedef no_padded_bmp24_pixel_locator locator;
		typedef bmp_pixel_iterator_reference<channels,pointer,locator> reference;
		typedef bmp_pixel_iterator_const_reference<channels,pointer,locator> const_reference;
		typedef bmp_pixel_iterator_value<channels, pointer, locator> value_type;
		typedef basic_pixel_iterator<row_incremeter_2d, reference, value_type> row_iterator;
		typedef basic_pixel_iterator<col_incremeter_2d, reference, value_type> col_iterator;
		typedef basic_pixel_iterator<row_incremeter_2d, const_reference, value_type> const_row_iterator;
		typedef basic_pixel_iterator<col_incremeter_2d, const_reference, value_type> const_col_iterator;
		typedef row_iterator iterator;
		typedef const_row_iterator const_iterator;
		typedef bitmap24_image_size_policy<T,channels> size_policy;
	};

	template<typename T>
	struct bmp_gray_format_t
	{
		enum{ channels = 1 };
		static const bitmap_type::bit_count bit_count = bitmap_type::BITMAP_8;
		typedef T* pointer;
		typedef bmp256_pixel_locator locator;
		typedef bmp_pixel_iterator_reference<channels,pointer,locator> reference;
		typedef bmp_pixel_iterator_const_reference<channels,pointer,locator> const_reference;
		typedef bmp_pixel_iterator_value<channels, pointer, locator> value_type;
		typedef basic_pixel_iterator<row_incremeter_2d, reference, value_type> row_iterator;
		typedef basic_pixel_iterator<col_incremeter_2d, reference, value_type> col_iterator;
		typedef basic_pixel_iterator<row_incremeter_2d, const_reference, value_type> const_row_iterator;
		typedef basic_pixel_iterator<col_incremeter_2d, const_reference, value_type> const_col_iterator;
		typedef row_iterator iterator;
		typedef const_row_iterator const_iterator;
		typedef bitmap256_image_size_policy<T> size_policy;
	};

	template<typename T>
	struct bmp_rgb256_format_t
	{
		enum{ channels = 3 };
		static const bitmap_type::bit_count bit_count = bitmap_type::BITMAP_8;
		typedef T* pointer;
		typedef bmp256_pixel_locator locator;
		typedef bmp256_pixel_iterator_reference<channels,pointer,locator> reference;
		typedef bmp256_pixel_iterator_const_reference<channels,pointer,locator> const_reference;
		typedef bmp256_pixel_iterator_value<channels, pointer, locator> value_type;
		typedef basic_pixel_iterator<row_incremeter_2d, reference, value_type> row_iterator;
		typedef basic_pixel_iterator<col_incremeter_2d, reference, value_type> col_iterator;
		typedef basic_pixel_iterator<row_incremeter_2d, const_reference, value_type> const_row_iterator;
		typedef basic_pixel_iterator<col_incremeter_2d, const_reference, value_type> const_col_iterator;
		typedef row_iterator iterator;
		typedef const_row_iterator const_iterator;
		typedef bitmap256_image_size_policy<T> size_policy;
	};

	template<typename T>
	struct bmp_rgb2_format_t
	{
		enum{ channels = 3 };
		static const bitmap_type::bit_count bit_count = bitmap_type::BITMAP_1;
		typedef T* pointer;
		typedef bmp1_pixel_locator locator;
		typedef bmp1_pixel_iterator_reference<pointer,locator> reference;
		typedef bmp1_pixel_iterator_const_reference<pointer,locator> const_reference;
		typedef bmp1_pixel_iterator_value<pointer, locator> value_type;
		typedef basic_pixel_iterator<row_incremeter_2d, reference, value_type> row_iterator;
		typedef basic_pixel_iterator<col_incremeter_2d, reference, value_type> col_iterator;
		typedef basic_pixel_iterator<row_incremeter_2d, const_reference, value_type> const_row_iterator;
		typedef basic_pixel_iterator<col_incremeter_2d, const_reference, value_type> const_col_iterator;
		typedef row_iterator iterator;
		typedef const_row_iterator const_iterator;
		typedef bitmap1_image_size_policy<T> size_policy;
	};

	template<typename T>
	struct bmp_mono_format_t
	{
		static const bitmap_type::bit_count bit_count = bitmap_type::BITMAP_1;
		typedef bool bit_type;
		typedef T* pointer;
		typedef bmp1_pixel_locator locator;
		typedef bmp_mono_pixel_iterator_reference<pointer,locator> reference;
		typedef bmp_mono_pixel_iterator_const_reference<pointer,locator> const_reference;
		typedef bmp_mono_pixel_iterator_value<pointer, locator> value_type;
		typedef basic_pixel_iterator<row_incremeter_2d, reference, value_type> row_iterator;
		typedef basic_pixel_iterator<col_incremeter_2d, reference, value_type> col_iterator;
		typedef basic_pixel_iterator<row_incremeter_2d, const_reference, value_type> const_row_iterator;
		typedef basic_pixel_iterator<col_incremeter_2d, const_reference, value_type> const_col_iterator;
		typedef row_iterator iterator;
		typedef const_row_iterator const_iterator;
		typedef bitmap1_image_size_policy<T> size_policy;
	};
}