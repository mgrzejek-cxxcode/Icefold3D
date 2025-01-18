
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX12_CORE_DEFS_H__
#define __IC3_GRAPHICS_HW3D_DX12_CORE_DEFS_H__

namespace Ic3::Graphics::GCI
{

	constexpr SIZE_T cvD3D12CpuDescriptorPtrInvalid = cppx::meta::limits<SIZE_T>::max_value;

	constexpr D3D12_CPU_DESCRIPTOR_HANDLE cvD3D12CpuDescriptorEmpty{ cvD3D12CpuDescriptorPtrInvalid };

	constexpr auto cvD3D12CommandListTypeInvalid = static_cast<D3D12_COMMAND_LIST_TYPE>( limits<uint32>::max_value );

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX12_CORE_DEFS_H__
