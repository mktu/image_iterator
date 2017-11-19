#include <gtest/gtest.h>
#include <Windows.h>
#include <algorithm>
#include "../ImageLib/ImageAll.h"

namespace
{
	using  namespace image_lib;
	typedef std::vector<BYTE> ByteArray;

	// ----------------------------------------
	// テストケース
	// 0	1	2	3	4	5	6	7	8	9
	// 10	11	12	13	14	15	16	17	18	19
	// 20	21	22	23	24	25	26	27	28	29
	// 30	31	32	33	34	35	36	37	38	39
	// 40	41	42	43	44	45	46	47	48	49
	// 50	51	52	53	54	55	56	57	58	59
	// 60	61	62	63	64	65	66	67	68	69
	// 70	71	72	73	74	75	76	77	78	79
	// 80	81	82	83	84	85	86	87	88	89
	// 90	91	92	93	94	95	96	97	98	99
	// ----------------------------------------

	co_ownership_gray8u_image obtain_incremental_test_data()
	{
		auto psDstGray( create_co_ownership_image(10,10, gray8u_format()) );
		unsigned char i = 0;
		std::generate(psDstGray.begin(), psDstGray.end(), [&i](){
			return i++;
		});
		return psDstGray;
	}

	// 通常のインクリメンタルは他テストで実施

	// 定量インクリメントテスト

	TEST(T_BasicIter, iter_step_inc_row_test)
	{
		co_ownership_gray8u_image psDstGray = obtain_incremental_test_data();
		auto iter = psDstGray.begin();
		auto end = psDstGray.end();
		unsigned char i = 0;
		for(;iter!=end;iter+=10) 
		{
			ASSERT_EQ( i, *iter );
			i += 10;
		}
	}

	TEST(T_BasicIter, iter_step_inc_col_test)
	{
		co_ownership_gray8u_image psDstGray = obtain_incremental_test_data();
		auto iter = psDstGray.col_begin(0);
		auto end = psDstGray.col_end(0);
		unsigned char i = 0;
		for(;iter!=end;iter+=2) 
		{
			ASSERT_EQ( i, *iter );
			i += 20;
		}
	}

	// デクリメントテスト

	TEST(T_BasicIter, iter_dec_row_test)
	{
		co_ownership_gray8u_image psDstGray = obtain_incremental_test_data();
		auto iter = psDstGray.rbegin();
		unsigned char i = 100;
		for(;iter!=psDstGray.rend();++iter) ASSERT_EQ( --i, *iter );
	}

	TEST(T_BasicIter, iter_dec_col_test)
	{
		co_ownership_gray8u_image psDstGray = obtain_incremental_test_data();
		auto iter = std::reverse_iterator<co_ownership_gray8u_image::y_iterator>( psDstGray.col_end(0) );
		auto end = iter + 10;
		unsigned char i = 90;
		for(;iter!=end;++iter) 
		{
			ASSERT_EQ( i, *iter );
			i -= 10;
		}
	}

	// 定量デクリメントテスト

	TEST(T_BasicIter, iter_step_dec_row_test)
	{
		co_ownership_gray8u_image psDstGray = obtain_incremental_test_data();
		auto iter = psDstGray.rbegin();
		auto end = psDstGray.rend();
		unsigned char i = 99;
		for(;iter!=end;iter+=10) 
		{
			ASSERT_EQ( i, *iter );
			i -= 10;
		}
	}

	TEST(T_BasicIter, iter_step_dec_col_test)
	{
		co_ownership_gray8u_image psDstGray = obtain_incremental_test_data();
		auto iter = std::reverse_iterator<co_ownership_gray8u_image::y_iterator>( psDstGray.col_end(0) );
		auto end = iter + 10;
		unsigned char i = 90;
		for(;iter!=end;iter+=2) 
		{
			ASSERT_EQ( i, *iter );
			i -= 20;
		}
	}

	// 大小比較

	TEST(T_BasicIter, iter_comp_test1)
	{
		co_ownership_gray8u_image psDstGray = obtain_incremental_test_data();
		auto iter = psDstGray.begin()+20;
		auto iter2 = psDstGray.row_begin(5);
		ASSERT_TRUE( iter < iter2 );
		ASSERT_FALSE( iter >= iter2 );
		ASSERT_TRUE( iter != iter2 );
	}

	TEST(T_BasicIter, iter_comp_test2)
	{
		co_ownership_gray8u_image psDstGray = obtain_incremental_test_data();
		auto iter = psDstGray.begin()+20;
		auto iter2 = psDstGray.row_begin(2);
		ASSERT_FALSE( iter < iter2 );
		ASSERT_FALSE( iter > iter2 );
		ASSERT_TRUE( iter >= iter2 );
		ASSERT_TRUE( iter <= iter2 );
		ASSERT_TRUE( iter == iter2 );
	}

	// Sub-Image
	// --------
	// 63	64
	// 73	74
	// 83	84
	// 93	94
	// --------

	TEST(T_BasicIter, iter_comp_test3)
	{
		co_ownership_gray8u_image psDstGray = obtain_incremental_test_data();
		co_ownership_gray8u_image subImage = sub_image(	obtain_incremental_test_data(), 2, 3, 3, 6 );
		auto iter = psDstGray.begin()+20;
		auto iter2 = subImage.begin();
		ASSERT_TRUE( iter < iter2 );
		ASSERT_FALSE( iter >= iter2 );
		ASSERT_TRUE( iter != iter2 );
	}

	TEST(T_BasicIter, iter_comp_test4)
	{
		co_ownership_gray8u_image psDstGray = obtain_incremental_test_data();
		co_ownership_gray8u_image subImage = sub_image( obtain_incremental_test_data(), 2, 3, 3, 6 );
		auto iter = psDstGray.begin()+73;
		auto iter2 = subImage.begin()+2;
		ASSERT_FALSE( iter < iter2 );
		ASSERT_FALSE( iter > iter2 );
		ASSERT_TRUE( iter >= iter2 );
		ASSERT_TRUE( iter <= iter2 );
		ASSERT_TRUE( iter == iter2 );
	}

	// 距離テスト

	TEST(T_BasicIter, iter_distance_test)
	{
		co_ownership_gray8u_image psDstGray = obtain_incremental_test_data();
		co_ownership_gray8u_image subImage = sub_image( obtain_incremental_test_data(), 2, 3, 3, 6 );
		auto iter = subImage.begin();
		auto iter2 = subImage.row_begin(2);
		ASSERT_EQ(-4,iter-iter2);
		ASSERT_EQ(4,iter2-iter);
	}
	
}