
#pragma once

#ifndef __IC3_GRAPHICS_GCI_TEXTURE_COMMON_H__
#define __IC3_GRAPHICS_GCI_TEXTURE_COMMON_H__

#include "TextureDimensions.h"

namespace Ic3::Graphics::GCI
{

	struct TextureCreateInfo;
	struct RenderTargetTextureCreateInfo;

	Ic3GCIDeclareClassHandle( Texture );
	Ic3GCIDeclareClassHandle( RenderTargetTexture );

	/// @brief Identifies all mip levels available in a texture resource. Can be used for all resource accesses.
	constexpr uint32 cxTextureMipLevelAll = cppx::meta::limits<uint32>::max_value;

	/// @brief
	constexpr uint32 cxTextureMSAALevelUndefined = cppx::meta::limits<uint32>::max_value;

	/// @brief
	inline constexpr TextureSize2D cxTextureSize2DUndefined{cppx::meta::limits<uint32>::max_value, cppx::meta::limits<uint32>::max_value };

	/// @brief
	enum ETextureDataCopyFlags : uint32
	{
		eTextureDataCopyFlagModeInvalidateBit = eGPUMemoryMapFlagWriteInvalidateBit,
		eTextureDataCopyFlagsDefault = 0,
	};

	/// @brief Bind flags for texture resources. Used to specify how textures are bound to the pipeline.
	///
	/// Note:
	/// Just like in case of EGPUBufferBindFlags, these flags contain required E_GPU_RESOURCE_USAGE_FLAG bits as well.
	enum ETextureBindFlags : resource_flags_value_t
	{
		//
		eTextureBindFlagCommonTextureBit = 0x010000,

		// Bind flag for using a texture as a sampled image available in one or more shader stages.
		// Such texture is accessible via one of the valid TX input registers and can be read through a sampler object.
		// Implies SHADER_INPUT usage bit.
		eTextureBindFlagShaderInputSampledImageBit =
			eTextureBindFlagCommonTextureBit |
			eGPUResourceUsageFlagShaderInputBit,

		// Bind flag for using a texture as a color attachment image, modified as part of the FBO rendering.
		// Implies RENDER_TARGET_COLOR usage bit.
		eTextureBindFlagRenderTargetColorAttachmentBit =
			eTextureBindFlagCommonTextureBit |
			eGPUResourceUsageFlagRenderTargetColorBit,

		//
		eTextureBindFlagRenderTargetDepthAttachmentBit =
			eTextureBindFlagCommonTextureBit |
			eGPUResourceUsageFlagRenderTargetDepthBit,

		// Bind flag for using a texture as a depth/stencil attachment image, modified as part of the FBO rendering
		// (target storage for depth/stencil values) or read/modified during the depth and/or stencil tests.
		// Implies RENDER_TARGET_DEPTH_STENCIL usage bit.
		eTextureBindFlagRenderTargetDepthStencilAttachmentBit =
			eTextureBindFlagCommonTextureBit |
			eGPUResourceUsageMaskRenderTargetDepthStencil,

		// Bind flag for using a texture as a source in transfer operations. @See E_GPU_RESOURCE_USAGE_FLAG_TRANSFER_SOURCE_BIT.
		eTextureBindFlagTransferSourceImageBit =
			eTextureBindFlagCommonTextureBit |
			eGPUResourceUsageFlagTransferSourceBit,

		// Bind flag for using a texture as a target in transfer operations. @See E_GPU_RESOURCE_USAGE_FLAG_TRANSFER_TARGET_BIT.
		eTextureBindFlagTransferTargetImageBit =
			eTextureBindFlagCommonTextureBit |
			eGPUResourceUsageFlagTransferTargetBit,
	};
	
	enum ETextureInitFlags : uint32
	{
		eTextureInitFlagGenerateMipmapsBit = 0x01
	};

