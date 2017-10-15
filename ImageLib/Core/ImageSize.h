#pragma once

#include <math.h>
#include <cstddef>

namespace image_lib
{
	template<typename T, size_t channels>
	struct image_size_policy
	{
		enum{ 
			byte_size = sizeof(T),
			pixel_byte_size = byte_size * channels
		};

		static size_t image_size_in_byte(size_t w, size_t h)
		{
			return pixel_byte_size * w * h;
		}
		
		static size_t image_size_in_logical_coordinate(size_t w, size_t h)
		{
			return channels * w * h;
		}

		static size_t array_size(size_t w, size_t h)
		{
			return image_size_in_logical_coordinate(w,h);
		}

	};

	template<typename T>
	struct bit_image_size_policy
	{
		enum{ 
			byte_size = sizeof(T)
		};

		static double inverse_of_bit_size() 
		{
			static const double cache = 1.0 / (byte_size * 8.0);
			return cache;
		}

		static size_t image_size_in_byte(size_t w, size_t h)
		{
			return static_cast<size_t>( ceil(w*h/8.0) );
		}

		static size_t image_size_in_logical_coordinate(size_t w, size_t h)
		{
			return w * h;
		}

		static size_t array_size(size_t w, size_t h)
		{
			return static_cast<size_t>( ceil(inverse_of_bit_size() * w * h) );
		}
	};


	template<typename T, size_t channels>
	struct planar_image_size_policy
	{
		enum{ 
			byte_size = sizeof(T),
			pixel_byte_size = byte_size * channels
		};

		static size_t image_size_in_byte(size_t w, size_t h)
		{
			return pixel_byte_size * w * h;
		}

		static size_t image_size_in_logical_coordinate(size_t w, size_t h)
		{
			return channels * w * h;
		}

		static size_t array_size(size_t w, size_t h)
		{
			return image_size_in_logical_coordinate(w,h);
		}
	};
}