#pragma once

#include "BitmapTypes.h"
#include "../GenericImage.h"

// http://tipsandtricks.runicsoft.com/Cpp/BitmapTutorial.html

namespace image_lib
{
	

	// about creation of bitmap image

	// helper function of getting pointer indicator that defined in BmpImageSource.h
	template<typename Pointer> indicator_top_of_image<Pointer> 
	top_of_image( Pointer p ) { return indicator_top_of_image<Pointer>(p); }

	template<typename Pointer> indicator_top_of_address<Pointer> 
	top_of_address( Pointer p ) { return indicator_top_of_address<Pointer>(p); }


	template<typename ImageFormat>
	inline image_t<ImageFormat, tag_bmp_co_ownership> create_co_ownership_bmp_image(
		const bmp_color_pallet_type& pallet, coord_t w, coord_t h, ImageFormat)
	{
		typedef image_t<ImageFormat, tag_bmp_co_ownership> image_type;
		typedef typename image_type::source_type source_type;
		typedef typename source_type::array_type array_type;
		typedef typename source_type::shared_array_type shared_array_type;
		shared_array_type s(new array_type(ImageFormat::size_policy::array_size(w,h)));
		return image_type( source_type(s,pallet,w,h) );
	}

	template<typename ImageFormat, typename PointerIndicator>
	inline image_t<ImageFormat, tag_bmp_non_ownership > 
		create_non_ownership_bmp_image( 
		bmp_color_pallet_type& pallet, const PointerIndicator& indicator, coord_t w, coord_t h, ImageFormat)
	{
		typedef image_t<ImageFormat, tag_bmp_non_ownership > image_type;
		typedef typename image_type::source_type source_type;
		coord_t w_in_byte = ImageFormat::size_policy::row_size_in_byte( w );
		return image_type( source_type(pallet,indicator,w,h,w_in_byte) );
	}

	template<typename ImageFormat, typename PointerIndicator>
	inline image_t<ImageFormat, tag_bmp_non_ownership > 
		create_non_ownership_bmp_image(
		const RGBQUAD* p_pallet, coord_t pallet_size,
		const PointerIndicator& indicator, coord_t w, coord_t h, ImageFormat)
	{
		bmp_color_pallet_type bmp_pallet(pallet_size);
		std::transform( p_pallet, p_pallet+pallet_size, bmp_pallet.begin(), [](RGBQUAD quad) ->quad_type
		{
			quad_type q = { quad };
			return quad_type(q);
		} );
		return create_non_ownership_bmp_image( bmp_pallet, indicator, w, h, ImageFormat() );
	}

	template<typename ImageFormat>
	inline image_t<ImageFormat, tag_bmp_co_ownership> create_co_ownership_bmp_image(coord_t w, coord_t h, ImageFormat)
	{
		return create_co_ownership_bmp_image( 
			detail::create_default_color_pallet<ImageFormat::bit_count>(), w, h, ImageFormat() );
	}

	template<typename ImageFormat, typename PointerIndicator>
	inline image_t<ImageFormat, tag_bmp_non_ownership > 
		create_non_ownership_bmp_image( 
		const PointerIndicator& indicator, coord_t w, coord_t h, ImageFormat)
	{
		return create_non_ownership_bmp_image(
			detail::create_default_color_pallet<ImageFormat::bit_count>(), indicator, w, h, ImageFormat() );
	}

	namespace detail
	{
		template<typename ImageFormat>
		struct co_ownership_image_provider;

		template<typename T>
		struct co_ownership_image_provider< bmp_rgb24_format_t<T> >
		{
			typedef image_t<bmp_rgb24_format_t<T>, tag_bmp_co_ownership> image_type;
			typedef typename image_type::source_type source_type;
			typedef typename source_type::array_type array_type;
			typedef typename source_type::shared_array_type shared_array_type;
			
			static image_type create(coord_t w, coord_t h)
			{
				return create_co_ownership_bmp_image( w, h, bmp_rgb24_format_t<T>() );
			}
		};
		template<typename T>
		struct co_ownership_image_provider< bmp_rgb256_format_t<T> >
		{
			typedef image_t<bmp_rgb256_format_t<T>, tag_bmp_co_ownership> image_type;
			typedef typename image_type::source_type source_type;
			typedef typename source_type::array_type array_type;
			typedef typename source_type::shared_array_type shared_array_type;
			
			static image_type create(coord_t w, coord_t h)
			{
				return create_co_ownership_bmp_image( w, h, bmp_rgb256_format_t<T>() );
			}
		};
		template<typename T>
		struct co_ownership_image_provider< bmp_gray_format_t<T> >
		{
			typedef image_t<bmp_gray_format_t<T>, tag_bmp_co_ownership> image_type;
			typedef typename image_type::source_type source_type;
			typedef typename source_type::array_type array_type;
			typedef typename source_type::shared_array_type shared_array_type;
			
			static image_type create(coord_t w, coord_t h)
			{
				return create_co_ownership_bmp_image( w, h, bmp_gray_format_t<T>() );
			}
		};
		template<typename T>
		struct co_ownership_image_provider< bmp_rgb2_format_t<T> >
		{
			typedef image_t<bmp_rgb2_format_t<T>, tag_bmp_co_ownership> image_type;
			typedef typename image_type::source_type source_type;
			typedef typename source_type::array_type array_type;
			typedef typename source_type::shared_array_type shared_array_type;
			
			static image_type create(coord_t w, coord_t h)
			{
				return create_co_ownership_bmp_image( w, h, bmp_rgb2_format_t<T>() );
			}
		};
		template<typename T>
		struct co_ownership_image_provider< bmp_mono_format_t<T> >
		{
			typedef image_t<bmp_mono_format_t<T>, tag_bmp_co_ownership> image_type;
			typedef typename image_type::source_type source_type;
			typedef typename source_type::array_type array_type;
			typedef typename source_type::shared_array_type shared_array_type;
			
			static image_type create(coord_t w, coord_t h)
			{
				return create_co_ownership_bmp_image( w, h, bmp_mono_format_t<T>() );
			}
		};
	}

	

	// color pallet provider

	template<typename ImageType>
	inline bmp_color_pallet_type& get_bmp_color_pallet(ImageType& image)
	{
		return *image.source().pallet_;
	}

	template<typename ImageType>
	inline const bmp_color_pallet_type& get_bmp_color_pallet(const ImageType& image)
	{
		return *image.source().pallet_;
	}
}