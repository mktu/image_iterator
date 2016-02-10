#include <gtest/gtest.h>
#include <Windows.h>
#include <algorithm>
#include "../ImageLib/ImageAll.h"

namespace
{
	using  namespace image_lib;
	typedef std::vector<BYTE> ByteArray;

	// ピクセル妥当性テスト
	TEST(T_MonoIter, mono_pixel_test)
	{
		co_ownership_gray8u_image::value_type px(10);
		co_ownership_gray8u_image::const_reference ref(px);
		co_ownership_gray8u_image::value_type px2(ref);
		co_ownership_gray8u_image::value_type px3(11);
		co_ownership_gray8u_image::value_type px4(px);
		ASSERT_EQ( 10, ref );
		ASSERT_EQ( 10, px2 );
		ASSERT_TRUE( ref == px );
		ASSERT_TRUE( ref != px3 );
		ASSERT_TRUE( px == px4 );
	}

	// サイズ妥当性テスト
	TEST(T_MonoIter, mono_size_test)
	{
		typedef gray_format_t<long> long_based_format;
		auto image = create_co_ownership_image( 10,10,long_based_format() );
		ASSERT_EQ( 100, image_size(image) );
		ASSERT_EQ( sizeof(long) * 100, image_size_in_byte(image) );
		ASSERT_EQ( 100, array_size_of_image(image) );
	}

	// 画像を埋め尽くす
	TEST(T_MonoIter, fill_mono_img)
	{
		co_ownership_gray8u_image psDstGray( create_co_ownership_image(10,10,gray8u_format()) );
		
		std::for_each(psDstGray.begin(), psDstGray.end(), [](co_ownership_gray8u_image::reference& b){ b = 128; });

		ASSERT_TRUE( std::all_of( psDstGray.begin(), psDstGray.end(), 
			[](const co_ownership_gray8u_image::reference& b)->bool{ return b == 128;} ) );
	}

// パラメータ化テスト
	// XIteratorテスト
	class PT_MonoIter_Row : public ::testing::TestWithParam<size_t>
	{	
	public:
		enum{ size_x = 6, size_y = 3 };
	protected:
		ByteArray array_;

		PT_MonoIter_Row() : array_(size_x*size_y, 0) { }
	};

	// 指定された行に対して値を設定する
	TEST_P(PT_MonoIter_Row, fill_mono_row) 
	{
		non_ownership_gray8u_image image (create_non_ownership_image( &array_[0], size_x, size_y, gray8u_format() ));
		const size_t line = GetParam();
		std::fill( image.row_begin(line), image.row_end(line), 128 );
		
		auto start_iterator = array_.begin()+size_x*line;

		ASSERT_TRUE( std::all_of( start_iterator,	start_iterator+size_x, [](BYTE b){return b == 128;} ) );
	}


INSTANTIATE_TEST_CASE_P(MonoIter_Category,
                        PT_MonoIter_Row,
                        ::testing::Values((size_t)0, (size_t)PT_MonoIter_Row::size_y-1));

	// YIteratorテスト
	class PT_MonoIter_Col : public ::testing::TestWithParam<size_t>
	{	
	public:
		enum{ size_x = 6, size_y = 3 };
	protected:
		ByteArray array_;

		PT_MonoIter_Col() : array_(size_x*size_y, 0) { }
	};

	// 指定された列に対して値を設定する
	TEST_P(PT_MonoIter_Col, fill_mono_col) 
	{
		non_ownership_gray8u_image image (create_non_ownership_image( &array_[0], size_x, size_y, gray8u_format() ));
		const size_t col = GetParam();
		std::fill( image.col_begin(col), image.col_end(col), 128 );
		
		auto start_iterator = array_.begin()+col;

		for(size_t i = 0; i < size_y; ++i)
		{
			ASSERT_EQ( 128,  *(start_iterator + size_x * i) );
		}
	}


INSTANTIATE_TEST_CASE_P(MonoIter_Category,
                        PT_MonoIter_Col,
                        ::testing::Values((size_t)0, (size_t)PT_MonoIter_Col::size_x-1));
}

#ifdef _RELEASE

#include "TestTimer.h"

namespace
{
	TEST(IteratorTest2, mono_simple_pointer_speed_test) 
	{
		ByteArray array_(4096*1024);
		ByteArray::pointer pointer = &array_[0];

		timer_on t;

		for(int i = 0; i < 1024; i++)
			for(int j = 0, offset = i * 4096; j < 4096; j ++)
			{
				pointer[ offset + j ] = 128;
			}

	}

	TEST(IteratorTest2, mono_image_t_speed_test) 
	{
		ByteArray array_(4096*1024);
		non_ownership_gray8u_image image (create_non_ownership_image( &array_[0], 4096, 1024, gray8u_format() ));

		timer_on t;

		std::fill( image.begin(), image.end(), 128 );
		
	}

	TEST(IteratorTest2, mono_image_t_array_like_access_speed_test) 
	{
		ByteArray array_(4096*1024);
		non_ownership_gray8u_image image ( create_non_ownership_image( &array_[0], 4096, 1024, gray8u_format() ));

		timer_on t;

		for(int i = 0; i < 1024; i++)
			for(int j = 0; j < 4096; j ++)
			{
				image(j,i) = 128;
			}
		
	}
}
#endif