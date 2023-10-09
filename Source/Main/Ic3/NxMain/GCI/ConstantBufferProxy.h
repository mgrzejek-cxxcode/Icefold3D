
#pragma once

#ifndef __IC3_NXMAIN_CONSTANT_BUFFER_PROXY_H__
#define __IC3_NXMAIN_CONSTANT_BUFFER_PROXY_H__

#include "../Prerequisites.h"
#include <Ic3/Graphics/GCI/Resources/GPUBufferCommon.h>

namespace Ic3
{

	struct ConstantBufferDataRef
	{
		uint32 bufferSize = 0;
		void * dataPtr = nullptr;
		uint32 dataSize = 0;
		uint32 modified = 0;
	};

	class ConstantBufferProxy
	{
	public:
		GCI::GPUDevice & mGPUDevice;

	public:
		ConstantBufferProxy( GCI::GPUBufferHandle pBuffer );
		~ConstantBufferProxy();

		GCI::GPUBufferHandle buffer() const noexcept;

		template <typename TData>
		void setData( const TData & pData )
		{
			ic3DebugAssert( sizeof( TData ) == _dataRef.dataSize );
			memCopyUnchecked( _dataRef.dataPtr, _dataRef.dataSize, &pData, sizeof( TData ) );
		}

		template <typename TData, typename TMember >
		void setSubData( TMember TData::* pMember, const TMember & pData )
		{
			const auto offset = memberOffset( pMember );
			const auto size = sizeof( TMember );
			ic3DebugAssert( ( offset < _dataRef.dataSize ) && ( size <= _dataRef.dataSize - offset ) );
			memCopyUnchecked( _dataRef.dataPtr + offset, size, &pData, size );
		}

		template <typename TData>
		void initialize();

		void flushUpdates();

	private:
		GCI::GPUBufferHandle _buffer;
		ConstantBufferDataRef _dataRef;
		DynamicMemoryBuffer _localCache;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_CONSTANT_BUFFER_PROXY_H__
