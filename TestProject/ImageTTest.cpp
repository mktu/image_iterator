#include <gtest/gtest.h>
#include <Windows.h>
#include <algorithm>
#include "../ImageLib/ImageAll.h"

namespace
{
	using  namespace image_lib;
	typedef std::vector<BYTE> ByteArray;

	TEST(ImageTTest, const_image_test1)
	{
		ByteArray array1(3*3*3);
		std::fill( array1.begin(), array1.end(), 0);

		ByteArray array2(array1.size());
		for(size_t i = 0; i < 3; i++)
			for(size_t j = 0; j < 3; j++)
			{
				array2[i*3+3*3*0+j] = 32;
				array2[i*3+3*3*1+j] = 64;
				array2[i*3+3*3*2+j] = 128;
			}
		BYTE* p = &array2[0];

		non_ownership_rgb24_planar_image image ( create_non_ownership_image( &array1[0], 3, 3, rgb8u_planar_format() ) );
		non_ownership_rgb24c_planar_image image2 ( create_non_ownership_image( p, 3, 3, rgb8uc_planar_format() ) );

		std::copy( image2.cbegin(), image2.cend(), image.begin() );
		
		for(size_t i = 0; i < 3; i++)
			for(size_t j = 0; j < 3; j++)
			{
				ASSERT_EQ( 32, array1[i*3+3*3*0+j] );
				ASSERT_EQ( 64, array1[i*3+3*3*1+j] );
				ASSERT_EQ( 128, array1[i*3+3*3*2+j] );
			}
	}

	TEST(ImageTTest, const_image_test2)
	{
		co_ownership_gray8u_image psDstGray (create_co_ownership_image(10,10,gray8u_format()));
		
		std::for_each(psDstGray.begin(), psDstGray.end(), [](co_ownership_gray8u_image::reference& b){ b = 128; });

		const co_ownership_gray8u_image psDstGray2 = psDstGray;

		ASSERT_TRUE( std::all_of( psDstGray2.cbegin(), psDstGray2.cend(), 
			[](const co_ownership_gray8u_image::const_reference& b)->bool{ return b == 128;} ) );
	}

	TEST(ImageTTest, const_image_test3)
	{
		co_ownership_gray8u_image psDstGray( create_co_ownership_image(10,10,gray8u_format()) );
		
		std::for_each(psDstGray.begin(), psDstGray.end(), [](co_ownership_gray8u_image::reference& b){ b = 128; });

		const co_ownership_gray8u_image psDstGray2 = psDstGray;
		const co_ownership_gray8u_image psDstGray3 (create_co_ownership_image(10,10,gray8u_format()) );

		ASSERT_TRUE( std::all_of( psDstGray2.cbegin(), psDstGray2.cend(), 
			[](const co_ownership_gray8u_image::const_reference& b)->bool{ return b == 128;} ) );
	}

	TEST(ImageTTest, 2d_access_test)
	{
		co_ownership_rgb24_planar_image psDstRGB ( create_co_ownership_image(10,10,rgb8u_planar_format()) );
		{
			BYTE rgb[3] = { 128,128,128 };
			co_ownership_rgb24_planar_image psRGB( create_co_ownership_image(10,10,rgb8u_planar_format()) );
			std::fill(psRGB.begin(), psRGB.end(), rgb);
			const co_ownership_rgb24_planar_image & refRGB = psRGB;
			
			for(size_t i = 0; i < 10; i++)
				for(size_t j = 0; j < 10; j++)
					for(size_t k = 0; k < 3; k++)
					{
						psDstRGB[i][j][k] = refRGB[i][j][k];
					}
		}
		ASSERT_TRUE( std::all_of( psDstRGB.begin(), psDstRGB.end(), 
			[](const co_ownership_rgb24_planar_image::reference& b)->bool{ return b[0] == 128 && b[1] == 128 && b[2] ==128;} ) );
	}

	TEST(ImageTTest, sub_image_test1)
	{
		co_ownership_rgb24_planar_image parent (create_co_ownership_image(10,10,rgb8u_planar_format()));
		auto subImage = sub_image( parent, 4, 2, 1, 6 );
		ASSERT_EQ(4, subImage.width() );
		ASSERT_EQ(2, subImage.height() );
	}

	TEST(ImageTTest, sub_image_test2)
	{
		co_ownership_rgb24_planar_image parent (create_co_ownership_image(50,50,rgb8u_planar_format()));
		auto subImage = sub_image( sub_image( parent, 10, 10, 20, 20 ), 5, 5, 5, 5 );
		ASSERT_EQ(5, subImage.width() );
		ASSERT_EQ(5, subImage.height() );
	}
}