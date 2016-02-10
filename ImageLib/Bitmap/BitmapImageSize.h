#pragma once

namespace image_lib
{
	inline size_t padded_width_in_color_coordinate(size_t width_in_color_coordinate)
	{
		size_t pad = 0;
		for( ; (width_in_color_coordinate+pad)%4 != 0; ++pad);
		return width_in_color_coordinate + pad;
	};

	template<typename T, size_t channels>
	struct bitmap24_image_size_policy
	{
		enum{ 
			byte_size = sizeof(T),
			pixel_byte_size = byte_size * channels
		};

		static size_t row_size_in_byte(size_t w)
		{
			return padded_width_in_color_coordinate( pixel_byte_size * w );
		}

		static size_t image_size_in_byte(size_t w, size_t h)
		{
			return row_size_in_byte(w) * h;
		}

		static size_t image_size_in_logical_coordinate(size_t w, size_t h)
		{
			return channels * w * h;
		}

		static size_t array_size(size_t w, size_t h)
		{
			return image_size_in_byte( w, h ) / byte_size;
		}
	};

	template<typename T>
	struct bitmap256_image_size_policy
	{
		enum{ 
			byte_size = sizeof(T),
			pixel_byte_size = byte_size
		};

		static size_t row_size_in_byte(size_t w)
		{
			return padded_width_in_color_coordinate( pixel_byte_size * w );
		}

		static size_t image_size_in_byte(size_t w, size_t h)
		{
			return row_size_in_byte(w) * h;
		}

		static size_t padded_image_size_in_logical_coordinate(size_t w, size_t h)
		{
			return padded_width_in_color_coordinate(w) * h;
		}

		static size_t image_size_in_logical_coordinate(size_t w, size_t h)
		{
			return w * h;
		}

		static size_t array_size(size_t w, size_t h)
		{
			return padded_width_in_color_coordinate(w) * h;
		}
	};

	template<typename T>
	struct bitmap1_image_size_policy
	{
		enum{ 
			byte_size = sizeof(T)
		};

		static double inverse_of_bit_size() 
		{
			static const double cache = 1.0 / (byte_size * 8.0);
			return cache;
		}

		static size_t row_size_in_byte(size_t w)
		{
			return padded_width_in_color_coordinate( static_cast<size_t>( ceil(w/8.0) ) );
		}

		static size_t image_size_in_byte(size_t w, size_t h)
		{
			return row_size_in_byte(w) * h;
		}

		static size_t image_size_in_logical_coordinate(size_t w, size_t h)
		{
			return w * h;
		}

		static size_t array_size(size_t w, size_t h)
		{
			return padded_width_in_color_coordinate( static_cast<size_t>( ceil(w*inverse_of_bit_size()) ) ) * h;
		}
	};
}