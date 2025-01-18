
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

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept;

		CPPX_ATTR_NO_DISCARD native_uint CountActiveVertexBuffers() const noexcept;

		CPPX_ATTR_NO_DISCARD const IAVertexStreamDefinition & GetVertexStreamDefinition() const noexcept;

		void Assign( const IAVertexStreamDefinition & pDefinition );

		IAVertexBufferReference & SetVertexBufferRef( native_uint pIndex );

		void SetVertexBufferRef( native_uint pIndex, const IAVertexBufferReference & pVBReference );
		void SetVertexBufferRefs( const IAVertexBufferReferenceArray & pVBReferences );
		void SetVertexBufferRefs( native_uint pFirstIndex, native_uint pCount, const IAVertexBufferReference * pVBReferences );

		IAIndexBufferReference & SetIndexBufferRef();

		void SetIndexBufferRef( const IAIndexBufferReference & pIBReference );

		void ResetVertexBufferRef( native_uint pIndex );
		void ResetVertexBufferRefs( native_uint pFirstIndex, native_uint pCount );
		void ResetVertexBufferRefs();

		void ResetIndexBufferRef();

		void ResetAllBufferRefs();

	private:
		void _SetVertexBufferRefs( native_uint pFirstIndex, native_uint pCount, const IAVertexBufferReference * pVBReferences );
		void _ResetVertexBufferRefs( native_uint pFirstIndex, native_uint pCount );

	private:
		IAVertexStreamDefinition _vertexStreamDefinition;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_INPUT_ASSEMBLER_DYNAMIC_STATES_H__
