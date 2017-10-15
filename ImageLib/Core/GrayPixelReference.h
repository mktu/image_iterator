#pragma once
#include <iterator>
#include "SourceHolderBase.h"

namespace image_lib
{
	template<typename Pointer, typename Locator>
	class gray_pixel_iterator_data
	{
	public:

		typedef Pointer pointer;
		typedef Locator locator;
		typedef typename locator::counter_type counter_type;

		template<typename Source>
		gray_pixel_iterator_data(const Source& src) 
			: p_(static_cast<pointer>(src.p_)), l_( src ), pos_(src){ }
		template<typename Source>
		gray_pixel_iterator_data(const Source& src, const counter_type& offset) 
			: p_(static_cast<pointer>(src.p_)), l_(src), pos_( offset ) { }

		pointer p_;
		locator l_;
		counter_type pos_;
	};

	template<typename Pointer, typename Locator>
	struct gray_pixel_iterator_const_reference
	{
		typedef Locator locator;
		typedef Pointer pointer;
		typedef gray_pixel_iterator_data<pointer, locator> data_type;
		typedef typename std::iterator_traits<pointer> iterator_traits;
		typedef typename iterator_traits::reference reference;
		typedef typename iterator_traits::value_type value_type;
		typedef typename iterator_traits::distance_type distance_type, difference_type;

		operator value_type() const 
		{
			return *(data_.p_+data_.l_(data_.pos_));
		}

		gray_pixel_iterator_const_reference( const data_type& base ) : data_( base ) { } 

	private:
		gray_pixel_iterator_const_reference& operator=(const gray_pixel_iterator_const_reference&);
	protected:
		data_type data_;
	};


	template<typename Pointer, typename Locator>
	struct gray_pixel_iterator_reference : public gray_pixel_iterator_const_reference<Pointer, Locator>
	{
		typedef gray_pixel_iterator_const_reference<Pointer, Locator> base_type;
		typedef typename base_type::data_type data_type;
		typedef typename base_type::reference reference;
		typedef typename base_type::value_type value_type;
		typedef typename base_type::distance_type distance_type, difference_type;

		gray_pixel_iterator_reference& operator=(const value_type&  right)
		{
			*(base_type::data_.p_+base_type::data_.l_(base_type::data_.pos_)) = right;
			return *this;
		}

		gray_pixel_iterator_reference& operator=(const gray_pixel_iterator_reference& right)
		{
			*(base_type::data_.p_+base_type::data_.l_(base_type::data_.pos_)) = static_cast<value_type>(right);
			return *this;
		}
		gray_pixel_iterator_reference( const data_type& base ) : base_type( base ) { } 
	};

	template<typename Pointer, typename Locator>
	struct gray_pixel_iterator_value : public gray_pixel_iterator_reference<Pointer, Locator>
	{
		typedef source_holder_base<Pointer> holder_base; 
		typedef gray_pixel_iterator_reference<Pointer, Locator> base_type;
		typedef typename base_type::value_type value_type;

		gray_pixel_iterator_value(const value_type&  right) : base_type( holder_base(values_, 1, 1) )
		{
			values_[0]=right;
		}

	private:
		value_type values_[1];
	};

	template<typename Poniter1, typename Pointer2, typename Locator>
	inline bool operator==(
		const gray_pixel_iterator_const_reference<Poniter1,Locator>& lhs,
		const gray_pixel_iterator_const_reference<Pointer2,Locator>& rhs )
	{
		return static_cast<typename gray_pixel_iterator_const_reference<Poniter1,Locator>::value_type>(lhs)
			== static_cast<typename gray_pixel_iterator_const_reference<Pointer2,Locator>::value_type>(rhs);
	}

	template<typename Poniter1, typename Pointer2, typename Locator>
	inline bool operator!=(
		const gray_pixel_iterator_const_reference<Poniter1,Locator>& lhs,
		const gray_pixel_iterator_const_reference<Pointer2,Locator>& rhs )
	{
		return !(lhs==rhs);
	}
}