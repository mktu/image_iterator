#pragma once
#include "../Core/StaticRecursion.h"
#include "BmpImageSource.h"
#include <vector>
#include <assert.h>

namespace image_lib
{
	template<typename Pointer, typename Locator>
	class bmp_pixel_iterator_data
	{
	public:
		typedef Pointer pointer;
		typedef Locator locator;
		typedef ptrdiff_t coord_t;
		typedef typename locator::counter_type counter_type;

		template<typename SourceType>
		bmp_pixel_iterator_data(const SourceType& src) 
			: p_(static_cast<pointer>(src.p_)), l_(src), pos_(src), pallet_(src.pallet_.get()) { }
		template<typename SourceType>
		bmp_pixel_iterator_data(const SourceType& src, const counter_type& offset) 
			: p_(static_cast<pointer>(src.p_)), l_(src), pos_( offset ), pallet_(src.pallet_.get()) { }

		pointer p_;
		locator l_;
		counter_type pos_;
		const bmp_color_pallet_type* pallet_;
	};

	

	template<size_t Channels,typename Pointer, typename Locator>
	struct bmp256_pixel_iterator_const_reference 
	{
		enum{ channels = Channels };

		typedef Pointer pointer;
		typedef bmp_pixel_iterator_data<pointer, Locator> data_type;
		typedef typename std::iterator_traits<pointer> iterator_traits;
		typedef typename iterator_traits::reference reference;
		typedef typename iterator_traits::value_type value_type;
		typedef typename iterator_traits::distance_type distance_type;

		value_type operator[](distance_type d) const 
		{
			quad_type q = (*data_.pallet_)[ index() ];
			return q.byte_[2-d];
		}

		value_type index() const 
		{
			return *(data_.p_+data_.l_(data_.pos_));
		}

		bmp256_pixel_iterator_const_reference( const data_type& base ) : data_( base ) { } 

	private:
		bmp256_pixel_iterator_const_reference& operator=(const bmp256_pixel_iterator_const_reference&);
	protected:
		data_type data_;
	};

	template<size_t Channels, typename Pointer, typename Locator>
	struct bmp_pixel_iterator_const_reference 
	{
		enum{ channels = Channels };

		typedef Pointer pointer;
		typedef bmp_pixel_iterator_data<pointer, Locator> data_type;
		typedef typename std::iterator_traits<pointer> iterator_traits;
		typedef typename iterator_traits::reference reference;
		typedef typename iterator_traits::value_type value_type;
		typedef typename iterator_traits::distance_type distance_type;

		value_type operator[](distance_type d) const 
		{
			return *(data_.p_+data_.l_(data_.pos_, d));
		}

		bmp_pixel_iterator_const_reference( const data_type& base ) : data_( base ) { } 

	private:
		bmp_pixel_iterator_const_reference& operator=(const bmp_pixel_iterator_const_reference&);
	protected:
		data_type data_;
	};

	template<typename Pointer, typename Locator>
	struct bmp_pixel_iterator_const_reference<1, Pointer, Locator> 
	{
		typedef bmp_pixel_iterator_data<Pointer, Locator> data_type;
		typedef typename std::iterator_traits<Pointer> iterator_traits;
		typedef typename iterator_traits::reference reference;
		typedef typename iterator_traits::value_type value_type;
		typedef typename iterator_traits::distance_type distance_type;

		operator value_type () const 
		{
			return *(data_.p_+data_.l_(data_.pos_));
		}

		bmp_pixel_iterator_const_reference( const data_type& base ) : data_( base ) { } 

	private:
		bmp_pixel_iterator_const_reference& operator=(const bmp_pixel_iterator_const_reference&);
	protected:
		data_type data_;
	};

	template<size_t Channels, typename Pointer, typename Locator>
	struct bmp_pixel_iterator_reference : public bmp_pixel_iterator_const_reference<Channels, Pointer, Locator>
	{
		typedef bmp_pixel_iterator_const_reference<Channels, Pointer, Locator> base_type;
		typedef typename base_type::data_type data_type;
		typedef typename base_type::reference reference;
		typedef typename base_type::value_type value_type;
		typedef typename base_type::distance_type distance_type;

