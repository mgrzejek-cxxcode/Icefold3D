
#pragma once

#ifndef __IC3_GRAPHICS_GCI_TEXTURE_COMMON_H__
#define __IC3_GRAPHICS_GCI_TEXTURE_COMMON_H__

#include "TextureDimensions.h"

namespace Ic3::Graphics::GCI
{

	struct TextureCreateInfo;
	struct RenderTargetTextureCreateInfo;

	ic3DeclareClassHandle( Texture );
	ic3DeclareClassHandle( RenderTargetTexture );

	/// @brief Identifies all mip levels available in a texture resource. Can be used for all resource accesses.
	constexpr uint32 cxTextureMipLevelAll = QLimits<uint32>::sMaxValue;

	/// @brief
	constexpr uint32 cxTextureMSAALevelUndefined = QLimits<uint32>::sMaxValue;

	/// @brief
	inline constexpr TextureSize2D cxTextureSize2DUndefined{QLimits<uint32>::sMaxValue, QLimits<uint32>::sMaxValue };

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

		// Bind flag for using a texture as a source in transfer operations. @See E_GPU_RESOURCE_USAGE_FLAG_TRANSFER_SOURCEBit.
		eTextureBindFlagTransferSourceImageBit =
			eTextureBindFlagCommonTextureBit |
			eGPUResourceUsageFlagTransferSourceBit,

		// Bind flag for using a texture as a target in transfer operations. @See E_GPU_RESOURCE_USAGE_FLAG_TRANSFER_TARGETBit.
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
		ETextureFormat mInternalFormat = ETextureFormat::Unknown;
		TextureSize2D mImageRect{0, 0 };
		uint32 mMSAALevel = 0;

