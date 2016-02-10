#pragma once

#include <vector>
#include <memory>
#include "SourceHolderBase.h"
#include "ImageSourceTag.h"
#include "ImageSourceDispatcher.h"

namespace image_lib
{
	//---------------------------------------------------------------------------
	/*!
	 * @brief   This class holds a raw pointer and a basic information of the image.\n
	 *          The pointer is not managed by this class.
	 * @param   Pointer A pointer that indicates a beginning of the image.
	 */
	//---------------------------------------------------------------------------
	template<typename Pointer>
	struct non_ownership_source_holder : public source_holder_base<Pointer>
	{
		typedef tag_bmp_non_ownership tag_type;
		typedef source_holder_base<Pointer> base_type;
		typedef typename base_type::pointer pointer;
		non_ownership_source_holder( pointer p, coord_t w, coord_t h ) : base_type( p, w, h )
			{ }
	};

	//---------------------------------------------------------------------------
	/*!
	 * @brief   This class holds a shared memory and a basic information of the image.\n
	 *          The shared memory is managed by instances of this class.
	 *			(When the last instance is destroyed, the shared memory will be deleted.)
	 * @param   Pointer A pointer that indicates a beginning of the image.
	 */
	//---------------------------------------------------------------------------
	template<typename Pointer>
	struct co_ownership_source_holder : public source_holder_base<Pointer>
	{
		typedef tag_bmp_co_ownership tag_type;
		typedef source_holder_base<Pointer> base_type;
		typedef typename base_type::pointer pointer;
		typedef typename base_type::value_type value_type;
		typedef std::vector<value_type> array_type;
		typedef std::shared_ptr<array_type> shared_array_type;

		co_ownership_source_holder( const shared_array_type& shared_array, coord_t w, coord_t h) : 
			array_(shared_array), base_type( &(*shared_array)[0], w,h) { }

		shared_array_type array_;
	};


	template<>
	struct image_source_tag_dispatcher<tag_co_ownership>
	{
		template<typename Pointer>
		struct get_source
		{
			typedef co_ownership_source_holder<Pointer> source;
		};
	};


	template<>
	struct image_source_tag_dispatcher<tag_non_ownership>
	{
		template<typename Pointer>
		struct get_source
		{
			typedef non_ownership_source_holder<Pointer> source;
		};
	};
}