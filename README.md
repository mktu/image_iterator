# image_iterator

An iterator for image buffer such as rgb image.  

## Usage

1. Include image iterator header.
```c++
#include "ImageAll.h"
```

2. Prepare a array or vector to store image data. 
Then wrap the buffer by image container such like below.

```c++
// 8x8 rgb planar image buffer
std::vector<unsigned char> source(8*8*3, 0); 

...(assign an image to the buffer)

// create image container
auto image = create_planar_image( &source[0], 8, 8 );

```
Available image container types are:

|                     | data format                       | creater function    |
|:-----------:        |:------------------:               |:------------:       |
| packed color        | 24 bit : (r,g,b)...(r,g,b)        | create_pakced_image |
| planar color        | 24 bit : (r...),(g...),(b...)     | create_planar_image |
| packed color(32bit) | 32 bit : (r,g,b,a)...(r,g,b,a)    | create_pakced_image_32 |
| planar color(32bit) | 32 bit : (r...),(g...),(b...),(a...) | create_planar_image_32 |
| gray scale          | 8 bit                             | create_gray_image  |
| monochrome          | 1 bit                             | create_bit_image |

3. Access the image like an std container.

```c++
unsigned char rgb[3] = { 128,128,128 }; // rgb[0] is r, rgb[2] is b

std::fill(image.begin(),image.end(),rgb); // fill the entire image with (128,128,128)

std::fill(image.row_begin(0),image.row_end(0),rgb); // fill the first row of the image with (128,128,128)

std::fill(image.col_begin(0),image.col_end(0),rgb); // fill the first col of the image witth (128,128,128)

// array like access is available too.
for(size_t i = 1; i < 8; i++)
			for(size_t j = 0; j < 7; j++)
			{
                image[i][j][0] = 16; // r
                image[i][j][1] = 16; // g
                image[i][j][2] = 16; // b
			}
```

If you want to access the part of image, sub_image function is helpfull.

```c++
// subimage : x-offset:1, y-offset:1, width:6, height:6
auto subImage = sub_image( image, 6, 6, 1, 1 );
std::fill(subImage.begin(),subImage.end(),rgb);
```
