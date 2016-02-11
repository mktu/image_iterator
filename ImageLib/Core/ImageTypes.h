#pragma once

#include "BitFormat.h"
#include "GrayFormat.h"
#include "PackedFormat.h"
#include "PlanarFormat.h"

#include "PointerHolder.h"
#include "GenericImage.h"

#include "ImageSourceTag.h"

namespace image_lib
{
	typedef bit_pixel_format<unsigned char> bit8u_format;
	typedef gray_format_t<unsigned char> gray8u_format;
	typedef gray_format_t<const unsigned char> gray8uc_format;
	typedef packed_format_t<3, unsigned char> rgb8u_format;
	typedef packed_format_t<3, const unsigned char> rgb8uc_format;
	typedef planar_format_t<3, unsigned char> rgb8u_planar_format;
	typedef planar_format_t<3, const unsigned char> rgb8uc_planar_format;
	
	typedef image_t<bit8u_format, tag_non_ownership> non_ownership_bit_image;
	typedef image_t<bit8u_format, tag_co_ownership> co_ownership_bit_image;
	typedef image_t<gray8u_format, tag_non_ownership> non_ownership_gray8u_image;
	typedef image_t<gray8uc_format, tag_non_ownership> non_ownership_gray8uc_image;
	typedef image_t<gray8u_format, tag_co_ownership> co_ownership_gray8u_image;
	typedef image_t<gray8uc_format, tag_co_ownership> co_ownership_gray8uc_image;
	typedef image_t<rgb8u_format, tag_non_ownership> non_ownership_rgb24_image;
	typedef image_t<rgb8uc_format, tag_non_ownership> non_ownership_rgb24c_image;
	typedef image_t<rgb8u_format, tag_co_ownership> co_ownership_rgb24_image;
	typedef image_t<rgb8uc_format, tag_co_ownership> co_ownership_rgb24c_image;
	typedef image_t<rgb8u_planar_format, tag_non_ownership> non_ownership_rgb24_planar_image;
	typedef image_t<rgb8uc_planar_format, tag_non_ownership> non_ownership_rgb24c_planar_image;
	typedef image_t<rgb8u_planar_format, tag_co_ownership> co_ownership_rgb24_planar_image;
	typedef image_t<rgb8uc_planar_format, tag_co_ownership> co_ownership_rgb24c_planar_image;
}