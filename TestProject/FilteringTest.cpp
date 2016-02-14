#include <gtest/gtest.h>
#include <Windows.h>
#include <algorithm>
#include "../ImageLib/ImageAll.h"

namespace
{
	using  namespace image_lib;

	template<typename ImageType, typename AveragingPolicy>
	inline void fill_by_average(ImageType& image, const AveragingPolicy& func)
	{
		auto avg = std::for_each( image.begin(), image.end(), func).average();
		std::fill( image.begin(), image.end(), avg );
	}

	template<typename ImageType, typename AveragingPolicy>
	inline void apply_averaging_filter(ImageType& image, size_t filter_size, const AveragingPolicy& func)
	{
		for(size_t i = 0, imax = image.height() - filter_size; i < imax; i+=filter_size)
			for(size_t j = 0, jmax = image.width()- filter_size; j < jmax; j+=filter_size)
			{
				auto subimage = sub_image( image, filter_size, filter_size, j, i );
				fill_by_average( subimage, func );
			}
	}

	struct averaging_filter
	{
		std::vector<unsigned int> init_;
		size_t times_;
		
		averaging_filter() : init_( 3, 0 ), times_( 0 ) {
		}

		std::vector<unsigned int> average() const{
			if (!times_) return init_;
			std::vector<unsigned int> dst( init_.size() );
			std::transform(init_.begin(), init_.end(), dst.begin(), [this](unsigned int val){
				return static_cast<unsigned int>(val / times_);
			});
			return dst;
		}

		template<typename pixel>
		void operator()(const pixel& pic)
		{
			++times_;
			init_[0] += pic[0];
			init_[1] += pic[1];
			init_[2] += pic[2];
		}
	};

	// 基本テスト：画像を埋め尽くす(8bit)
	TEST(T_avg_filter, filter)
	{
		auto image (create_planar_image(5,5));
		typedef decltype(image) image_type;
		int rgb[] = { 64, 128, 192 };
		int rgb2[] = { 32, 64, 128 };
		std::fill( image.begin(), image.end(), rgb );
		apply_averaging_filter( image, 2, averaging_filter() );

		ASSERT_TRUE( 
			std::all_of( image.begin(), image.end(), [](image_type::const_reference ref){
				return ref[0] == 64 &&
					ref[1] == 128 &&
					ref[2] == 192;
		}) );

		image[1][1] = rgb2;
		apply_averaging_filter( image, 2, averaging_filter() );

		enum{
			ans_r = (64*3+32)/4,
			ans_g = (128*3+64)/4,
			ans_b = (192*3+128)/4
		};

		ASSERT_EQ( ans_r, image[0][0][0] );
		ASSERT_EQ( ans_g, image[0][0][1] );
		ASSERT_EQ( ans_b, image[0][0][2] );

	}

}