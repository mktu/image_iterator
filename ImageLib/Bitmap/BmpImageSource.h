#pragma once

#include <vector>
#include <memory>
#include <Windows.h>
#include "../SourceHolderBase.h"
#include "../ImageSourceTag.h"
#include "../ImageSourceDispatcher.h"

namespace image_lib
{
#pragma pack(1)
	union quad_type
	{
		RGBQUAD quad_;
		BYTE byte_[4];
		UINT32 uint_;
	};
#pragma pack()

	inline quad_type get_initialized_quad()
	{
		quad_type q;
		q.uint_ = 0;
		return q;
	}

	typedef std::vector<quad_type> bmp_color_pallet_type;
	typedef std::shared_ptr<bmp_color_pallet_type> shared_bmp_color_pallet;

	template<typename Pointer>
	struct bmp_source_holder_base : public source_holder_base<Pointer>
	{
		typedef source_holder_base<Pointer> base_type;
		typedef typename base_type::pointer pointer;
		typedef typename base_type::value_type value_type;

		bmp_source_holder_base( pointer p, coord_t w, coord_t h, coord_t w_in_byte) : 
			pallet_(std::make_shared<bmp_color_pallet_type>()), width_in_byte_( w_in_byte ), base_type( p, w, h ) { }

		bmp_source_holder_base( pointer p, const bmp_color_pallet_type& pallet, coord_t w, coord_t h, coord_t w_in_byte) : 
			pallet_(std::make_shared<bmp_color_pallet_type>(pallet)), width_in_byte_( w_in_byte ), base_type( p, w, h ) { }

		shared_bmp_color_pallet pallet_;
		coord_t width_in_byte_;
	};


	template<typename Pointer>
	class indicator_top_of_image 
	{
		Pointer pointer_;
	public:
		typedef Pointer pointer;

		explicit indicator_top_of_image(Pointer pointer) : pointer_(pointer) { }

		Pointer retrieve_top_of_address(coord_t width_in_byte, coord_t height) const
		{
			return pointer_ - (width_in_byte * (height-1) );
		}
	};

	template<typename Pointer>
	class indicator_top_of_address
	{
		Pointer pointer_;
	public:
		typedef Pointer pointer;

		explicit indicator_top_of_address(Pointer pointer) : pointer_(pointer) { }

		Pointer retrieve_top_of_address(coord_t , coord_t ) const
		{
			return pointer_ ;
		}
	};

	template<typename Pointer>
	struct co_ownership_bmp_source_holder : public bmp_source_holder_base<Pointer>
	{
		typedef bmp_source_holder_base base_type;
		typedef typename base_type::pointer pointer;
		typedef typename base_type::value_type value_type;
		typedef std::vector<value_type> array_type;
		typedef std::shared_ptr<array_type> shared_array_type;
		typedef tag_bmp_co_ownership tag_type;

	private:
		static coord_t width_in_byte( const shared_array_type& shared_array, coord_t h){
			return h > 0 ? shared_array->size() / h : 0;
		}
	public:

		co_ownership_bmp_source_holder( const shared_array_type& shared_array, coord_t w, coord_t h) : 
			array_(shared_array), base_type( &(*shared_array)[0], w, h, width_in_byte(shared_array, h) ) { }

		co_ownership_bmp_source_holder( const shared_array_type& shared_array, const bmp_color_pallet_type& pallet, coord_t w, coord_t h) : 
			array_(shared_array), base_type( &(*shared_array)[0], pallet, w, h, width_in_byte(shared_array, h) ) { }

		shared_array_type array_;
	};

	template<typename Pointer>
	struct non_ownership_bmp_source_holder : public bmp_source_holder_base<Pointer>
	{
		typedef bmp_source_holder_base base_type;
		typedef typename base_type::pointer pointer;
		typedef typename base_type::value_type value_type;
		typedef tag_bmp_non_ownership tag_type;
	public:

		template<typename PointerIndicator>
		non_ownership_bmp_source_holder( const bmp_color_pallet_type& pallet, const PointerIndicator& indicator, coord_t w, coord_t h, coord_t width_in_byte) : 
			base_type( indicator.retrieve_top_of_address(width_in_byte, h), pallet, w, h, width_in_byte) { }
	};

	typedef co_ownership_bmp_source_holder<BYTE*> bmp_source_holder;

	template<>
	struct image_source_tag_dispatcher<tag_bmp_co_ownership>
	{
		template<typename Pointer>
		struct get_source
		{
			typedef co_ownership_bmp_source_holder<Pointer> source;
		};
	};


	template<>
	struct image_source_tag_dispatcher<tag_bmp_non_ownership>
	{
		template<typename Pointer>
		struct get_source
		{
			typedef non_ownership_bmp_source_holder<Pointer> source;
		};
	};
}