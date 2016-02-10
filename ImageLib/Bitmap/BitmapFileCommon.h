#pragma once

#include <Windows.h>
#include <memory>
#include <string>
#include "BmpImageSource.h"

namespace image_lib
{
	typedef ptrdiff_t coord_t;

	class bitmap_type
	{
	public:
		enum bit_count
		{
			BITMAP_1  = 1,
			BITMAP_8  = 8,
			BITMAP_24 = 24,
			BITMAP_32 = 32
		};

	private:
		bit_count make_type(WORD bitCount)
		{
			switch(bitCount)
			{
			case 1:  return BITMAP_1;
			case 8:  return BITMAP_8;
			case 24: return BITMAP_24;
			}
			return BITMAP_32;
		}

	public:
		
		bitmap_type(WORD bitCount) : type_( make_type(bitCount) ){ 	}

		operator bit_count() const { return type_; }

		std::string to_string() const 
		{
			switch(type_)
			{
			case BITMAP_1 : return "1bit bitmap";
			case BITMAP_8 : return "8bit bitmap";
			case BITMAP_24: return "24bit bitmap";
			}
			return "";
		}

	private:

		bit_count type_;
	};

	namespace detail
	{
		template<bitmap_type::bit_count type>
		inline bmp_color_pallet_type create_default_color_pallet();

		template<>
		inline bmp_color_pallet_type create_default_color_pallet<bitmap_type::BITMAP_1>()
		{

			enum{ pallet_size = 1 << bitmap_type::BITMAP_1 };

			bmp_color_pallet_type pallet(pallet_size);
			for(size_t i = 0; i < 4; ++i)
			{
				pallet[0].byte_[i]=0;
				pallet[1].byte_[i]=255;
			}
			return pallet;
		}
		
		template<>
		inline bmp_color_pallet_type create_default_color_pallet<bitmap_type::BITMAP_8>()
		{
			enum{ pallet_size = 1 << bitmap_type::BITMAP_8 };

			bmp_color_pallet_type pallet(pallet_size);
			for(size_t i = 0; i < pallet_size; ++i )
				for(size_t j = 0; j < 4; ++j)
					pallet[i].byte_[j]=static_cast<BYTE>(i);
			return pallet;
		}
		
		template<>
		inline bmp_color_pallet_type create_default_color_pallet<bitmap_type::BITMAP_24>()
		{
			return bmp_color_pallet_type();
		}
	}
}