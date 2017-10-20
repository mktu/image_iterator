#pragma once
#include <iterator>
#include "SourceHolderBase.h"

namespace image_lib
{
	typedef ptrdiff_t coord_t;

	//---------------------------------------------------------------------------
	/*!
	 * @brief   this class used by "image iterator" for recording current position (x, y). 
	 * 
	 *   the current position is updated by "incremeter" which also image iterator has.
	 */
	//---------------------------------------------------------------------------
	struct counterable_2d
	{
		counterable_2d(const logical_coordinate_holder& source) 
			: x_( 0 ), y_( 0 )
			, offset_x_( source.superficial_coordinates_.offset_x_)
			, offset_y_( source.superficial_coordinates_.offset_y_)
			, width_( source.superficial_coordinates_.width_ )
			, underlying_width_( source.underlying_coordinates_.width_) {  }

		counterable_2d(coord_t x, coord_t y, const logical_coordinate_holder& source) 
			: x_( x ), y_( y )
			, offset_x_( source.superficial_coordinates_.offset_x_)
			, offset_y_( source.superficial_coordinates_.offset_y_)
			, width_( source.superficial_coordinates_.width_ )
			, underlying_width_( source.underlying_coordinates_.width_) {  }

		inline coord_t operator-(const counterable_2d& right) const
		{	
			return ((offset_y_+y_)*width_+(offset_x_+x_))
				-  ((right.offset_y_+right.y_)*right.width_+(right.offset_x_+right.x_));
		}

		inline bool operator>(const counterable_2d& right) const
		{
			return (offset_y_+y_) == (right.offset_y_ + right.y_)
				?  (offset_x_+x_) >  (right.offset_x_ + right.x_)
				:  (offset_y_+y_) >  (right.offset_y_ + right.y_);
		}

		inline bool operator<(const counterable_2d& right) const
		{
			return (offset_y_+y_) == (right.offset_y_ + right.y_)
				?  (offset_x_+x_) <  (right.offset_x_ + right.x_)
				:  (offset_y_+y_) <  (right.offset_y_ + right.y_);
		}

		inline bool operator==(const counterable_2d& right) const
		{
			return (offset_y_+y_) == (right.offset_y_ + right.y_)
				?  (offset_x_+x_) == (right.offset_x_ + right.x_) : false;
		}

		inline bool operator!=(const counterable_2d& right) const
		{
			return !(*this==right);
		}

		inline bool operator>=(const counterable_2d& right) const
		{
			return (offset_y_+y_) == (right.offset_y_ + right.y_)
				?  (offset_x_+x_) >= (right.offset_x_ + right.x_)
				:  (offset_y_+y_) >  (right.offset_y_ + right.y_);
		}

		inline bool operator<=(const counterable_2d& right) const
		{
			return (offset_y_+y_) == (right.offset_y_ + right.y_)
				?  (offset_x_+x_) <= (right.offset_x_ + right.x_)
				:  (offset_y_+y_) <  (right.offset_y_ + right.y_);
		}

		coord_t x_;
		coord_t y_;
		coord_t offset_x_;
		coord_t offset_y_;
		coord_t width_;
		coord_t underlying_width_;
	};

	struct col_incremeter_2d
	{
		col_incremeter_2d(const logical_coordinate_holder& source){ }

		void advance(coord_t d, counterable_2d& counterable)
		{
			counterable.y_+=d;
		}

		void disadvance(coord_t d, counterable_2d& counterable)
		{
			counterable.y_-=d;
		}

		void increment(counterable_2d& counterable)
		{
			++counterable.y_;
		}

		void decrement(counterable_2d& counterable)
		{
			--counterable.y_;
		}

		coord_t distance(const counterable_2d& l, const counterable_2d& r) const
		{
			return l.y_ > r.y_ ? l.y_ - r.y_ : r.y_ - l.y_;
		}
	};

	struct row_incremeter_2d
	{
		row_incremeter_2d(const logical_coordinate_holder& source) 
			: width_( source.superficial_coordinates_.width_ )
			, inverse_of_width_( 1.0 / source.superficial_coordinates_.width_ ) { }
		
		void advance(coord_t d, counterable_2d& counterable)
		{
			counterable.x_+=d;
			if(counterable.x_>=width_)
			{
				coord_t dd = static_cast<coord_t>(counterable.x_*inverse_of_width_);
				counterable.y_+=dd;
				counterable.x_-=dd*width_;
			}
		}

		void disadvance(coord_t d, counterable_2d& counterable)
		{
			counterable.x_-=d;
			if(counterable.x_<0)
			{
				coord_t dd = static_cast<coord_t>(counterable.x_*inverse_of_width_)-1;
				counterable.y_+=dd;
				counterable.x_-=dd*width_;
			}
		}

		void increment(counterable_2d& counterable)
		{
			if(++counterable.x_>=width_)
			{
				++counterable.y_;
				counterable.x_=0;
			}
		}

		void decrement(counterable_2d& counterable)
		{
			if(--counterable.x_<0)
			{
				--counterable.y_;
				counterable.x_=width_-1;
			}
		}

		coord_t distance(const counterable_2d& l, const counterable_2d& r) const
		{
			return l-r;
		}

	private:
		coord_t width_;
		double inverse_of_width_;
	};
}