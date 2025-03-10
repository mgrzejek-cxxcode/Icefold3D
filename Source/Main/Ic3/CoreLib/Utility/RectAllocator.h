
#ifndef __IC3_CORELIB_RECT_ALLOCATOR_H__
#define __IC3_CORELIB_RECT_ALLOCATOR_H__

#include "../Prerequisites.h"

#include <cxm/vectorOps.h>

namespace Ic3
{

	struct RectAllocatorConfig
	{
		struct HorizontalLayout
		{
			uint32 hLineAdvanceThreshold = cppx::meta::limits<uint32>::max_value;
			uint32 hSpacing = 0;
		};

		struct VerticalLayout
		{
			uint32 baseLineHeight;
			uint32 vSpacing = 0;
		};

		HorizontalLayout horizontalLayout;
		VerticalLayout verticalLayout;
	};

	class IC3_CORELIB_CLASS RectAllocator
	{
	public:
		RectAllocator( const cxm::vec2u32 & pBoundingRectDimensions, const RectAllocatorConfig & pAllocatorConfig );
		~RectAllocator();

		bool AddRect( const cxm::vec2u32 & pRect, cxm::vec2u32 * pOutPosition );

		void UpdateHorizontalLayout( RectAllocatorConfig::HorizontalLayout & pHorizontalLayout );
		void UpdateVerticalLayout( RectAllocatorConfig::VerticalLayout & pVerticalLayout );

		void Reset();

		CPPX_ATTR_NO_DISCARD bool CheckFreeSpace( const cxm::vec2u32 & pRect ) const;

        CPPX_ATTR_NO_DISCARD bool IsEmpty() const;

        CPPX_ATTR_NO_DISCARD const RectAllocatorConfig & getConfig() const
		{
			return _config;
		}

	private:
		const cxm::vec2u32 _boundingRectDimensions;
		RectAllocatorConfig _config;
		cxm::vec2u32       _allocPointerOffset;
		uint32              _rectCounter;
	};
	
}

#endif // __IC3_CORELIB_RECT_ALLOCATOR_H__
