#include <gtest/gtest.h>
#include <Windows.h>
#include <algorithm>
#include "../ImageLib/Bitmap/BitmapImage.h"
#include "TestTimer.h"

namespace
{
	using  namespace image_lib;

	struct color_pixel_for_test
	{
		color_pixel_for_test() : values_( 3, 0 )
		{  }
		color_pixel_for_test(BYTE r, BYTE g, BYTE b) : values_( 3, 0 )
		{ 
		values_[0] = r; values_[1] = g; values_[2] = b;
		}

		BYTE operator[](size_t pos) const
		{
			return values_[pos];
		}

		color_pixel_for_test& operator++()
		{	
			std::for_each(values_.begin(), values_.end(), [](BYTE& val){ ++val; });
			return (*this);
		}

		color_pixel_for_test operator++(int)
		{	
			color_pixel_for_test tmp = *this;
			++*this;
			return (tmp);
		}

	private:
		std::vector<BYTE> values_;
	};

	struct plover_generator
	{
		typedef bool result_type;

		plover_generator() : b_(false) {}

		result_type operator()()
		{
			b_= !b_;
			return b_;
		} 
	private:
		bool b_;
	};


	inline bmp_color_pallet_type create_gray_histgram()
	{
		bmp_color_pallet_type pallet(256);
		for(size_t i = 0, imax = pallet.size(); i != imax; ++i)
		{
			BYTE px = static_cast<BYTE>(i);
			quad_type q = { px, px, px, 0 };
			pallet[i]=q;
		}
		return pallet;
	}

	// ピクセル妥当性テスト
	TEST(T_BmpMonoIter, pixel_rgb2_test)
	{
		unsigned char values[] = {10,11,12};
		unsigned char values2[] = {9,11,12};
		non_ownership_bmp_rgb2_image::value_type px(values);
		non_ownership_bmp_rgb2_image::const_reference ref(px);
		non_ownership_bmp_rgb2_image::value_type px2(ref);
		non_ownership_bmp_rgb2_image::value_type px3(values2);
		non_ownership_bmp_rgb2_image::value_type px4(px);
		ASSERT_EQ( 10, ref[0] );
		ASSERT_EQ( 11, ref[1] );
		ASSERT_EQ( 12, ref[2] );
		ASSERT_EQ( 10, px2[0] );
		ASSERT_EQ( 11, px2[1] );
		ASSERT_EQ( 12, px2[2] );
		ASSERT_TRUE( ref == px2 );
		ASSERT_TRUE( ref != px3 );
		ASSERT_TRUE( px == px4 );
	}

	TEST(T_BmpMonoIter, pixel_mono_test)
	{
		non_ownership_bmp_mono_image::value_type px(true);
		non_ownership_bmp_mono_image::const_reference ref(px);
		non_ownership_bmp_mono_image::value_type px2(ref);
		non_ownership_bmp_mono_image::value_type px3(false);
		non_ownership_bmp_mono_image::value_type px4(px);
		ASSERT_EQ( true, ref );
		ASSERT_EQ( true, px2 );
		ASSERT_TRUE( ref == px2 );
		ASSERT_TRUE( ref != px3 );
		ASSERT_TRUE( px == px4 );
	}

	// サイズ妥当性テスト
	TEST(T_BmpMonoIter, bmp_mono_size_test)
	{
		auto image = create_co_ownership_bmp_image( 7,7,bmp_mono_format() );
		ASSERT_EQ( 7*7, image_size(image) );
		ASSERT_EQ( 4*7, image_size_in_byte(image) );
		ASSERT_EQ( 4*7, array_size_of_image(image) );
	}


	// 基本テスト：画像を埋め尽くす
	TEST(T_BmpMonoIter, fill_bmp_rgb2_img)
	{
		auto dstBmpMono =  create_co_ownership_bmp_image( 16, 16, bmp_rgb2_format() );

		std::fill(dstBmpMono.begin(), dstBmpMono.end(), true);

		const BYTE* buffer = dstBmpMono.address();

		// 本当はビットマップ座標系で考えないといけないが、
		// 埋め尽くしに関しては通常座標系でも問題ない
		for(size_t i = 0; i < 16; ++i)
			for(size_t j = 0; j < 4; ++j)
			{
				if(j<2) ASSERT_EQ( 255, buffer[i * 4 + j] );
				else    ASSERT_EQ(   0, buffer[i * 4 + j] );
			}
	}

