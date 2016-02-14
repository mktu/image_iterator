#pragma once

#include "Provicers.h"
#include "ImageTypes.h"

#define MAKE_NON_OWNERSHIP_IAMGE_PROVIDER(format_t,channels,name) \
template<typename Pointer> \
	inline typename detail::retrieve_image_format<channels,Pointer>::template dispatcher<format_t>::image_type \
		create_## name ##_image(Pointer p, coord_t w, coord_t h) \
	{ \
		typedef typename detail::retrieve_image_format<channels,Pointer>::template dispatcher<format_t>::format_type type; \
		return create_non_ownership_image(p,w,h,type()); \
	}

namespace image_lib
{

MAKE_NON_OWNERSHIP_IAMGE_PROVIDER(gray_format_t,1,gray)
MAKE_NON_OWNERSHIP_IAMGE_PROVIDER(bit_pixel_format,1,bit)
MAKE_NON_OWNERSHIP_IAMGE_PROVIDER(packed_format_t,3,packed)
MAKE_NON_OWNERSHIP_IAMGE_PROVIDER(planar_format_t,3,planar)
MAKE_NON_OWNERSHIP_IAMGE_PROVIDER(packed_format_t,4,packed_32)
MAKE_NON_OWNERSHIP_IAMGE_PROVIDER(planar_format_t,4,planar_32)


}

#undef MAKE_NON_OWNERSHIP_IAMGE_PROVIDER