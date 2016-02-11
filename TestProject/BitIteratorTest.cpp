#include <gtest/gtest.h>
#include <Windows.h>
#include <algorithm>
#include "../ImageLib/ImageAll.h"
#include <bitset>
#include "TestTimer.h"

namespace
{
	using  namespace image_lib;
	typedef bit_pixel_format<unsigned short> u16_based_bit_image;
	typedef image_t<u16_based_bit_image, tag_co_ownership> co_ownership_bit_image_16;
	typedef image_t<u16_based_bit_image, tag_non_ownership> non_ownership_bit_image_16;

	// ピクセル妥当性テスト
	TEST(T_BitIter, bit_pixel_test)
	{
		co_ownership_bit_image::value_type px(true);
		co_ownership_bit_image::const_reference ref(px);
		co_ownership_bit_image::value_type px2(ref);
		co_ownership_bit_image::value_type px3(false);
		co_ownership_bit_image::value_type px4(px);
		ASSERT_EQ( true, ref );
		ASSERT_EQ( true, px2 );
		ASSERT_TRUE( ref == px2 );
		ASSERT_TRUE( ref != px3 );
		ASSERT_TRUE( px == px4 );
	}

	// サイズ妥当性テスト
	TEST(T_BitIter, bit_size_test)
	{
		typedef bit_pixel_format<long> bit32_format;
		auto image = create_co_ownership_image( 7,7,bit32_format() );
		ASSERT_EQ( 7*7, image_size(image) );
		ASSERT_EQ( static_cast<size_t>(ceil(7.0*7.0/8.0)), image_size_in_byte(image) );
		ASSERT_EQ( static_cast<size_t>(ceil(7.0*7.0/(8.0*4.0))), array_size_of_image(image) );
	}

	// サイズ妥当性テスト
	TEST(T_BitIter, bit_size_test2)
	{
		typedef bit_pixel_format<long> bit32_format;
		auto image = create_co_ownership_image( 65,65,bit32_format() );
		ASSERT_EQ( 65*65, image_size(image) );
		ASSERT_EQ( static_cast<size_t>(ceil(65.0*65.0/8.0)), image_size_in_byte(image) );
		ASSERT_EQ( static_cast<size_t>(ceil(65.0*65.0/(8.0*4.0))), array_size_of_image(image) );
	}

	// 基本テスト：画像を埋め尽くす(8bit)
	TEST(T_BitIter, fill_bit_img_byte)
	{
		co_ownership_bit_image psDstBit (create_co_ownership_image(16,16,bit8u_format()));
		
		std::fill(psDstBit.begin(), psDstBit.end(), true);

		ASSERT_TRUE( std::all_of( psDstBit.begin(), psDstBit.end(), 
			[](const co_ownership_bit_image::reference& b)->bool{ return b == true;} ) );
	}

	// 基本テスト：画像を埋め尽くす(16bit)
	// ただしここでは、アドレスから値の妥当性をチェックしている
	TEST(T_BitIter, fill_bit_img_short)
	{
		co_ownership_bit_image_16 psDstBit( create_co_ownership_image(16,16,u16_based_bit_image()) );

		std::fill( psDstBit.begin(),  psDstBit.end(), true);

		unsigned short* p = psDstBit.address();

		ASSERT_TRUE( std::all_of(p,p + 16,[](unsigned short t){return t==USHRT_MAX;}) );

	}

// パラメータ化テスト
	class PT_BitIter_Row : public ::testing::TestWithParam<size_t>
	{	
	public:
		enum{ size_x_in_bit = 8, size_x_in_byte = size_x_in_bit / 8, size_y = 16 };
		enum{ size_x_in_bit_short_type = 16, size_x_in_short = 1 };
	};

	// 指定された行に対して値を設定する(8bit)
	TEST_P(PT_BitIter_Row, fill_bit_row_byte)
	{
		std::vector<BYTE> v(size_x_in_byte*size_y,0);
		non_ownership_bit_image psDstBit (create_non_ownership_image(&v[0],size_x_in_bit,size_y, bit8u_format()));
		const size_t line = GetParam();

		std::fill(psDstBit.row_begin(line), psDstBit.row_end(line), true );

		for(size_t i = 0; i < size_y; ++i)
		{
			if (i == line) ASSERT_EQ( 255, v[i] );
			else		   ASSERT_EQ( 0,   v[i] );
		}
	}

	

