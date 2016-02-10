#pragma once

#include "BitmapFormat.h"
#include "BmpImageSource.h"
#include "../GenericImage.h"

namespace image_lib
{
	typedef bmp_rgb24_format_t<BYTE> bmp_rgb24_format;
	typedef bmp_rgb24_format_t<const BYTE> bmp_rgb24c_format;
	typedef bmp_rgb24_no_padded_format_t<BYTE> bmp_rgb24_no_padded_format;
	typedef bmp_rgb24_no_padded_format_t<const BYTE> bmp_rgb24c_no_padded_format;
	typedef bmp_gray_format_t<BYTE> bmp_gray_format;
	typedef bmp_gray_format_t<const BYTE> bmp_grayc_format;
	typedef bmp_rgb256_format_t<BYTE> bmp_rgb256_format;
	typedef bmp_rgb256_format_t<const BYTE> bmp_rgb256c_format;
	typedef bmp_rgb2_format_t<BYTE> bmp_rgb2_format;
	typedef bmp_rgb2_format_t<const BYTE> bmp_rgb2c_format;
	typedef bmp_mono_format_t<BYTE> bmp_mono_format;
	typedef bmp_mono_format_t<const BYTE> bmp_monoc_format;

	typedef image_t<bmp_rgb24_format, tag_bmp_co_ownership> co_ownership_bmp_rgb24_image;
	typedef image_t<bmp_rgb24c_format, tag_bmp_co_ownership> co_ownership_bmp_rgb24c_image;
	typedef image_t<bmp_rgb256_format, tag_bmp_co_ownership> co_ownership_bmp_rgb256_image;
	typedef image_t<bmp_rgb256c_format, tag_bmp_co_ownership> co_ownership_bmp_rgb256c_image;
	typedef image_t<bmp_gray_format, tag_bmp_co_ownership> co_ownership_bmp_gray_image;
	typedef image_t<bmp_grayc_format, tag_bmp_co_ownership> co_ownership_bmp_grayc_image;
	typedef image_t<bmp_rgb2_format, tag_bmp_co_ownership> co_ownership_bmp_rgb2_image;
	typedef image_t<bmp_rgb2c_format, tag_bmp_co_ownership> co_ownership_bmp_rgb2c_image;
	typedef image_t<bmp_mono_format, tag_bmp_co_ownership> co_ownership_bmp_mono_image;
	typedef image_t<bmp_monoc_format, tag_bmp_co_ownership> co_ownership_bmp_monoc_image;
	
	typedef image_t<bmp_rgb24_format, tag_bmp_non_ownership> non_ownership_bmp_rgb24_image;
	typedef image_t<bmp_rgb24c_format, tag_bmp_non_ownership> non_ownership_bmp_rgb24c_image;
	typedef image_t<bmp_rgb256_format, tag_bmp_non_ownership> non_ownership_bmp_rgb256_image;
	typedef image_t<bmp_rgb256c_format, tag_bmp_non_ownership> non_ownership_bmp_rgb256c_image;
	typedef image_t<bmp_gray_format, tag_bmp_non_ownership> non_ownership_bmp_gray_image;
	typedef image_t<bmp_grayc_format, tag_bmp_non_ownership> non_ownership_bmp_grayc_image;
	typedef image_t<bmp_rgb2_format, tag_bmp_non_ownership> non_ownership_bmp_rgb2_image;
	typedef image_t<bmp_rgb2c_format, tag_bmp_non_ownership> non_ownership_bmp_rgb2c_image;
	typedef image_t<bmp_mono_format, tag_bmp_non_ownership> non_ownership_bmp_mono_image;
	typedef image_t<bmp_monoc_format, tag_bmp_non_ownership> non_ownership_bmp_monoc_image;
	
}