	TEST(T_BmpMonoIter, fill_bmp_mono_img)
	{
		auto dstBmpMono =  create_co_ownership_bmp_image( 16, 16, bmp_mono_format() );

		std::fill(dstBmpMono.begin(), dstBmpMono.end(), true);

		const BYTE* buffer = dstBmpMono.address();

		// 本当はビットマップ座標系で考えないといけないが、
		// 埋め尽くしに関しては通常座標系でも問題ない
		for(size_t i = 0; i < 16; ++i)
			for(size_t j = 0; j < 4; ++j)
			{
				if(j<2) ASSERT_EQ( 255, buffer[i * 4 + j] );
				else    ASSERT_EQ(   0, buffer[i * 4 + j] );
			}
	}

	// パラメータ化テスト

	class PT_BmpMonoIter : public ::testing::TestWithParam<size_t>
	{	
	public:
		enum{ size = 16, valid_width_in_byte = size/8, width_in_byte = valid_width_in_byte < 4 ? 4 : valid_width_in_byte };
	};


	// 指定された行に対して値を設定する
	TEST_P(PT_BmpMonoIter, fill_bmp_rgb2_row) 
	{
		auto dstMonoBmp (create_co_ownership_bmp_image( size, size, bmp_rgb2_format() ));

		const size_t line_of_bmp_coord = GetParam();
		const size_t line_of_array_coord = size - line_of_bmp_coord - 1;

		std::generate(dstMonoBmp.row_begin(line_of_bmp_coord), dstMonoBmp.row_end(line_of_bmp_coord), plover_generator());

		const BYTE* buffer = dstMonoBmp.address();

		for(size_t i = 0; i < width_in_byte; ++i)
			{
				if(i<valid_width_in_byte) ASSERT_EQ( 170, buffer[width_in_byte*line_of_array_coord + i] );
				else                      ASSERT_EQ(   0, buffer[width_in_byte*line_of_array_coord + i] );
			}
	}

	TEST_P(PT_BmpMonoIter, fill_bmp_mono_row) 
	{
		auto dstMonoBmp (create_co_ownership_bmp_image( size, size, bmp_mono_format() ));

		const size_t line_of_bmp_coord = GetParam();
		const size_t line_of_array_coord = size - line_of_bmp_coord - 1;

		std::generate(dstMonoBmp.row_begin(line_of_bmp_coord), dstMonoBmp.row_end(line_of_bmp_coord), plover_generator());

		const BYTE* buffer = dstMonoBmp.address();

		for(size_t i = 0; i < width_in_byte; ++i)
			{
				if(i<valid_width_in_byte) ASSERT_EQ( 170, buffer[width_in_byte*line_of_array_coord + i] );
				else                      ASSERT_EQ(   0, buffer[width_in_byte*line_of_array_coord + i] );
			}
	}

	// 指定された列に対して値を設定する
	TEST_P(PT_BmpMonoIter, fill_bmp_rgb2_col) 
	{
		auto dstMonoBmp (create_co_ownership_bmp_image( size, size, bmp_rgb2_format() ));
		
		const size_t row_of_bmp_coord = GetParam();
		const size_t row_of_array_coord = row_of_bmp_coord/8;

		std::generate(dstMonoBmp.col_begin(row_of_bmp_coord), dstMonoBmp.col_end(row_of_bmp_coord), plover_generator());

		const BYTE* buffer = dstMonoBmp.address();

		const BYTE expected = 1 << (7-(row_of_bmp_coord%8));

		for(size_t i = 0; i < size; ++i)
			{
				if((i%2)==1) ASSERT_EQ( expected, buffer[ i * width_in_byte + row_of_array_coord ] );
				else         ASSERT_EQ(        0, buffer[ i * width_in_byte + row_of_array_coord ] );
			}
	}

