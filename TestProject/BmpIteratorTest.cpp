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

	template<typename pixel_type>
	struct incremental_generator
	{
		typedef pixel_type result_type;

		incremental_generator() : count_( 0 )
		{  }

		incremental_generator(result_type start) : count_(start)
		{  }

		result_type operator()()
		{
			return count_++;
		} 

	private:
		result_type count_;
	};

	typedef incremental_generator<BYTE> incremental_generator_for_bit8;
	typedef incremental_generator<color_pixel_for_test> incremental_generator_for_bit24;

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
	TEST(T_BmpIter, pixel_24bit_test)
	{
		unsigned char values[] = {10,11,12};
		unsigned char values2[] = {9,11,12};
		co_ownership_bmp_rgb24_image::value_type px(values);
		co_ownership_bmp_rgb24_image::const_reference ref(px);
		co_ownership_bmp_rgb24_image::value_type px2(ref);
		co_ownership_bmp_rgb24_image::value_type px3(values2);
		co_ownership_bmp_rgb24_image::value_type px4(px);
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

	// ピクセル妥当性テスト
	TEST(T_BmpIter, pixel_8bit_test)
	{
		co_ownership_bmp_gray_image::value_type px(10);
		co_ownership_bmp_gray_image::const_reference ref(px);
		co_ownership_bmp_gray_image::value_type px2(ref);
		co_ownership_bmp_gray_image::value_type px3(12);
		co_ownership_bmp_gray_image::value_type px4(px);

		ASSERT_EQ( 10, ref );
		ASSERT_EQ( 10, px2 );
		ASSERT_TRUE( ref == px2 );
		ASSERT_TRUE( ref != px3 );
		ASSERT_TRUE( px == px4 );
	}

	// ピクセル妥当性テスト
	TEST(T_BmpIter, pixel_8bit_color_test)
	{
		unsigned char values[] = {10,11,12};
		unsigned char values2[] = {9,11,12};
		co_ownership_bmp_rgb256_image::value_type px(values);
		co_ownership_bmp_rgb256_image::const_reference ref(px);
		co_ownership_bmp_rgb256_image::value_type px2(ref);
		co_ownership_bmp_rgb256_image::value_type px3(values2);
		co_ownership_bmp_rgb256_image::value_type px4(px);
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

	// サイズ妥当性テスト
	TEST(T_BmpIter, bmp24_size_test)
	{
		auto image = create_co_ownership_bmp_image( 7,7,bmp_rgb24_format() );
		ASSERT_EQ( 7*7*3, image_size(image) );
		ASSERT_EQ( 24*7, image_size_in_byte(image) );
		ASSERT_EQ( 24*7, array_size_of_image(image) );
	}

	// サイズ妥当性テスト
	TEST(T_BmpIter, bmp256_size_test)
	{
		auto image = create_co_ownership_bmp_image( 7,7,bmp_rgb256_format() );
		ASSERT_EQ( 7*7, image_size(image) );
		ASSERT_EQ( 8*7, image_size_in_byte(image) );
		ASSERT_EQ( 8*7, array_size_of_image(image) );
	}

	// 基本テスト：画像を埋め尽くす
	TEST(T_BmpIter, fill_bmp24_img)
	{
		auto dst24BitBmp( create_co_ownership_bmp_image( 16, 16, bmp_rgb24_format() ) );
		BYTE rgb[] = { 64, 128, 192 };

		std::fill(dst24BitBmp.begin(), dst24BitBmp.end(), rgb);

		const BYTE* buffer = dst24BitBmp.address();

		for(size_t i = 0; i < 16; ++i)
			for(size_t j = 0; j < 16; ++j)
			{
				ASSERT_EQ( 192, buffer[3 * i * 16 + j * 3    ] );
				ASSERT_EQ( 128, buffer[3 * i * 16 + j * 3 + 1] );
				ASSERT_EQ( 64,  buffer[3 * i * 16 + j * 3 + 2] );
			}
	}

	// 基本テスト：画像を埋め尽くす(8bit color)
	TEST(T_BmpIter, fill_bmp_8bit_color_img)
	{
		auto dst8BitBmpColor =  create_co_ownership_bmp_image( 16, 16, bmp_rgb256_format() );

		std::fill(dst8BitBmpColor.begin(), dst8BitBmpColor.end(), 128);

		const BYTE* buffer = dst8BitBmpColor.address();

		for(size_t i = 0; i < 16; ++i)
			for(size_t j = 0; j < 16; ++j)
			{
				ASSERT_EQ( 128, buffer[i * 16 + j] );
			}
	}

	// 4Byte境界ではない画像サイズに対して、画像を埋め尽くす(24bit)
	TEST(T_BmpIter, not_4byte_allign_24bit)
	{
		auto dst24BitBmp( create_co_ownership_bmp_image( 15, 15, bmp_rgb24_format() ) );
		const size_t row_size = 16;
		BYTE rgb[] = { 64, 128, 192 };

		auto it = dst24BitBmp.begin()+(15*15-2);
		*(dst24BitBmp.begin()+15*15-2) = rgb;

		std::fill(dst24BitBmp.begin(), dst24BitBmp.end(), rgb);

		const BYTE* buffer = dst24BitBmp.address();

		for(size_t i = 0; i < 15; ++i)
			for(size_t j = 0; j < 15; ++j)
			{
				ASSERT_EQ( 192, buffer[3 * i * row_size + j * 3    ] );
				ASSERT_EQ( 128, buffer[3 * i * row_size + j * 3 + 1] );
				ASSERT_EQ( 64,  buffer[3 * i * row_size + j * 3 + 2] );
			}
	}

	TEST(T_BmpIter, sort_test)
	{
		auto dst24BitBmp (create_co_ownership_bmp_image( 3, 3, bmp_rgb24_format() ));
		BYTE rgb[] = { 64, 128, 192 };
		std::fill(dst24BitBmp.begin(), dst24BitBmp.end(), rgb);
		dst24BitBmp[2][1][0] = 255;
		dst24BitBmp[1][0][0] = 192;

		std::sort(dst24BitBmp.begin(), dst24BitBmp.end(), [](bmp_rgb24_format::iterator::reference ref1, bmp_rgb24_format::iterator::reference ref2){
			return ref1[0] > ref2[0];
		});

		ASSERT_EQ( 255, (*dst24BitBmp.begin())[0] );
		ASSERT_EQ( 192, (*(dst24BitBmp.begin()+1))[0] );
	}

	// 4Byte境界ではない画像サイズに対して、画像を埋め尽くす(8bit)
	TEST(T_BmpIter, not_4byte_allign_8bit)
	{
		auto dst8BitBmp (create_co_ownership_bmp_image( 15, 15, bmp_gray_format() ));
		const size_t row_size = 16;

		std::fill(dst8BitBmp.begin(), dst8BitBmp.end(), 128);

		const BYTE* buffer = dst8BitBmp.address();

		for(size_t i = 0; i < 15; ++i)
			for(size_t j = 0; j < 15; ++j)
			{
				ASSERT_EQ( 128, buffer[i * row_size + j ] );
			}
	}

	// 4Byte境界ではない画像サイズに対して、画像を埋め尽くす(8bitcolor)
	TEST(T_BmpIter, not_4byte_allign_8bit_color)
	{
		auto dst8BitBmp (create_co_ownership_bmp_image( 15, 15, bmp_rgb256_format() ));
		const size_t row_size = 16;

		std::fill(dst8BitBmp.begin(), dst8BitBmp.end(), 128);

		const BYTE* buffer = dst8BitBmp.address();

		for(size_t i = 0; i < 15; ++i)
			for(size_t j = 0; j < 15; ++j)
			{
				ASSERT_EQ( 128, buffer[i * row_size + j ] );
			}
	}

	// 4Byte境界ではない画像サイズに対して、画像を埋め尽くす(24bit)
	TEST(T_BmpIter, not_4byte_allign_non_ownership_24bit)
	{
		std::vector<BYTE> buffer( 16*15*3 ); // 16 = (15*3+3) / 3
		auto dst24BitBmp( create_non_ownership_bmp_image( top_of_address(&buffer[0]), 15, 15, bmp_rgb24_format() ) );
		const size_t row_size = 16;
		BYTE rgb[] = { 64, 128, 192 };

		std::fill(dst24BitBmp.begin(), dst24BitBmp.end(), rgb);

		for(size_t i = 0; i < 15; ++i)
			for(size_t j = 0; j < 15; ++j)
			{
				SCOPED_TRACE(std::to_string(static_cast<unsigned long long>(i)) + "," + std::to_string(static_cast<unsigned long long>(j)) + ".");
				ASSERT_EQ( 192, buffer[3 * i * row_size + j * 3    ] );
				ASSERT_EQ( 128, buffer[3 * i * row_size + j * 3 + 1] );
				ASSERT_EQ( 64,  buffer[3 * i * row_size + j * 3 + 2] );
			}
	}

// パラメータ化テスト

	class PT_BmpIter : public ::testing::TestWithParam<size_t>
	{	
	public:
		enum{ size = 8 };
	};

	// 指定された行に対して値を設定する(24bitビットマップ）
	TEST_P(PT_BmpIter, fill_bmp_row_24bit) 
	{
		auto dst24BitBmp (create_co_ownership_bmp_image( size, size, bmp_rgb24_format() ));

		const size_t line_of_bmp_coord = GetParam();
		const size_t line_of_array_coord = size - line_of_bmp_coord - 1;

		// 行に対して(64,128,192),(65,129,193)...のように輝度を設定していく
		std::generate(dst24BitBmp.row_begin(line_of_bmp_coord), dst24BitBmp.row_end(line_of_bmp_coord), 
			incremental_generator_for_bit24(color_pixel_for_test(64,128,192)));

		const BYTE* buffer = dst24BitBmp.address();

		for(size_t i = 0; i < size; ++i)
			{
				ASSERT_EQ( static_cast<BYTE>(192+i), buffer[size*3*line_of_array_coord + 3*i] );
				ASSERT_EQ( static_cast<BYTE>(128+i), buffer[size*3*line_of_array_coord + 3*i+1] );
				ASSERT_EQ( static_cast<BYTE>(64+i), buffer[size*3*line_of_array_coord + 3*i+2] );
			}
	}

	// 指定された列に対して値を設定する(24bitビットマップ）
	TEST_P(PT_BmpIter, fill_bmp__col_24bit) 
	{
		auto dst24BitBmp( create_co_ownership_bmp_image( size, size, bmp_rgb24_format() ) );
		
		BYTE rgb[] = { 64, 128, 192 };

		const size_t row_of_bmp_coord = GetParam();
		const size_t row_of_array_coord = row_of_bmp_coord;

		std::generate(dst24BitBmp.col_begin(row_of_bmp_coord), dst24BitBmp.col_end(row_of_bmp_coord), 
			incremental_generator_for_bit24(color_pixel_for_test(64,128,192)));

		const BYTE* buffer = dst24BitBmp.address();

		for(size_t i = 0; i < size; ++i)
			{
				ASSERT_EQ( static_cast<BYTE>(192+size-1-i), buffer[3 * i * size + row_of_array_coord * 3    ] );
				ASSERT_EQ( static_cast<BYTE>(128+size-1-i), buffer[3 * i * size + row_of_array_coord * 3 + 1] );
				ASSERT_EQ( static_cast<BYTE>(64+size-1-i),  buffer[3 * i * size + row_of_array_coord * 3 + 2] );
			}
	}

	// 指定された行に対して値を設定する(8bitビットマップ） 
	TEST_P(PT_BmpIter, fill_bmp__row_8bit) 
	{
		co_ownership_bmp_gray_image dst8BitBmp (create_co_ownership_bmp_image( size, size, bmp_gray_format() ));

		const size_t line_of_bmp_coord = GetParam();
		const size_t line_of_array_coord = size - line_of_bmp_coord - 1;

		std::generate(dst8BitBmp.row_begin(line_of_bmp_coord), dst8BitBmp.row_end(line_of_bmp_coord), incremental_generator_for_bit8());

		const BYTE* buffer = dst8BitBmp.address();

		for(size_t i = 0; i < size; ++i)
			{
				ASSERT_EQ( static_cast<BYTE>(i), buffer[size*line_of_array_coord + i] );
			}
	}

	// 指定された列に対して値を設定する(8bitビットマップ）
	TEST_P(PT_BmpIter, fill_bmp__col_8bit) 
	{
		co_ownership_bmp_gray_image dst8BitBmp (create_co_ownership_bmp_image( size, size, bmp_gray_format() ));
		
		const size_t row_of_bmp_coord = GetParam();
		const size_t row_of_array_coord = row_of_bmp_coord;

		std::generate(dst8BitBmp.col_begin(row_of_bmp_coord), dst8BitBmp.col_end(row_of_bmp_coord), incremental_generator_for_bit8());

		const BYTE* buffer = dst8BitBmp.address();

		for(size_t i = 0; i < size; ++i)
			{
				ASSERT_EQ( static_cast<BYTE>(size-i-1), buffer[ i * size + row_of_array_coord ] );
			}
	}

	// 指定された行に対して値を設定する(8bit colorビットマップ） 
	TEST_P(PT_BmpIter, fill_bmp__row_8bit_color) 
	{
		co_ownership_bmp_rgb256_image dst8BitBmp (create_co_ownership_bmp_image( size, size, bmp_rgb256_format() ));

		const size_t line_of_bmp_coord = GetParam();
		const size_t line_of_array_coord = size - line_of_bmp_coord - 1;

		std::generate(dst8BitBmp.row_begin(line_of_bmp_coord), dst8BitBmp.row_end(line_of_bmp_coord), incremental_generator_for_bit8());

		const BYTE* buffer = dst8BitBmp.address();

		for(size_t i = 0; i < size; ++i)
			{
				ASSERT_EQ( static_cast<BYTE>(i), buffer[size*line_of_array_coord + i] );
			}
	}

	// 指定された列に対して値を設定する(8bit colorビットマップ）
	TEST_P(PT_BmpIter, fill_bmp__col_8bit_color) 
	{
		co_ownership_bmp_rgb256_image dst8BitBmp (create_co_ownership_bmp_image( size, size, bmp_rgb256_format() ));
		
		const size_t row_of_bmp_coord = GetParam();
		const size_t row_of_array_coord = row_of_bmp_coord;

		std::generate(dst8BitBmp.col_begin(row_of_bmp_coord), dst8BitBmp.col_end(row_of_bmp_coord), incremental_generator_for_bit8());

		const BYTE* buffer = dst8BitBmp.address();

		for(size_t i = 0; i < size; ++i)
			{
				ASSERT_EQ( static_cast<BYTE>(size-i-1), buffer[ i * size + row_of_array_coord ] );
			}
	}

// 値をいれる行（列）をパラメータ化する
INSTANTIATE_TEST_CASE_P(BmpIter_Category,
                        PT_BmpIter,
                        ::testing::Values((size_t)0, (size_t)PT_BmpIter::size-1));
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

	class PT_BmpIter_SubImg : public ::testing::TestWithParam<SubImageCoord>
	{		};

	// SubImageを埋め尽くす(24bit)
	TEST_P(PT_BmpIter_SubImg, sub_bmp_img_24bit)
	{
		auto parentImage (create_co_ownership_bmp_image(10,10,bmp_rgb24_format()));
		const SubImageCoord subImageCoord = GetParam(); 
		auto subImage = sub_image( parentImage, subImageCoord.size_x, subImageCoord.size_y, subImageCoord.start_x, subImageCoord.start_y );
		BYTE rgb[] =  {64,128,192};
		std::fill( subImage.begin(), subImage.end(), rgb );

		for(size_t i = 0; i < 10; ++i)
			for(size_t j = 0; j < 10; ++j)
			{
				if( subImageCoord.is_in_sub_image(j,i) )
				{
					ASSERT_EQ( 64,  parentImage[i][j][0] );
					ASSERT_EQ( 128, parentImage[i][j][1] );
					ASSERT_EQ( 192, parentImage[i][j][2] );
				}
				else
				{
					ASSERT_EQ( 0,  parentImage[i][j][0] );
					ASSERT_EQ( 0,  parentImage[i][j][1] );
					ASSERT_EQ( 0,  parentImage[i][j][2] );
				}
			}
	}

	// SubImageを埋め尽くす(8bit)
	TEST_P(PT_BmpIter_SubImg, sub_bmp_img_8bit)
	{
		co_ownership_bmp_gray_image parentImage (create_co_ownership_bmp_image(10,10,bmp_gray_format()));
		const SubImageCoord subImageCoord = GetParam(); 
		auto subImage = sub_image( parentImage, subImageCoord.size_x, subImageCoord.size_y, subImageCoord.start_x, subImageCoord.start_y );
		std::fill( subImage.begin(), subImage.end(), 128 );

		for(size_t i = 0; i < 10; ++i)
			for(size_t j = 0; j < 10; ++j)
			{
				if( subImageCoord.is_in_sub_image(j,i) )
				{
					ASSERT_EQ( 128, parentImage[i][j] );
					ASSERT_EQ( 128, parentImage[i][j] );
					ASSERT_EQ( 128, parentImage[i][j] );
				}
				else
				{
					ASSERT_EQ( 0,  parentImage[i][j] );
					ASSERT_EQ( 0,  parentImage[i][j] );
					ASSERT_EQ( 0,  parentImage[i][j] );
				}
			}
	}

INSTANTIATE_TEST_CASE_P(BmpIter_Category,
                        PT_BmpIter_SubImg,
                        ::testing::Values(SubImageCoord(0,0,5,5), 
										  SubImageCoord(5,0,5,5),
										  SubImageCoord(0,5,5,5),
										  SubImageCoord(5,5,5,5),
										  SubImageCoord(0,0,1,1),
										  SubImageCoord(0,0,10,10)) );

	// ネストしたsubimageへのアクセス
	TEST(T_BmpIter, nested_sub_bmp_img)
	{
		auto parentImage (create_co_ownership_bmp_image(10,10,bmp_rgb24_format()));
		auto subImage = sub_image( parentImage,5,5,5,5 );
		auto subsubImage =  sub_image(subImage,2,2,3,3);


		BYTE rgb[] =  {64,128,192};
		std::fill( subsubImage.begin(), subsubImage.end(), rgb );

		for(size_t i = 0; i < 10; ++i)
			for(size_t j = 0; j < 10; ++j)
			{
				if( i >= 8 && i < 10 && j >= 8 && j < 10 )
				{
					ASSERT_EQ( 64,  parentImage[i][j][0] );
					ASSERT_EQ( 128, parentImage[i][j][1] );
					ASSERT_EQ( 192, parentImage[i][j][2] );
				}
				else
				{
					ASSERT_EQ( 0,  parentImage[i][j][0] );
					ASSERT_EQ( 0,  parentImage[i][j][1] );
					ASSERT_EQ( 0,  parentImage[i][j][2] );
				}
			}
	}

#ifdef _RELEASE
	TEST(T_BmpIter, bmp_it_speed_test_rgb) 
	{
		BYTE rgb[] = { 32,64,128 };
		auto image (create_co_ownership_bmp_image( 4096, 1024, bmp_rgb24_no_padded_format() ));
		timer_on t;
		for(auto begin = image.begin(), end = image.end(); begin!=end; ++begin) *begin=rgb;
	}

	TEST(T_BmpIter, bmp_it_access_like_array_speed_test_rgb) 
	{
		BYTE rgb[] = { 32,64,128 };
		auto image (create_co_ownership_bmp_image( 4096, 1024, bmp_rgb24_no_padded_format() ));
		timer_on t;
		for(int i = 0; i < 1024; i++)
			for(int j = 0; j < 4096; j ++)
			{
				image(j, i) [ 0 ] = 128;
				image(j, i) [ 1 ] = 128;
				image(j, i) [ 2 ] = 128;
			}
	}

	TEST(T_BmpIter, bmp_simple_pointer_speed_test) 
	{
		std::vector<BYTE> array_(4096*1024*3);
		BYTE* pointer = &array_[0];
		timer_on t;
		for(int i = 0; i < 1024; i++)
			for(int j = 0, offsetY = (1023-i)*4096*3; j < 4096; j ++)
			{
				pointer[ offsetY + j * 3 ] = 128;
				pointer[ offsetY + j * 3 + 1] = 128;
				pointer[ offsetY + j * 3 + 2] = 128;
			}
	}
#endif
}