		explicit operator bool() const noexcept
		{
			return mInternalFormat != ETextureFormat::Unknown;
		}
	};

	struct ShaderInputTextureLayout
	{
		ETextureClass mTexClass = ETextureClass::Unknown;
		ETextureFormat mInternalFormat = ETextureFormat::Unknown;
		TextureSize2D mImageRect{0, 0 };
		uint32 mDepth = 1;
		uint32 mArraySize = 1;
		uint32 mMipLevelsNum = 1;
		uint32 mMSAALevel = 0;

		explicit operator bool() const noexcept
		{
			return mInternalFormat != ETextureFormat::Unknown;
		}
	};

	struct TextureLayout
	{
		ETextureClass mTexClass = ETextureClass::Unknown;
		ETextureFormat mInternalFormat = ETextureFormat::Unknown;
		TextureDimensions mDimensions;
		uint32 mMSAALevel;
		uint32 mBitsPerPixel;
		uint32 mStorageSize;

		explicit operator bool() const noexcept
		{
			return mTexClass != ETextureClass::Unknown;
		}
	};

	struct TextureMipSubLevelInitDataDesc : public ResourceInputDataDesc
	{
		uint32 mMipWidth;
		uint32 mMipHeight;
		uint32 mMipDepth;
		uint32 mMipLevelIndex;
	};

	struct TextureSubTextureInitDataDesc
	{
		using MipLevelInitDataDescArray = std::array<TextureMipSubLevelInitDataDesc, GCM::cxTextureMaxMipLevelsNum>;
		MipLevelInitDataDescArray mMipLevelInitDataArray;
		uint32 mSubTextureIndex;
	};

	struct TextureInitDataDesc
	{
	public:
		TextureInitDataDesc() = default;

		TextureInitDataDesc( TextureInitDataDesc && pSrcObject );
		TextureInitDataDesc & operator=( TextureInitDataDesc && pRhs );

		TextureInitDataDesc( const TextureInitDataDesc & pSrcObject );
		TextureInitDataDesc & operator=( const TextureInitDataDesc & pRhs );

		using SubTextureInitDataDescArray = std::vector<TextureSubTextureInitDataDesc>;

		// Pointer to one or more TextureSubTextureInitDataDesc objects with init data for sub-textures.
		// Points to either _subTextureInitDataSingleTexture member or the beginning of the data in the
		// _subTextureInitDataTextureArray vector. This is an optimisation done for single textures
		// (arraySize=1) in order to prevent dynamic allocation of memory. In few cases where the engine
		// is used for allocating huge number of small non-array textures, this turned out to be an issue.
		TextureSubTextureInitDataDesc * subTextureInitDataBasePtr = nullptr;

		TBitmask<ETextureInitFlags> textureInitFlags = eTextureInitFlagGenerateMipmapsBit;

		explicit operator bool() const
		{
			return subTextureInitDataBasePtr != nullptr;
		}

		void initialize( const TextureDimensions & pDimensions );

		void swap( TextureInitDataDesc & pOther );

		IC3_ATTR_NO_DISCARD bool isArray() const;

		IC3_ATTR_NO_DISCARD bool empty() const;

		IC3_ATTR_NO_DISCARD TextureInitDataDesc copy() const;

	private:
		// Init data for single-layer (non-array) texture.
		TextureSubTextureInitDataDesc _subTextureInitData;
		
		// Vector of init data structs for array textures.
		SubTextureInitDataDescArray _subTextureInitDataArray;
	};

	struct TextureInputDataDesc : public ResourceInputDataDesc
	{
		EPixelDataLayout mPixelDataLayout;
		EBaseDataType mPixelDataType;
	};

	struct TextureCreateInfo : public ResourceCreateInfo
	{
		ETextureClass mTexClass;
		uint32 mMSAALevel = 0;
		TextureDimensions mDimensions{0, 0, 1, 1, 1 };
		ETextureFormat mInternalFormat;
		TextureInitDataDesc mInitDataDesc;

		explicit operator bool() const
		{
			return ( mTexClass != ETextureClass::Unknown ) && ( mInternalFormat != ETextureFormat::Unknown );
		}
	};

	struct TextureDataCopyDesc
	{
		ETextureClass mTexClass = ETextureClass::Unknown;
		TBitmask<ETextureDataCopyFlags> mFlags = eTextureDataCopyFlagsDefault;
	};

	struct TextureSubDataCopyDesc
	{
		ETextureClass mTexClass = ETextureClass::Unknown;
		TextureSubRegion mSourceTextureSubRegion;
		TextureOffset mTargetTextureOffset;
		TBitmask<ETextureDataCopyFlags> mFlags = eTextureDataCopyFlagsDefault;
	};

	struct TextureDataUploadDesc
	{
		ETextureClass mTexClass = ETextureClass::Unknown;
		TextureInputDataDesc mInputDataDesc;
		TBitmask<ETextureDataCopyFlags> mFlags = eTextureDataCopyFlagsDefault;
	};

	struct TextureSubDataUploadDesc
	{
		ETextureClass mTexClass = ETextureClass::Unknown;
		TextureSubRegion mTextureSubRegion;
		TextureInputDataDesc mInputDataDesc;
		TBitmask<ETextureDataCopyFlags> mFlags = eTextureDataCopyFlagsDefault;
	};

	namespace RCU
	{

		IC3_GRAPHICS_GCI_API_NO_DISCARD bool checkTextureFormatAndBindFlagsCompatibility(
				ETextureFormat pTextureFormat,
				TBitmask<resource_flags_value_t> pBindFlags );

		IC3_GRAPHICS_GCI_API_NO_DISCARD const TextureLayout & queryTextureLayout( TextureHandle pTexture ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD RenderTargetTextureLayout queryRenderTargetTextureLayoutForTexture( TextureHandle pTexture ) noexcept;

		IC3_GRAPHICS_GCI_API_NO_DISCARD RenderTargetTextureLayout queryRenderTargetTextureLayoutForTexture( const TextureLayout & pTextureLayout ) noexcept;

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_TEXTURE_COMMON_H__
