#pragma once
#include "SourceHolderBase.h"

namespace image_lib
{
	template<
		typename Incremeter, 
		typename ReferenceType, 
		typename ValueType
	>
	class basic_pixel_iterator 
	{
	public:
		typedef typename ReferenceType::data_type data_type;
		typedef typename data_type::pointer pointer;
		typedef typename std::iterator_traits<pointer> iterator_traits;
		typedef basic_pixel_iterator my_iter;
		typedef Incremeter incremeter;
		typedef ReferenceType reference; 
		typedef ValueType value_type;

		typedef std::random_access_iterator_tag iterator_category;
		typedef typename iterator_traits::difference_type difference_type,distance_type;
		typedef pointer _Unchecked_type;
		typedef size_t size_type;
			
	private:
		incremeter inc_;
		data_type data_;

	public:

		template<typename SourceType>
		basic_pixel_iterator(const SourceType& src )
			: inc_( src ), data_(src){  }

		template<typename SourceType>
		basic_pixel_iterator(const SourceType& src, difference_type x, difference_type y)
			: inc_( src ), data_(src,typename data_type::counter_type(x,y,src)){  }

		reference operator*() const
		{	// return designated object
			return reference(data_);
		}

		my_iter& operator++()
		{	// preincrement
			inc_.increment(data_.pos_);
			return (*this);
		}

		my_iter operator++(int)
		{	// postincrement
			my_iter tmp = *this;
			++*this;
			return (tmp);
		}

		my_iter& operator--()
		{	// predecrement
			inc_.decrement(data_.pos_);
			return (*this);
		}

		my_iter operator--(int)
		{	// postdecrement
			my_iter tmp = *this;
			--*this;
			return (tmp);
		}

		my_iter& operator+=(difference_type off)
		{	// increment by integer
			inc_.advance(off,data_.pos_);
			return (*this);
		}

		const my_iter operator+(difference_type off) const
		{	// return this + integer
			my_iter tmp = *this;
			return (tmp += off);
		}

		my_iter& operator-=(difference_type off)
		{	// decrement by integer
			inc_.disadvance(off,data_.pos_);
			return (*this);
		}

		my_iter operator-(difference_type off) const
		{	// return this - integer
			my_iter tmp = *this;
			return (tmp -= off);
		}

		difference_type operator-(const my_iter& right) const
		{	// return difference of iterators
			return (this->data_.pos_-right.data_.pos_); 
		}

		reference operator[](difference_type _Off) const
		{	// subscript
			return (*(*this + _Off));
		}

		bool operator>(const my_iter& right) const
		{
			return this->data_.pos_ > right.data_.pos_;
		}

		bool operator<(const my_iter& right) const
		{
			return this->data_.pos_ < right.data_.pos_;
		}

		bool operator==(const my_iter& right) const
		{
			return this->data_.pos_ == right.data_.pos_;
		}

		bool operator!=(const my_iter& right) const
		{
			return !(this->data_.pos_ == right.data_.pos_);
		}

		bool operator>=(const my_iter& right) const
		{
			return this->data_.pos_ >= right.data_.pos_;
		}

		bool operator<=(const my_iter& right) const
		{
			return this->data_.pos_ <= right.data_.pos_;
		}
	};
}