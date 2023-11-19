
#ifndef __IC3_CORELIB_RECT_ALLOCATOR_H__
#define __IC3_CORELIB_RECT_ALLOCATOR_H__

#include "../MathImports.h"

namespace Ic3
{

	struct RectAllocatorConfig
	{
		struct HorizontalLayout
		{
			uint32 hLineAdvanceThreshold = QLimits<uint32>::maxValue;
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

	class RectAllocator
	{
	public:
		RectAllocator( const Math::Vec2u32 & pBoundingRectDimensions, const RectAllocatorConfig & pAllocatorConfig );
		~RectAllocator();

		bool addRect( const Math::Vec2u32 & pRect, Math::Vec2u32 * pOutPosition );

		void updateHorizontalLayout( RectAllocatorConfig::HorizontalLayout & pHorizontalLayout );
		void updateVerticalLayout( RectAllocatorConfig::VerticalLayout & pVerticalLayout );

		void reset();

		IC3_ATTR_NO_DISCARD bool checkFreeSpace( const Math::Vec2u32 & pRect ) const;
        IC3_ATTR_NO_DISCARD bool empty() const;

        IC3_ATTR_NO_DISCARD const RectAllocatorConfig & getConfig() const
		{
			return _config;
		}

	private:
		const Math::Vec2u32 _boundingRectDimensions;
		RectAllocatorConfig _config;
		Math::Vec2u32       _allocPointerOffset;
		uint32              _rectCounter;
	};
	
}

#endif // __IC3_CORELIB_RECT_ALLOCATOR_H__
