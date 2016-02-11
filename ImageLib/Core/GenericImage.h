#pragma once

#include <iterator>
#include "ImageSourceDispatcher.h"

namespace image_lib
{
	//---------------------------------------------------------------------------
	/*!
	 * @brief   this class provides generic image iterators.
	 * @param   ImageFormat a format that holds information about image iterator.
	 * @param   SourceTag indicates a Source Holder that holds a raw pointer and basic image informations
	 *         (In most cases, It is such as width, height, ...).
	 */
	//---------------------------------------------------------------------------
	template<typename ImageFormat, typename SourceTag>
	class image_t
	{
	public:
		typedef ImageFormat image_format;
		
		typedef typename image_format::row_iterator x_iterator;
		typedef typename image_format::col_iterator y_iterator;
		typedef typename image_format::const_row_iterator const_x_iterator;
		typedef typename image_format::const_col_iterator const_y_iterator;
		
		typedef typename image_format::iterator iterator;
		typedef typename image_format::const_iterator const_iterator;

		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

		typedef typename iterator::pointer pointer;
		typedef typename iterator::distance_type coord_t;
		typedef typename iterator::reference reference;
		typedef typename iterator::value_type value_type;
		typedef typename const_iterator::reference const_reference;

		typedef image_source_tag_dispatcher<SourceTag> source_dispatcher_type;
		typedef typename source_dispatcher_type:: template get_source<pointer>::source source_type;

	private:
		source_type source_;

	public:

		explicit image_t(const source_type& source)
			: source_( source)
		{ }

		// Basic Function
		size_t width() const
		{ 
			return source_.superficial_coordinates_.width_; 
		}

		size_t height() const
		{ 
			return source_.superficial_coordinates_.height_; 
		}

		pointer address() const
		{
			return source_.p_; 
		}

		const source_type& source() const
		{
			return source_;
		}

		source_type& source()
		{
			return source_;
		}

		// 1D Navigation

		iterator begin()
		{
			return iterator( source_ );
		}

		iterator end()
		{
			return begin() + ( width()*height() );
		}

		const_iterator cbegin() const
		{
			return const_iterator( source_ ); 
		}

		const_iterator cend() const
		{
			return cbegin() + ( width()*height() );
		}

		const_iterator begin() const
		{
			return cbegin();
		}

		const_iterator end() const
		{
			return cend();
		}

		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}

		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}

		const_reverse_iterator crbegin() const
		{
			return const_reverse_iterator(cend());
		}

		const_reverse_iterator  crend() const
		{
			return const_reverse_iterator(cbegin());
		}

		const_reverse_iterator rbegin() const
		{
			return crbegin();
		}

		const_reverse_iterator  rend() const
		{
			return crend();
		}

		// X Navigation
		x_iterator row_begin(coord_t y)
		{
			return x_iterator( source_, 0, y );
		}

		x_iterator row_end(coord_t y)
		{
			return row_begin(y)+width();
		}

		x_iterator x_at(coord_t x, coord_t y)
		{
			return x_iterator( source_, x, y );
		}

		const_x_iterator row_cbegin(coord_t y) const
		{
			return const_x_iterator( source_, 0, y );
		}

		const_x_iterator  row_cend(coord_t y) const
		{
			return row_cbegin(y)+width();
		}

		const_x_iterator  x_cat(coord_t x, coord_t y) const
		{
			return const_x_iterator( source_, x, y );
		}

		const_x_iterator row_begin(coord_t y) const
		{
			return row_cbegin();
		}

		const_x_iterator  row_end(coord_t y) const
		{
			return row_cend();
		}

		const_x_iterator  x_at(coord_t x, coord_t y) const
		{
			return x_cat();
		}

		// Y Navigation
		y_iterator col_begin(coord_t x)
		{
			return y_iterator( source_, x, 0 );
		}

		y_iterator col_end(coord_t x)
		{
			return col_begin(x) + height();
		}

		y_iterator y_at(coord_t x, coord_t y)
		{
			return y_iterator( source_, x, y );
		}

		const_y_iterator col_cbegin(coord_t x) const
		{
			return const_y_iterator( source_, x, 0 );
		}

		const_y_iterator col_cend(coord_t x) const
		{
			return col_cbegin(x) + height();
		}

		const_y_iterator y_cat(coord_t x, coord_t y) const
		{
			return const_y_iterator( source_, x, y );
		}

		const_y_iterator col_begin(coord_t x) const
		{
			return col_cbegin();
		}

		const_y_iterator col_end(coord_t x) const
		{
			return col_cend();
		}

		const_y_iterator y_at(coord_t x, coord_t y) const
		{
			return y_cat();
		}
			
		// 2D Navigation
		reference operator()(coord_t x, coord_t y)
		{
			return *x_at(x,y);
		}

		x_iterator operator[](coord_t y)
		{
			return row_begin(y);
		}

		const_reference operator()(coord_t x, coord_t y) const
		{
			return *x_cat(x,y);
		}

		const_x_iterator operator[](coord_t y) const
		{
			return row_cbegin(y);
		}

	};

}