	// 指定された行に対して値を設定する(16bit)
	TEST_P(PT_BitIter_Row, fill_bit_row_short)
	{
		std::vector<unsigned short> v(size_x_in_short * size_y,0);
		non_ownership_bit_image_16 psDstBit (create_non_ownership_image(&v[0],size_x_in_bit_short_type,size_y,u16_based_bit_image()));
		const size_t line = GetParam();

		std::fill( psDstBit.row_begin(line),  psDstBit.row_end(line), true);

		for(size_t i = 0; i < size_y; ++i)
		{
			if (i == line) ASSERT_EQ( USHRT_MAX, v[i] );
			else		   ASSERT_EQ( 0,         v[i] );
		}

	}

INSTANTIATE_TEST_CASE_P(BitIter_Category,
                        PT_BitIter_Row,
                        ::testing::Values((size_t)0, (size_t)PT_BitIter_Row::size_y-1));

	class PT_BitIter_Col : public ::testing::TestWithParam<size_t>
	{	
	public:
		enum{ size_x_in_bit = 8, size_x_in_byte = size_x_in_bit / 8, size_y = 16 };
		enum{ size_x_in_bit_short_type = 16, size_x_in_short = 1 };
	};

	// 指定された列に対して値を設定する(8bit)
	TEST_P(PT_BitIter_Col, fill_bit_col_test_byte)
	{
		std::vector<BYTE> v(size_x_in_byte*size_y,0);
		non_ownership_bit_image psDstBit (create_non_ownership_image(&v[0],size_x_in_bit,size_y,bit8u_format()));
		const size_t col = GetParam();

		std::fill(psDstBit.col_begin(col), psDstBit.col_end(col), true );
		std::bitset<8> expected(0);
		expected[7-col] = true;

		ASSERT_TRUE(
			std::all_of(v.begin(), v.end(), [expected](BYTE v){ return v == static_cast<BYTE>(expected.to_ulong()); }));

	}


	// 指定された列に対して値を設定する(16bit)
	TEST_P(PT_BitIter_Col, fill_bit_col_short)
	{
		std::vector<unsigned short> v(size_x_in_short * size_y,0);
		non_ownership_bit_image_16 psDstBit (create_non_ownership_image(&v[0],size_x_in_bit_short_type,size_y,u16_based_bit_image()));
		const size_t col = GetParam();

		std::fill( psDstBit.col_begin(col),  psDstBit.col_end(col), true);
		std::bitset<16> expected(0);
		expected[15-col] = true;

		ASSERT_TRUE(
			std::all_of(v.begin(), v.end(), [expected](unsigned short v){ return v == static_cast<unsigned short>(expected.to_ulong()); }));

	}

INSTANTIATE_TEST_CASE_P(BitIter_Category,
                        PT_BitIter_Col,
                        ::testing::Values((size_t)0, (size_t)PT_BitIter_Col::size_x_in_bit-1));


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

	class PT_BitIter_SubImg : public ::testing::TestWithParam<SubImageCoord>
	{		};

	// SubImageを埋め尽くす(8bit)
	TEST_P(PT_BitIter_SubImg, sub_img_byte)
	{
		co_ownership_bit_image parentImage (create_co_ownership_image(10,10,bit8u_format()));
		const SubImageCoord subImageCoord = GetParam(); 
		auto subImage = sub_image( parentImage, subImageCoord.size_x, subImageCoord.size_y, subImageCoord.start_x, subImageCoord.start_y );
		std::fill( subImage.begin(), subImage.end(), true );

		for(size_t i = 0; i < 10; ++i)
			for(size_t j = 0; j < 10; ++j)
			{
				if( subImageCoord.is_in_sub_image(j,i) )
				{
					ASSERT_TRUE( parentImage[i][j] );
					ASSERT_TRUE( parentImage[i][j] );
					ASSERT_TRUE( parentImage[i][j] );
				}
				else
				{
					ASSERT_FALSE( parentImage[i][j] );
					ASSERT_FALSE( parentImage[i][j] );
					ASSERT_FALSE( parentImage[i][j] );
				}
			}
	}

INSTANTIATE_TEST_CASE_P(BitIter_Category,
                        PT_BitIter_SubImg,
                        ::testing::Values(SubImageCoord(1,1,5,5), 
										  SubImageCoord(4,0,5,5),
										  SubImageCoord(0,4,5,5),
										  SubImageCoord(4,4,5,5),
										  SubImageCoord(0,0,1,1),
										  SubImageCoord(0,0,10,10)) );

#ifdef _RELEASE
	TEST(T_BitIter, it_speed_test_1) 
	{
		co_ownership_bit_image image (create_co_ownership_image( 4096, 1024, bit8u_format() ));
		timer_on t;
		std::fill( image.begin(), image.end(), true );
	}
#endif
}