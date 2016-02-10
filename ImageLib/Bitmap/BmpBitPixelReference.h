#pragma once
#include "BmpPixelReference.h"

namespace image_lib
{
	template<typename Pointer, typename Locator>
	struct bmp1_pixel_iterator_const_reference 
	{
		enum{ channels = 3 };

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
			return bit() ? 1 : 0;
		}

		bool bit() const
		{
			return data_.l_.get_bit(data_.p_, data_.pos_);
		}

		bmp1_pixel_iterator_const_reference( const data_type& base ) : data_( base ) { } 

	private:
		bmp1_pixel_iterator_const_reference& operator=(const bmp1_pixel_iterator_const_reference&);
	protected:
		data_type data_;
	};

	template<typename Pointer, typename Locator>
	struct bmp1_pixel_iterator_reference : public bmp1_pixel_iterator_const_reference<Pointer, Locator>
	{
		typedef bmp1_pixel_iterator_const_reference<Pointer, Locator> base_type;
		typedef typename base_type::data_type data_type;
		typedef typename base_type::value_type value_type;
		typedef typename base_type::distance_type distance_type;

		bmp1_pixel_iterator_reference& operator=(bool right)
		{
			data_.l_.set_bit(data_.p_, data_.pos_, right);
			return *this;
		}

		bmp1_pixel_iterator_reference& operator=(value_type right)
		{
			data_.l_.set_bit(data_.p_, data_.pos_, right == 1);
			return *this;
		}

		bmp1_pixel_iterator_reference& operator=(const base_type& right)
		{
			return this->operator=( right.bit() );
		}

		bmp1_pixel_iterator_reference& operator=(const bmp1_pixel_iterator_reference& right)
		{
			return this->operator=( static_cast<const base_type&>( right ) );
		}

		value_type operator[](distance_type d) const 
		{
			return base_type::operator[](d);
		}

		bmp1_pixel_iterator_reference( const data_type& base ) : base_type( base ) { } 
	};

	template<typename Pointer, typename Locator>
	struct bmp1_pixel_iterator_value : public bmp1_pixel_iterator_reference<Pointer, Locator>
	{
		typedef bmp1_pixel_iterator_reference<Pointer, Locator> base_type;
		typedef bmp1_pixel_iterator_const_reference<Pointer, Locator> const_iterator_reference;
		typedef bmp_source_holder_base<Pointer> holder_base;
		typedef typename base_type::value_type value_type;

		bmp1_pixel_iterator_value( const const_iterator_reference& base ) : base_type( holder_base( values_, 1, 1, 4 ) ), pallet_(1)
		{
			pallet_[0].quad_.rgbRed = base[0];
			pallet_[0].quad_.rgbGreen = base[1];
			pallet_[0].quad_.rgbBlue= base[2];
			values_[0]=0;
			data_.pallet_=&pallet_;
		} 

		template<typename ArrayAccesible>
		bmp1_pixel_iterator_value(const ArrayAccesible&  right) : base_type( holder_base( values_, 1, 1, 4 ) ), pallet_(1)
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

	template<typename Poniter1, typename Pointer2, typename Locator>
	inline bool operator==(
		const bmp1_pixel_iterator_const_reference <Poniter1,Locator>& lhs,
		const bmp1_pixel_iterator_const_reference <Pointer2,Locator>& rhs )
	{
		return detail::static_recursion<3>::equals(lhs,rhs);
	}

	template<typename Poniter1, typename Pointer2, typename Locator>
	inline bool operator!=(
		const bmp1_pixel_iterator_const_reference <Poniter1,Locator>& lhs,
		const bmp1_pixel_iterator_const_reference <Pointer2,Locator>& rhs )
	{
		return !(lhs==rhs);
	}

	// mono reference

	template<typename Pointer, typename Locator>
	struct bmp_mono_pixel_iterator_const_reference 
	{
		typedef Pointer pointer;
		typedef bmp_pixel_iterator_data<pointer, Locator> data_type;
		typedef typename std::iterator_traits<pointer> iterator_traits;
		typedef typename iterator_traits::reference reference;
		typedef typename iterator_traits::value_type value_type;
		typedef typename iterator_traits::distance_type distance_type;

		operator bool() const
		{
			return data_.l_.get_bit(data_.p_, data_.pos_);
		}

		bmp_mono_pixel_iterator_const_reference( const data_type& base ) : data_( base ) { } 

	private:
		bmp_mono_pixel_iterator_const_reference& operator=(const bmp_mono_pixel_iterator_const_reference&);
	protected:
		data_type data_;
	};

	template<typename Pointer, typename Locator>
	struct bmp_mono_pixel_iterator_reference : public bmp_mono_pixel_iterator_const_reference<Pointer, Locator>
	{
		typedef bmp_mono_pixel_iterator_const_reference<Pointer, Locator> base_type;
		typedef typename base_type::data_type data_type;
		typedef typename base_type::value_type value_type;
		typedef typename base_type::distance_type distance_type;

		bmp_mono_pixel_iterator_reference& operator=(bool right)
		{
			data_.l_.set_bit(data_.p_, data_.pos_, right);
			return *this;
		}

		bmp_mono_pixel_iterator_reference& operator=(value_type right)
		{
			data_.l_.set_bit(data_.p_, data_.pos_, right == 1);
			return *this;
		}

		bmp_mono_pixel_iterator_reference& operator=(const base_type& right)
		{
			return this->operator=( static_cast<bool>(right) );
		}

		bmp_mono_pixel_iterator_reference& operator=(const bmp_mono_pixel_iterator_reference& right)
		{
			return this->operator=( static_cast<const base_type&>( right ) );
		}

		operator bool() const
		{
			return base_type::operator bool();
		}

		bmp_mono_pixel_iterator_reference( const data_type& base ) : base_type( base ) { } 
	};

	template<typename Pointer, typename Locator>
	struct bmp_mono_pixel_iterator_value : public bmp_mono_pixel_iterator_reference<Pointer, Locator>
	{
		typedef bmp_mono_pixel_iterator_reference<Pointer, Locator> base_type;
		typedef bmp_mono_pixel_iterator_const_reference<Pointer, Locator> const_iterator_reference;
		typedef bmp_source_holder_base<Pointer> holder_base;
		typedef typename base_type::value_type value_type;

		bmp_mono_pixel_iterator_value( const const_iterator_reference& base ) : base_type( holder_base( values_, 1, 1, 4 ) ), pallet_(1)
		{
			value_type v = base ? 255 : 0;
			pallet_[0].quad_.rgbRed = v;
			pallet_[0].quad_.rgbGreen = v;
			pallet_[0].quad_.rgbBlue= v;
			values_[0]=v;
			data_.pallet_=&pallet_;
		} 

		bmp_mono_pixel_iterator_value(bool right) : base_type( holder_base( values_, 1, 1, 4 ) ), pallet_(1)
		{
			value_type v = right ? 255 : 0;
			pallet_[0].quad_.rgbRed = v;
			pallet_[0].quad_.rgbGreen = v;
			pallet_[0].quad_.rgbBlue= v;
			values_[0]=v;
			data_.pallet_=&pallet_;
		}

	private:
		value_type values_[1];
		bmp_color_pallet_type pallet_;
	};
}