	/// @brief Represents all valid targets for texture resources. Corresponding E_TEXTURE_BIND_FLAGs are used as values.
	enum class ETextureTarget : resource_flags_value_t
	{
		ShaderInputSampledImage            = eTextureBindFlagShaderInputSampledImageBit,
		RenderTargetColorAttachment        = eTextureBindFlagRenderTargetColorAttachmentBit,
		RenderTargetDepthStencilAttachment = eTextureBindFlagRenderTargetDepthStencilAttachmentBit,
		TransferSourceImage                = eTextureBindFlagTransferSourceImageBit,
		TransferTargetImage                = eTextureBindFlagTransferTargetImageBit,
		Unknown = 0
	};

	struct RenderTargetTextureLayout
	{
		ETextureFormat internalFormat = ETextureFormat::Undefined;
		TextureSize2D imageRect{0, 0 };
		uint32 msaaLevel = 0;

		explicit operator bool() const noexcept
		{
			return internalFormat != ETextureFormat::Undefined;
		}
	};

	struct ShaderInputTextureLayout
	{
		ETextureClass texClass = ETextureClass::Unknown;
		ETextureFormat internalFormat = ETextureFormat::Undefined;
		TextureSize2D imageRect{0, 0 };
		uint32 depth = 1;
		uint32 arraySize = 1;
		uint32 mipLevelsNum = 1;
		uint32 msaaLevel = 0;

		explicit operator bool() const noexcept
		{
			return internalFormat != ETextureFormat::Undefined;
		}
	};

	struct TextureLayout
	{
		ETextureClass texClass = ETextureClass::Unknown;
		ETextureFormat internalFormat = ETextureFormat::Undefined;
		TextureDimensions dimensions;
		uint32 msaaLevel;
		uint32 bitsPerPixel;
		uint32 storageSize;

		explicit operator bool() const noexcept
		{
			return texClass != ETextureClass::Unknown;
		}
	};

	struct TextureMipSubLevelInitDataDesc : public ResourceInputDataDesc
	{
		uint32 mipWidth;
		uint32 mipHeight;
		uint32 mipDepth;
		uint32 mipLevelIndex;
	};

	struct TextureSubTextureInitDataDesc
	{
		using MipLevelInitDataDescArray = std::array<TextureMipSubLevelInitDataDesc, GCM::kTextureMaxMipLevelsNum>;
		MipLevelInitDataDescArray mipLevelInitDataArray;
		uint32 subTextureIndex;

		CPPX_ATTR_NO_DISCARD TextureMipSubLevelInitDataDesc & GetMipLevelInitData( native_uint pMipLevel ) noexcept
		{
			return mipLevelInitDataArray[pMipLevel];
		}

		CPPX_ATTR_NO_DISCARD const TextureMipSubLevelInitDataDesc & GetMipLevelInitData( native_uint pMipLevel ) const noexcept
		{
			return mipLevelInitDataArray[pMipLevel];
		}
	};

	class IC3_GRAPHICS_GCI_CLASS TextureInitDataDesc
	{
	public:
		TextureInitDataDesc() = default;

		TextureInitDataDesc( TextureInitDataDesc && pSrcObject );
		TextureInitDataDesc & operator=( TextureInitDataDesc && pRhs );

		TextureInitDataDesc( const TextureInitDataDesc & pSrcObject );
		TextureInitDataDesc & operator=( const TextureInitDataDesc & pRhs );

		using SubTextureInitDataDescArray = std::vector<TextureSubTextureInitDataDesc>;


		explicit operator bool() const
		{
			return _subTextureInitDataBasePtr != nullptr;
		}

		void Initialize( const TextureDimensions & pDimensions, cppx::bitmask<ETextureInitFlags> pInitFlags = 0 );

		void SetInitFlags( cppx::bitmask<ETextureInitFlags> pInitFlags );

		void Swap( TextureInitDataDesc & pOther );

		CPPX_ATTR_NO_DISCARD bool IsArray() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept;

		CPPX_ATTR_NO_DISCARD TextureInitDataDesc Copy() const noexcept;

