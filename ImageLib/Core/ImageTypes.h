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
	typedef packed_format_t<3, unsigned char> color24u_format;
	typedef packed_format_t<3, const unsigned char> color24uc_format;
	typedef planar_format_t<3, unsigned char> color24u_planar_format;
	typedef planar_format_t<3, const unsigned char> color24uc_planar_format;
	
	typedef image_t<bit8u_format, tag_non_ownership> non_ownership_bit_image;
	typedef image_t<bit8u_format, tag_co_ownership> co_ownership_bit_image;
	typedef image_t<gray8u_format, tag_non_ownership> non_ownership_gray8u_image;
	typedef image_t<gray8uc_format, tag_non_ownership> non_ownership_gray8uc_image;
	typedef image_t<gray8u_format, tag_co_ownership> co_ownership_gray8u_image;
	typedef image_t<gray8uc_format, tag_co_ownership> co_ownership_gray8uc_image;
	typedef image_t<color24u_format, tag_non_ownership> non_ownership_color24u_image;
	typedef image_t<color24uc_format, tag_non_ownership> non_ownership_color24uc_image;
	typedef image_t<color24u_format, tag_co_ownership> co_ownership_color24u_image;
	typedef image_t<color24uc_format, tag_co_ownership> co_ownership_color24uc_image;
	typedef image_t<color24u_planar_format, tag_non_ownership> non_ownership_color24u_planar_image;
	typedef image_t<color24uc_planar_format, tag_non_ownership> non_ownership_color24uc_planar_image;
	typedef image_t<color24u_planar_format, tag_co_ownership> co_ownership_color24u_planar_image;
	typedef image_t<color24uc_planar_format, tag_co_ownership> co_ownership_color24uc_planar_image;
}