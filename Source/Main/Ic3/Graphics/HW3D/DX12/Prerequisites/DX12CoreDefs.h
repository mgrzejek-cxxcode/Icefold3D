
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX12_CORE_DEFS_H__
#define __IC3_GRAPHICS_HW3D_DX12_CORE_DEFS_H__

namespace Ic3::Graphics::GCI
{

	constexpr SIZE_T cvD3D12CPUDescriptorPtrInvalid = Limits<SIZE_T>::maxValue;

	constexpr D3D12_CPU_DESCRIPTOR_HANDLE cvD3D12CPUDescriptorEmpty{ cvD3D12CPUDescriptorPtrInvalid };

	constexpr auto cvD3D12CommandListTypeInvalid = static_cast<D3D12_COMMAND_LIST_TYPE>( Limits<uint32>::maxValue );

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX12_CORE_DEFS_H__
