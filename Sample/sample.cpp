#include <vector>
#include <algorithm>
#include "ImageAll.h"

using namespace image_lib;

void planar_image_sample()
{
    // 8x8 rgb planar image buffer
    // (0,0,0), (0,0,0), ...,(0,0,0)
    // ...
    // (0,0,0), (0,0,0), ...,(0,0,0)
    std::vector<unsigned char> source(8*8*3, 0); 

    // create image container
    auto image = create_planar_image( &source[0], 8, 8 );
    unsigned char rgb[3] = { 128,128,128 };
    // fill image buffer with (r,g,b,)=(128,128,128)
    // (128,128,128), (128,128,128), ...,(128,128,128)
    // ...
    // (128,128,128), (128,128,128), ...,(128,128,128)   
    std::fill(image.begin(),image.end(),rgb);
    
    // fill the first row with (r,g,b,)=(64,64,64)
    // (64,64,64), (64,64,64), ..., (64,64,64)
    // (128,128,128), (128,128,128), ...,(128,128,128) 
    // ...
    // (128,128,128), (128,128,128), ...,(128,128,128) 
    rgb[0]=64;rgb[1]=64;rgb[2]=64;
    std::fill(image.row_begin(0),image.row_end(0),rgb);

    // fill the last col with (r,g,b,)=(32,32,32)
    // (64,64,64), (64,64,64), ..., (32,32,32)
    // (128,128,128), (128,128,128), ...,(32,32,32) 
    // ...
    // (128,128,128), (128,128,128), ...,(32,32,32)
    rgb[0]=32;rgb[1]=32;rgb[2]=32;
    std::fill(image.col_begin(7),image.col_end(7),rgb);

    // fill the range with (r,g,b,)=(16,16,16)
    // (64,64,64), (64,64,64), ..., (16,16,16), (32,32,32)
    // (16,16,16), (16,16,16), ..., (16,16,16), (32,32,32) 
    // ...
    // (16,16,16), (16,16,16), ..., (16,16,16), (32,32,32)
    for(size_t i = 1; i < 8; i++)
			for(size_t j = 0; j < 7; j++)
			{
                image[i][j][0] = 16;
                image[i][j][1] = 16;
                image[i][j][2] = 16;
			}

    // fill the range with (r,g,b,)=(255,255,255)
    // (64,64,64), (64,64,64), ..., (16,16,16), (32,32,32)
    // (16,16,16), (255,255,255), ..., (255,255,255), (32,32,32) 
    // ...
    // (16,16,16), (255,255,255), ..., (255,255,255), (32,32,32)
    rgb[0]=255;rgb[1]=255;rgb[2]=255;
    auto subImage = sub_image( image, 6, 6, 1, 1 );
    std::fill(subImage.begin(),subImage.end(),rgb);
 
}

void gray_image_sample()
{
    // 8x8 gray scale image buffer
    std::vector<unsigned char> source(8*8, 0);

    // create image container
    auto image = create_gray_image( &source[0], 8, 8 );

    std::fill(image.begin(),image.end(),128);

    std::fill(image.row_begin(0),image.row_end(0),64);

    std::fill(image.col_begin(7),image.col_end(7),32);

    for(size_t i = 1; i < 8; i++)
			for(size_t j = 0; j < 7; j++)
			{
                image[i][j] = 16;
			}
    auto subImage = sub_image( image, 6, 6, 1, 1 );
    std::fill(subImage.begin(),subImage.end(),255);
}

void bit_image_sample()
{
    // 8x8 bit image buffer
    std::vector<unsigned char> source(8*8/8, 0);

    // create image container
    auto image = create_bit_image( &source[0], 8, 8 );

    std::fill(image.begin(),image.end(),true);

    std::fill(image.row_begin(0),image.row_end(0),false);

    std::fill(image.col_begin(7),image.col_end(7),true);

    for(size_t i = 1; i < 8; i++)
			for(size_t j = 0; j < 7; j++)
			{
                image[i][j] = false;
			}
    auto subImage = sub_image( image, 6, 6, 1, 1 );
    std::fill(subImage.begin(),subImage.end(),true);
}

int main()
{
    planar_image_sample();
    gray_image_sample();
    bit_image_sample();
    return 0;
}