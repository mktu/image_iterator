#pragma once
#include <iterator>
#include "SourceHolderBase.h"

namespace image_lib
{

	template<typename Pointer, typename Locator>
	class bit_pixel_iterator_data
	{
	public:
		
		typedef Pointer pointer;
		typedef typename std::iterator_traits<pointer>::difference_type difference_type;
		typedef Locator locator;
		typedef typename locator::counter_type counter_type;

		template<typename Source>
		bit_pixel_iterator_data(const Source& src) 
			: p_(static_cast<pointer>(src.p_)), l_( src ), pos_(src){ }

		template<typename Source>
		bit_pixel_iterator_data(const Source& src, const counter_type& offset) 
			: p_(static_cast<pointer>(src.p_)), l_(src), pos_( offset ) { }

		pointer p_;
		locator l_;
		counter_type pos_;
	};

	template<typename Pointer, typename Locator>
	struct bit_pixel_iterator_const_reference 
	{
		typedef Locator locator;
		typedef bit_pixel_iterator_data<Pointer,Locator> data_type;
		typedef bool value_type;

		operator value_type() const 
		{
			return data_.l_.get_bit(data_.p_, data_.pos_);
		}

		bit_pixel_iterator_const_reference( const data_type& base ) : data_( base ) { } 

	private:
		bit_pixel_iterator_const_reference& operator=(const bit_pixel_iterator_const_reference&);
	protected:
		data_type data_;
	};

	template<typename Pointer, typename Locator>
	struct bit_pixel_iterator_reference : public bit_pixel_iterator_const_reference<Pointer, Locator>
	{
		typedef bit_pixel_iterator_const_reference<Pointer, Locator> base_type;
		typedef typename base_type::data_type data_type;
		typedef typename base_type::value_type value_type;

		bit_pixel_iterator_reference& operator=(value_type right)
		{
			data_.l_.set_bit(data_.p_, data_.pos_, right);
			return *this;
		}

		bit_pixel_iterator_reference& operator=(const bit_pixel_iterator_reference& right)
		{
			return this->operator=(static_cast<value_type>(right));
		}

		bit_pixel_iterator_reference( const data_type& base ) : base_type( base ) { } 
	};

	template<typename Pointer, typename Locator>
	struct bit_pixel_iterator_value : public bit_pixel_iterator_reference<Pointer, Locator>
	{
		typedef source_holder_base<Pointer> holder_base; 
		typedef bit_pixel_iterator_reference<Pointer, Locator> base_type;
		typedef typename base_type::value_type value_type;

		bit_pixel_iterator_value(const value_type&  right) : base_type( holder_base(values_, 1, 1) )
		{
			this->base_type::operator=(right);
		}

	private:
		typename std::iterator_traits<Pointer>::value_type values_[1];
	};

	template<typename Poniter1, typename Pointer2, typename Locator>
	inline bool operator==(
		const bit_pixel_iterator_const_reference<Poniter1,Locator>& lhs,
		const bit_pixel_iterator_const_reference<Pointer2,Locator>& rhs )
	{
		return static_cast<typename bit_pixel_iterator_const_reference<Poniter1,Locator>::value_type>(lhs) ==
			   static_cast<typename bit_pixel_iterator_const_reference<Pointer2,Locator>::value_type>(rhs);
	}

	template<typename Poniter1, typename Pointer2, typename Locator>
	inline bool operator!=(
		const bit_pixel_iterator_const_reference<Poniter1,Locator>& lhs,
		const bit_pixel_iterator_const_reference<Pointer2,Locator>& rhs )
	{
		return !(lhs==rhs);
	}
}