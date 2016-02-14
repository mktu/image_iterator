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


#define MAKE_CO_OWNERSHIP_IAMGE_PROVIDER(format,name) \
	inline detail::co_ownership_image_provider<format>::image_type \
		create_## name ##_image(coord_t w, coord_t h) \
	{ \
		return create_co_ownership_image(w,h,format()); \
	}

namespace image_lib
{

MAKE_NON_OWNERSHIP_IAMGE_PROVIDER(gray_format_t,1,gray)
MAKE_NON_OWNERSHIP_IAMGE_PROVIDER(bit_pixel_format,1,bit)
MAKE_NON_OWNERSHIP_IAMGE_PROVIDER(packed_format_t,3,packed)
MAKE_NON_OWNERSHIP_IAMGE_PROVIDER(planar_format_t,3,planar)
MAKE_NON_OWNERSHIP_IAMGE_PROVIDER(packed_format_t,4,packed_32)
MAKE_NON_OWNERSHIP_IAMGE_PROVIDER(planar_format_t,4,planar_32)

MAKE_CO_OWNERSHIP_IAMGE_PROVIDER(gray8u_format,gray)
MAKE_CO_OWNERSHIP_IAMGE_PROVIDER(bit8u_format,bit)
MAKE_CO_OWNERSHIP_IAMGE_PROVIDER(color24u_format,packed)
MAKE_CO_OWNERSHIP_IAMGE_PROVIDER(color24u_planar_format,planar)

}

#undef MAKE_NON_OWNERSHIP_IAMGE_PROVIDER
#undef MAKE_CO_OWNERSHIP_IAMGE_PROVIDER