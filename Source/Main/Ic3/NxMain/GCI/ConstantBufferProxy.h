
#pragma once

#ifndef __IC3_NXMAIN_CONSTANT_BUFFER_PROXY_H__
#define __IC3_NXMAIN_CONSTANT_BUFFER_PROXY_H__

#include "../Prerequisites.h"
#include <Ic3/Graphics/GCI/Resources/GpuBufferCommon.h>

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
		GCI::GpuDevice & mGpuDevice;

	public:
		ConstantBufferProxy( GCI::GpuBufferHandle pBuffer );
		~ConstantBufferProxy();

		GCI::GpuBufferHandle buffer() const noexcept;

		template <typename TData>
		void setData( const TData & pData )
		{
			ic3DebugAssert( sizeof( TData ) == _dataRef.mDataSize );
			memCopyUnchecked( _dataRef.mDataPtr, _dataRef.mDataSize, &pData, sizeof( TData ) );
		}

		template <typename TData, typename TMember >
		void setSubData( TMember TData::* pMember, const TMember & pData )
		{
			const auto offset = memberOffset( pMember );
			const auto size = sizeof( TMember );
			ic3DebugAssert( ( offset < _dataRef.mDataSize ) && (size <= _dataRef.mDataSize - offset ) );
			memCopyUnchecked( _dataRef.mDataPtr + offset, size, &pData, size );
		}

		template <typename TData>
		void initialize();

		void flushUpdates();

	private:
		GCI::GpuBufferHandle _buffer;
		ConstantBufferDataRef _dataRef;
		Cppx::DynamicMemoryBuffer _localCache;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_CONSTANT_BUFFER_PROXY_H__