		template<typename ArrayAccesible>
		bmp_pixel_iterator_reference& operator=(const ArrayAccesible&  right)
		{
			detail::static_recursion<channels>::copy( *this, right );
			return *this;
		}

		bmp_pixel_iterator_reference& operator=(const bmp_pixel_iterator_reference& right)
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
			return *(data_.p_+data_.l_(data_.pos_, d));
		}

		bmp_pixel_iterator_reference( const data_type& base ) : base_type( base ) { } 
	};

	template<size_t Channels, typename Pointer, typename Locator>
	struct bmp256_pixel_iterator_reference : public bmp256_pixel_iterator_const_reference<Channels, Pointer, Locator>
	{
		typedef bmp256_pixel_iterator_const_reference<Channels, Pointer, Locator> base_type;
		typedef typename base_type::data_type data_type;
		typedef typename base_type::value_type value_type;
		typedef typename base_type::distance_type distance_type;

		bmp256_pixel_iterator_reference& operator=(const value_type&  right)
		{
			*(data_.p_+data_.l_(data_.pos_)) = right;
			return *this;
		}

		bmp256_pixel_iterator_reference& operator=(const base_type& right)
		{
			*(data_.p_+data_.l_(data_.pos_)) = right.index();
			return *this;
		}

		bmp256_pixel_iterator_reference& operator=(const bmp256_pixel_iterator_reference& right)
		{
			return this->operator=( static_cast<const base_type&>( right ) );
		}

		value_type operator[](distance_type d) const 
		{
			return base_type::operator[](d);
		}

		bmp256_pixel_iterator_reference( const data_type& base ) : base_type( base ) { } 
	};

	template<typename Pointer, typename Locator>
	struct bmp_pixel_iterator_reference<1, Pointer, Locator> : public bmp_pixel_iterator_const_reference<1, Pointer, Locator>
	{
		typedef bmp_pixel_iterator_const_reference<1, Pointer, Locator> base_type;
		typedef typename base_type::data_type data_type;
		typedef typename base_type::value_type value_type;

		bmp_pixel_iterator_reference& operator=(const value_type&  right)
		{
			*(data_.p_+data_.l_(data_.pos_)) = right;
			return *this;
		}

		bmp_pixel_iterator_reference& operator=(const bmp_pixel_iterator_reference& right)
		{
			*(data_.p_+data_.l_(data_.pos_)) = static_cast<value_type>(right);
			return *this;
		}

		operator value_type () const 
		{
			return base_type::operator value_type();
		}

		bmp_pixel_iterator_reference( const data_type& base ) : base_type( base ) { } 
	};

	template<size_t Channels, typename Pointer, typename Locator>
	struct bmp_pixel_iterator_value : public bmp_pixel_iterator_reference<Channels, Pointer, Locator>
	{
		typedef bmp_pixel_iterator_reference<Channels, Pointer, Locator> base_type;
		typedef bmp_source_holder_base<Pointer> holder_base;
		typedef typename base_type::value_type value_type;

		template<typename ArrayAccesible>
		bmp_pixel_iterator_value(const ArrayAccesible&  right) : base_type( holder_base( values_, 1, 1, 4) ) 
		{
			detail::static_recursion<channels>::copy( *this, right );
		}

	private:
		value_type values_[Channels];
	};

	template<size_t Channels, typename Pointer, typename Locator>
	struct bmp256_pixel_iterator_value : public bmp256_pixel_iterator_reference<Channels, Pointer, Locator>
	{
		typedef bmp256_pixel_iterator_reference<Channels, Pointer, Locator> base_type;
		typedef bmp256_pixel_iterator_const_reference<Channels, Pointer, Locator> const_iterator_reference;
		typedef bmp_source_holder_base<Pointer> holder_base;
		typedef typename base_type::value_type value_type;

		bmp256_pixel_iterator_value( const const_iterator_reference& base ) : base_type( holder_base( values_, 1, 1, 4 ) ), pallet_(1)
		{
			pallet_[0].quad_.rgbRed = base[0];
			pallet_[0].quad_.rgbGreen = base[1];
			pallet_[0].quad_.rgbBlue= base[2];
			values_[0]=0;
			data_.pallet_=&pallet_;
		} 

		template<typename ArrayAccesible>
		bmp256_pixel_iterator_value(const ArrayAccesible&  right) : base_type( holder_base( values_, 1, 1, 4 ) ), pallet_(1)
		{
			pallet_[0].quad_.rgbRed = right[0];
			pallet_[0].quad_.rgbGreen = right[1];
			pallet_[0].quad_.rgbBlue= right[2];
			values_[0]=0;
			data_.pallet_=&pallet_;
		}

	private:
		value_type values_[1];
		bmp_color_pallet_type pallet_;
	};

	template<typename Pointer, typename Locator>
	struct bmp_pixel_iterator_value<1, Pointer, Locator> : public bmp_pixel_iterator_reference<1, Pointer, Locator>
	{
		typedef bmp_pixel_iterator_reference<1, Pointer, Locator> base_type;
		typedef bmp_source_holder_base<Pointer> holder_base;
		typedef typename base_type::value_type value_type;

		bmp_pixel_iterator_value(const value_type&  right) : values_(right), base_type( holder_base( &values_, 1, 1, 4 ) ) 
		{
		}

	private:
		value_type values_;
	};

	template<size_t Channels,typename Poniter1, typename Pointer2, typename Locator>
	inline bool operator==(
		const bmp256_pixel_iterator_const_reference <Channels,Poniter1,Locator>& lhs,
		const bmp256_pixel_iterator_const_reference <Channels,Pointer2,Locator>& rhs )
	{
		return detail::static_recursion<Channels>::equals(lhs,rhs);
	}

	template<size_t Channels,typename Poniter1, typename Pointer2, typename Locator>
	inline bool operator!=(
		const bmp256_pixel_iterator_const_reference <Channels,Poniter1,Locator>& lhs,
		const bmp256_pixel_iterator_const_reference <Channels,Pointer2,Locator>& rhs )
	{
		return !(lhs==rhs);
	}

	template<size_t Channels, typename Poniter1, typename Pointer2, typename Locator>
	inline bool operator==(
		const bmp_pixel_iterator_const_reference<Channels,Poniter1,Locator>& lhs,
		const bmp_pixel_iterator_const_reference<Channels,Pointer2,Locator>& rhs )
	{
		return detail::static_recursion<Channels>::equals(lhs,rhs);
	}

	template<size_t Channels, typename Poniter1, typename Pointer2, typename Locator>
	inline bool operator!=(
		const bmp_pixel_iterator_const_reference<Channels,Poniter1,Locator>& lhs,
		const bmp_pixel_iterator_const_reference<Channels,Pointer2,Locator>& rhs )
	{
		return !(lhs==rhs);
	}

	template<typename Poniter1, typename Pointer2, typename Locator>
	inline bool operator==(
		const bmp_pixel_iterator_const_reference<1,Poniter1,Locator>& lhs,
		const bmp_pixel_iterator_const_reference<1,Pointer2,Locator>& rhs )
	{
		return static_cast<typename bmp_pixel_iterator_const_reference<1,Poniter1,Locator>::value_type>(lhs)
			== static_cast<typename bmp_pixel_iterator_const_reference<1,Pointer2,Locator>::value_type>(rhs);
	}

	template<typename Poniter1, typename Pointer2, typename Locator>
	inline bool operator!=(
		const bmp_pixel_iterator_const_reference<1,Poniter1,Locator>& lhs,
		const bmp_pixel_iterator_const_reference<1,Pointer2,Locator>& rhs )
	{
		return !(lhs==rhs);
	}
}