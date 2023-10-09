
#pragma once

#ifndef __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_DYNAMIC_STATES_H__
#define __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_DYNAMIC_STATES_H__

#include "InputAssemblerCommon.h"

namespace Ic3::Graphics::GCI
{

	/// @brief
	class IC3_GRAPHICS_GCI_CLASS IAVertexStreamDynamicState
	{
	public:
		IAVertexStreamDynamicState( const IAVertexStreamDynamicState & ) = default;
		IAVertexStreamDynamicState & operator=( const IAVertexStreamDynamicState & ) = default;

		IAVertexStreamDynamicState();
		explicit IAVertexStreamDynamicState( const IAVertexStreamDefinition & pDefinition );

		~IAVertexStreamDynamicState();

		IC3_ATTR_NO_DISCARD bool empty() const noexcept;

		IC3_ATTR_NO_DISCARD native_uint countActiveVertexBuffers() const noexcept;

		IC3_ATTR_NO_DISCARD const IAVertexStreamDefinition & getVertexStreamDefinition() const noexcept;

		void assign( const IAVertexStreamDefinition & pDefinition );

		IAVertexBufferReference & setVertexBufferRef( native_uint pIndex );

		void setVertexBufferRef( native_uint pIndex, const IAVertexBufferReference & pVBReference );
		void setVertexBufferRefs( const IAVertexBufferReferenceArray & pVBReferences );
		void setVertexBufferRefs( native_uint pFirstIndex, native_uint pCount, const IAVertexBufferReference * pVBReferences );

		IAIndexBufferReference & setIndexBufferRef();

		void setIndexBufferRef( const IAIndexBufferReference & pIBReference );

		void resetVertexBufferRef( native_uint pIndex );
		void resetVertexBufferRefs( native_uint pFirstIndex, native_uint pCount );
		void resetVertexBufferRefs();

		void resetIndexBufferRef();

		void resetAllBufferRefs();

	private:
		void _setVertexBufferRefs( native_uint pFirstIndex, native_uint pCount, const IAVertexBufferReference * pVBReferences );
		void _resetVertexBufferRefs( native_uint pFirstIndex, native_uint pCount );

	private:
		IAVertexStreamDefinition _vertexStreamDefinition;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_DYNAMIC_STATES_H__