		CPPX_ATTR_NO_DISCARD cppx::bitmask<ETextureInitFlags> GetInitFlags() const noexcept;

		CPPX_ATTR_NO_DISCARD const TextureSubTextureInitDataDesc * GetSubTextureInitDataArrayPtr() const noexcept;

		CPPX_ATTR_NO_DISCARD TextureSubTextureInitDataDesc & GetSubTextureInitDesc( native_uint pSubTextureIndex ) noexcept;

		CPPX_ATTR_NO_DISCARD const TextureSubTextureInitDataDesc & GetSubTextureInitDesc( native_uint pSubTextureIndex ) const noexcept;

	private:
		// Pointer to one or more TextureSubTextureInitDataDesc objects with init data for sub-textures.
		// Points to either _subTextureInitDataSingleTexture member or the beginning of the data in the
		// _subTextureInitDataTextureArray vector. This is an optimisation done for single textures
		// (arraySize=1) in order to prevent dynamic allocation of memory. In few cases where the engine
		// is used for allocating huge number of small non-array textures, this turned out to be an issue.
		TextureSubTextureInitDataDesc * _subTextureInitDataBasePtr = nullptr;

		cppx::bitmask<ETextureInitFlags> _textureInitFlags;

		// Init data for single-layer (non-array) texture.
		TextureSubTextureInitDataDesc _subTextureInitData;
		
		// Vector of init data structs for array textures.
		SubTextureInitDataDescArray _subTextureInitDataArray;
	};

	struct TextureInputDataDesc : public ResourceInputDataDesc
	{
		EPixelDataLayout pixelDataLayout;

		EBaseDataType pixelDataType;
	};

	struct TextureCreateInfo : public ResourceCreateInfo
	{
		ETextureClass texClass;
		uint32 msaaLevel = 0;
		TextureDimensions dimensions{0, 0, 1, 1, 1 };
		ETextureFormat internalFormat;
		TextureInitDataDesc initDataDesc;

		explicit operator bool() const
		{
			return ( texClass != ETextureClass::Unknown ) && ( internalFormat != ETextureFormat::Undefined );
		}
	};

	struct TextureDataCopyDesc
	{
		ETextureClass texClass = ETextureClass::Unknown;
		cppx::bitmask<ETextureDataCopyFlags> flags = eTextureDataCopyFlagsDefault;
	};

	struct TextureSubDataCopyDesc
	{
		ETextureClass texClass = ETextureClass::Unknown;
		TextureSubRegion sourceTextureSubRegion;
		TextureOffset targetTextureOffset;
		cppx::bitmask<ETextureDataCopyFlags> flags = eTextureDataCopyFlagsDefault;
	};

	struct TextureDataUploadDesc
	{
		ETextureClass texClass = ETextureClass::Unknown;
		TextureInputDataDesc inputDataDesc;
		cppx::bitmask<ETextureDataCopyFlags> flags = eTextureDataCopyFlagsDefault;
	};

	struct TextureSubDataUploadDesc
	{
		ETextureClass texClass = ETextureClass::Unknown;
		TextureSubRegion textureSubRegion;
		TextureInputDataDesc inputDataDesc;
		cppx::bitmask<ETextureDataCopyFlags> flags = eTextureDataCopyFlagsDefault;
	};

	namespace RCU
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool CheckTextureFormatAndBindFlagsCompatibility(
				ETextureFormat pTextureFormat,
				cppx::bitmask<resource_flags_value_t> pBindFlags );

		IC3_GRAPHICS_GCI_API_NO_DISCARD const TextureLayout & QueryTextureLayout( TextureHandle pTexture ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD RenderTargetTextureLayout QueryRenderTargetTextureLayoutForTexture( TextureHandle pTexture ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD RenderTargetTextureLayout QueryRenderTargetTextureLayoutForTexture( const TextureLayout & pTextureLayout ) noexcept;

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_TEXTURE_COMMON_H__
