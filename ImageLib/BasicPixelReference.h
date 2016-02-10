#pragma once

#include <iterator>
#include "SourceHolderBase.h"
#include "StaticRecursion.h"

namespace image_lib
{
	template<typename Pointer, typename Locator>
	class basic_pixel_iterator_data
	{
	public:
		typedef Pointer pointer;
		typedef Locator locator;
		typedef typename locator::counter_type counter_type;
		typedef ptrdiff_t coord_t;

		template<typename SourceType>
		basic_pixel_iterator_data(const SourceType& src) 
			: p_(static_cast<pointer>(src.p_)), l_(src), pos_(src){ }
		template<typename SourceType>
		basic_pixel_iterator_data(const SourceType& src, const counter_type& offset) 
			: p_(static_cast<pointer>(src.p_)), l_(src), pos_( offset ) { }

		pointer p_;
		locator l_;
		counter_type pos_;
	};

	template<size_t Channels, typename Pointer, typename Locator>
	struct basic_pixel_iterator_const_reference
	{
		enum{ channels = Channels };

		typedef Locator locator;
		typedef Pointer pointer;
		typedef basic_pixel_iterator_data<pointer, locator> data_type;
		typedef typename std::iterator_traits<pointer> iterator_traits;
		typedef typename iterator_traits::reference reference;
		typedef typename iterator_traits::value_type value_type;
		typedef typename iterator_traits::distance_type distance_type, difference_type;

		value_type operator[](distance_type d) const 
		{
			return *(data_.p_+data_.l_(data_.pos_, d));
		}

		basic_pixel_iterator_const_reference( const data_type& base ) : data_( base ) { } 

	private:
		basic_pixel_iterator_const_reference& operator=(const basic_pixel_iterator_const_reference&);
	protected:
		data_type data_;
	};


	template<size_t Channels, typename Pointer, typename Locator>
	struct basic_pixel_iterator_reference : public basic_pixel_iterator_const_reference<Channels, Pointer, Locator>
	{
		typedef basic_pixel_iterator_const_reference<Channels, pointer, locator> base_type;
		typedef typename base_type::data_type data_type;
		typedef typename base_type::reference reference;
		typedef typename base_type::value_type value_type;
		typedef typename base_type::distance_type distance_type, difference_type;

		template<typename ArrayAccesible>
		basic_pixel_iterator_reference& operator=(const ArrayAccesible&  right)
		{
			detail::static_recursion<channels>::copy( *this, right );
			return *this;
		}

		basic_pixel_iterator_reference& operator=(const basic_pixel_iterator_reference& right)
		{
			detail::static_recursion<channels>::copy( *this, right );
			return *this;
		}

		value_type operator[](distance_type d) const 
		{
			return base_type::operator[](d);
		}

		reference operator[](distance_type d) 
		{
			return *(data_.p_+data_.l_(data_.pos_,d));
		}

		basic_pixel_iterator_reference( const data_type& base ) : base_type( base ) { } 
	};

	template<size_t Channels, typename Pointer, typename Locator>
	struct basic_pixel_iterator_value : public basic_pixel_iterator_reference<Channels, Pointer, Locator>
	{
		typedef source_holder_base<pointer> holder_base; 
		typedef basic_pixel_iterator_reference<Channels, pointer, locator> base_type;
		typedef typename base_type::value_type value_type;

		template<typename ArrayAccesible>
		basic_pixel_iterator_value(const ArrayAccesible&  right) : base_type( holder_base(values_, 1, 1) )
		{
			detail::static_recursion<channels>::copy( *this, right );
		}

	private:
		value_type values_[Channels];
	};

	template<size_t Channels, typename Poniter1, typename Pointer2, typename Locator>
	inline bool operator==(
		const basic_pixel_iterator_const_reference<Channels,Poniter1,Locator>& lhs,
		const basic_pixel_iterator_const_reference<Channels,Pointer2,Locator>& rhs)
	{
		return detail::static_recursion<Channels>::equals(lhs,rhs);
	}

	template<size_t Channels, typename Poniter1, typename Pointer2, typename Locator>
	inline bool operator!=(
		const basic_pixel_iterator_const_reference<Channels,Poniter1,Locator>& lhs,
		const basic_pixel_iterator_const_reference<Channels,Pointer2,Locator>& rhs)
	{
		return ! detail::static_recursion<Channels>::equals(lhs,rhs);
	}
}