	TEST_P(PT_BmpMonoIter, fill_bmp_mono_col) 
	{
		auto dstMonoBmp (create_co_ownership_bmp_image( size, size, bmp_mono_format() ));
		
		const size_t row_of_bmp_coord = GetParam();
		const size_t row_of_array_coord = row_of_bmp_coord/8;

		std::generate(dstMonoBmp.col_begin(row_of_bmp_coord), dstMonoBmp.col_end(row_of_bmp_coord), plover_generator());

		const BYTE* buffer = dstMonoBmp.address();

		const BYTE expected = 1 << (7-(row_of_bmp_coord%8));

		for(size_t i = 0; i < size; ++i)
			{
				if((i%2)==1) ASSERT_EQ( expected, buffer[ i * width_in_byte + row_of_array_coord ] );
				else         ASSERT_EQ(        0, buffer[ i * width_in_byte + row_of_array_coord ] );
			}
	}

// 値をいれる行（列）をパラメータ化する
INSTANTIATE_TEST_CASE_P(BmpIter_Category,
                        PT_BmpMonoIter,
                        ::testing::Values((size_t)0, (size_t)PT_BmpMonoIter::size-1));

	
	 struct SubImageCoord
	{
		size_t start_x;
		size_t start_y;
		size_t size_x;
		size_t size_y;

		SubImageCoord(size_t st_x,size_t st_y, size_t sz_x, size_t sz_y)
			: start_x( st_x ), start_y( st_y ), size_x( sz_x ), size_y( sz_y ) {  }

		bool is_in_sub_image(size_t w, size_t h) const
		{
			return w >= start_x && w < (start_x + size_x) && h >= start_y && h < (start_y + size_y);
		}
	};

	class PT_BmpMonoIter_SubImg : public ::testing::TestWithParam<SubImageCoord>
	{		};

	// SubImageを埋め尽くす(8bit)
	TEST_P(PT_BmpMonoIter_SubImg, sub_bmp_rgb2_img)
	{
		auto parentImage (create_co_ownership_bmp_image(10,10,bmp_rgb2_format()));
		const SubImageCoord subImageCoord = GetParam(); 
		auto subImage = sub_image( parentImage, subImageCoord.size_x, subImageCoord.size_y, subImageCoord.start_x, subImageCoord.start_y );
		std::fill( subImage.begin(), subImage.end(), true );

		for(size_t i = 0; i < 10; ++i)
			for(size_t j = 0; j < 10; ++j)
			{
				if( subImageCoord.is_in_sub_image(j,i) )
				{
					ASSERT_EQ( 255, parentImage[i][j][0] );
					ASSERT_EQ( 255, parentImage[i][j][0] );
					ASSERT_EQ( 255, parentImage[i][j][0] );
				}
				else
				{
					ASSERT_EQ( 0,  parentImage[i][j][0] );
					ASSERT_EQ( 0,  parentImage[i][j][0] );
					ASSERT_EQ( 0,  parentImage[i][j][0] );
				}
			}
	}

	TEST_P(PT_BmpMonoIter_SubImg, sub_bmp_mono_img)
	{
		auto parentImage (create_co_ownership_bmp_image(10,10,bmp_mono_format()));
		const SubImageCoord subImageCoord = GetParam(); 
		auto subImage = sub_image( parentImage, subImageCoord.size_x, subImageCoord.size_y, subImageCoord.start_x, subImageCoord.start_y );
		std::fill( subImage.begin(), subImage.end(), true );

		for(size_t i = 0; i < 10; ++i)
			for(size_t j = 0; j < 10; ++j)
			{
				if( subImageCoord.is_in_sub_image(j,i) )
				{
					ASSERT_EQ( true, parentImage[i][j] );
				}
				else
				{
					ASSERT_EQ( false,  parentImage[i][j] );
				}
			}
	}

INSTANTIATE_TEST_CASE_P(BmpIter_Category,
                        PT_BmpMonoIter_SubImg,
                        ::testing::Values(SubImageCoord(0,0,5,5), 
										  SubImageCoord(5,0,5,5),
										  SubImageCoord(0,5,5,5),
										  SubImageCoord(5,5,5,5),
										  SubImageCoord(0,0,1,1),
										  SubImageCoord(0,0,10,10)) );
}