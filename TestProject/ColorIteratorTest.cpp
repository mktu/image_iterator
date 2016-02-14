#include <gtest/gtest.h>
#include <Windows.h>
#include <algorithm>
#include "../ImageLib/ImageAll.h"

#include "TestTimer.h"

namespace
{
	using  namespace image_lib;
	typedef std::vector<BYTE> ByteArray;

	class T_Iter :  public ::testing::Test
	{
	protected:
		ByteArray array_;
		T_Iter() : array_(3*3*3, 0) { }

	};

	// ピクセル妥当性テスト
	TEST_F(T_Iter, pixel_test)
	{
		unsigned char values[] = {10,11,12};
		unsigned char values2[] = {9,11,12};
		non_ownership_color24u_planar_image::value_type px(values);
		non_ownership_color24u_planar_image::const_reference ref(px);
		non_ownership_color24u_planar_image::value_type px2(ref);
		non_ownership_color24u_planar_image::value_type px3(values2);
		non_ownership_color24u_planar_image::value_type px4(px);

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
	TEST_F(T_Iter, packed_size_test)
	{
		typedef packed_format_t<3,long> long_based_format;
		auto image = create_co_ownership_image( 10,10,long_based_format() );
		ASSERT_EQ( 300, image_size(image) );
		ASSERT_EQ( sizeof(long) * 300, image_size_in_byte(image) );
		ASSERT_EQ( 300, array_size_of_image(image) );
	}

	// サイズ妥当性テスト
	TEST_F(T_Iter, planar_size_test)
	{
		typedef planar_format_t<3,long> long_based_format;
		auto image = create_co_ownership_image( 10,10,long_based_format() );
		ASSERT_EQ( 300, image_size(image) );
		ASSERT_EQ( sizeof(long) * 300, image_size_in_byte(image) );
		ASSERT_EQ( 300, array_size_of_image(image) );
	}

// イテレータテスト
	// iteratorを使ってPlanar画像を埋め尽くす
	TEST_F(T_Iter, fill_img_planar_iter) 
	{
		BYTE rgb[3] = { 128,128,128 };
		auto image ( create_planar_image( &array_[0], 3, 3 ) );
		std::fill( image.begin(), image.end(), rgb );
		ASSERT_TRUE( std::all_of( array_.begin(), array_.end(), [](BYTE b){return b == 128;} ) );
	}

	// reverse iteratorを使ってPlanar画像を埋め尽くす
	TEST_F(T_Iter, fill_img_planar_reverse_iter) 
	{
		BYTE rgb[3] = { 128,128,128 };
		auto image (create_planar_image( &array_[0], 3, 3 ));
		std::fill( image.rbegin(), image.rend(), rgb );
		ASSERT_TRUE( std::all_of( array_.begin(), array_.end(), [](BYTE b){return b == 128;} ) );
	}

	// iteratorを使ってPacked画像を埋め尽くす
	TEST_F(T_Iter, fill_img_packed_iter_test) 
	{
		BYTE rgb[3] = { 32,64,128 };
		auto image( create_packed_image( &array_[0], 3, 3 ) );
		std::fill( image.begin(), image.end(), rgb );
		for( size_t i = 0; i < array_.size(); i+=3 )
		{
			ASSERT_EQ( 32, array_[i] );
			ASSERT_EQ( 64, array_[i+1] );
			ASSERT_EQ( 128, array_[i+2] );
		}
	}

	// reverse iteratorを使ってPacked画像を埋め尽くす
	TEST_F(T_Iter, fill_img_packed_reverse_iter) 
	{
		BYTE rgb[3] = { 32,64,128 };
		auto image ( create_packed_image( &array_[0], 3, 3 ) );
		std::fill( image.rbegin(), image.rend(), rgb );
		for( size_t i = 0; i < array_.size(); i+=3 )
		{
			ASSERT_EQ( 32, array_[i] );
			ASSERT_EQ( 64, array_[i+1] );
			ASSERT_EQ( 128, array_[i+2] );
		}
	}

// パラメータ化テスト
	// XIteratorテスト
	class PT_Row_Iter : public ::testing::TestWithParam<size_t>
	{	
	public:
		enum{ size_x = 6, size_y = 3 };
	protected:
		ByteArray array_;

		PT_Row_Iter() : array_(size_x*size_y*3, 0) { }
	};

	// 指定された行に対して値を設定する(Planar)
	TEST_P(PT_Row_Iter, fill_planar_row) 
	{
		BYTE rgb[3] = { 32,64,128 };
		auto image( create_planar_image( &array_[0], size_x, size_y ) );
		const size_t line = GetParam();
		std::fill( image.row_begin(line), image.row_end(line), rgb );
		
		auto start_iterator_r = array_.begin()+size_x*line;
		auto start_iterator_g = start_iterator_r + size_x * size_y;
		auto start_iterator_b = start_iterator_g + size_x * size_y;

		ASSERT_TRUE( std::all_of( start_iterator_r,	start_iterator_r+size_x, [](BYTE b){return b == 32;} ) );
		ASSERT_TRUE( std::all_of( start_iterator_g, start_iterator_g+size_x, [](BYTE b){return b == 64;} ) );
		ASSERT_TRUE( std::all_of( start_iterator_b, start_iterator_b+size_x, [](BYTE b){return b == 128;} ) );
	}

	// 指定された行に対して値を設定する(Packed)
	TEST_P(PT_Row_Iter, fill_packed_row) 
	{
		BYTE rgb[3] = { 32,64,128 };
		auto image ( create_packed_image( &array_[0], size_x, size_y ) );
		const size_t line = GetParam();
		std::fill( image.row_begin(line), image.row_end(line), rgb );
		
		auto start_iterator_r = array_.begin()+size_x*3*line;
		auto start_iterator_g = start_iterator_r + 1;
		auto start_iterator_b = start_iterator_g + 1;

		for(size_t i = 0; i < size_x; ++i)
		{
			ASSERT_EQ( 32,  *(start_iterator_r + i * 3) );
			ASSERT_EQ( 64,  *(start_iterator_g + i * 3) );
			ASSERT_EQ( 128, *(start_iterator_b + i * 3) );
		}
	}

INSTANTIATE_TEST_CASE_P(Iter_Category,
                        PT_Row_Iter,
                        ::testing::Values((size_t)0, (size_t)PT_Row_Iter::size_y-1));

	// YIteratorテスト
	class PT_Col_Iterator : public ::testing::TestWithParam<size_t>
	{	
	public:
		enum{ size_x = 6, size_y = 3 };
	protected:
		ByteArray array_;

		PT_Col_Iterator() : array_(size_x*size_y*3, 0) { }
	};

	// 指定された列に対して値を設定する(Planar)
	TEST_P(PT_Col_Iterator, fill_planar_col) 
	{
		BYTE rgb[3] = { 32,64,128 };
		auto image( create_planar_image( &array_[0], size_x, size_y ) );
		const size_t col = GetParam();
		std::fill( image.col_begin(col), image.col_end(col), rgb );
		
		auto start_iterator_r = array_.begin()+col;
		auto start_iterator_g = start_iterator_r + size_x * size_y;
		auto start_iterator_b = start_iterator_g + size_x * size_y;

		for(size_t i = 0; i < size_y; ++i)
		{
			ASSERT_EQ( 32,  *(start_iterator_r + size_x * i) );
			ASSERT_EQ( 64,  *(start_iterator_g + size_x * i) );
			ASSERT_EQ( 128, *(start_iterator_b + size_x * i) );
		}
	}

	// 指定された列に対して値を設定する(Packed)
	TEST_P(PT_Col_Iterator, fill_packed_col) 
	{
		BYTE rgb[3] = { 32,64,128 };
		auto image (create_packed_image( &array_[0], size_x, size_y ));
		std::fill( image.y_at(1,1), image.col_end(1), rgb );
		const size_t col = GetParam();
		std::fill( image.col_begin(col), image.col_end(col), rgb );
		
		auto start_iterator_r = array_.begin()+col * 3;
		auto start_iterator_g = start_iterator_r + 1;
		auto start_iterator_b = start_iterator_g + 1;

		for(size_t i = 0; i < size_y; ++i)
		{
			ASSERT_EQ( 32,  *(start_iterator_r + size_x * i * 3) );
			ASSERT_EQ( 64,  *(start_iterator_g + size_x * i * 3) );
			ASSERT_EQ( 128, *(start_iterator_b + size_x * i * 3) );
		}
	}

INSTANTIATE_TEST_CASE_P(Iter_Category,
                        PT_Col_Iterator,
                        ::testing::Values((size_t)0, (size_t)PT_Col_Iterator::size_x-1));


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

	class PT_Iter_SubImage : public ::testing::TestWithParam<SubImageCoord>
	{		};


	// SubImageを埋め尽くす(Planar)
	TEST_P(PT_Iter_SubImage, planar_sub_img)
	{
		auto parentImage( create_co_ownership_image(10,10,color24u_planar_format()));
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

	// SubImageを埋め尽くす(Packed)
	TEST_P(PT_Iter_SubImage, packed_sub_img)
	{
		auto parentImage( create_co_ownership_image(10,10,color24u_format()) );
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

INSTANTIATE_TEST_CASE_P(Iter_Category,
                        PT_Iter_SubImage,
                        ::testing::Values(SubImageCoord(0,0,5,5), 
										  SubImageCoord(5,0,5,5),
										  SubImageCoord(0,5,5,5),
										  SubImageCoord(5,5,5,5),
										  SubImageCoord(0,0,1,1),
										  SubImageCoord(0,0,10,10)) );

	// NestしたSubImageを埋め尽くす
	TEST_F(T_Iter, nested_sub_img)
	{
		auto parentImage( create_co_ownership_image(10,10,color24u_planar_format()) );
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

	TEST_F(T_Iter, simple_pointer_speed_test) 
	{
		array_.resize(4096*1024*3);
		BYTE* pointer = &array_[0];
		timer_on t;
		for(int i = 0; i < 1024; i++)
			for(int j = 0; j < 4096; j ++)
			{
				pointer[ i * 4096 * 3 + j * 3 + 0] = 128;
				pointer[ i * 4096 * 3 + j * 3 + 1] = 128;
				pointer[ i * 4096 * 3 + j * 3 + 2] = 128;
			}
	}

	template<class _FwdIt,
	class _Ty> inline
	void fill(_FwdIt _First, _FwdIt _Last, const _Ty& _Val)
	{	// copy _Val through [_First, _Last)
	for (; _First != _Last; ++_First)
		*_First = _Val;
	}


	TEST_F(T_Iter, img_t_speed_test_with_subimage) 
	{
		BYTE rgb[3] = { 32,64,128 };
		//array_.resize(4096*1024*3);
		auto image (create_co_ownership_image( 4096, 1024, color24u_format() ));
		auto subimage = sub_image(image, 4096, 1024, 0, 0);
		timer_on t;
		fill( subimage.begin(), subimage.end(), rgb );
		
	}

	TEST_F(T_Iter, img_t_speed_test_with_array_like_access) 
	{
		BYTE rgb[3] = { 32,64,128 };
		array_.resize(4096*1024*3);
		auto image( create_planar_image( &array_[0], 4096, 1024 ) );
		timer_on t;
		for(int i = 0; i < 1024; i++)
			for(int j = 0; j < 4096; j ++)
			{
				image(j,i) = rgb;
			}
		
	}

#endif
}