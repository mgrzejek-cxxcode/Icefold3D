
#pragma once

#ifndef __IC3_GRAPHICS_GCI_TEXTURE_REFERENCE_H__
#define __IC3_GRAPHICS_GCI_TEXTURE_REFERENCE_H__

#include "TextureCommon.h"

namespace Ic3::Graphics::GCI
{

	class IC3_GRAPHICS_GCI_CLASS TextureReference
	{
	public:
		TextureReference( const TextureReference & ) = default;
		TextureReference & operator=( const TextureReference & ) = default;

		TextureReference() = default;
		TextureReference( TextureHandle pTexture );
		TextureReference( TextureHandle pTexture, const TextureSubResource & pReferencedSubResource );

		~TextureReference();

		Texture * operator->() const noexcept;

		explicit operator bool() const noexcept;

		/// @brief Returns a pointer to the referenced texture. This can be null if the reference is empty().
		CPPX_ATTR_NO_DISCARD TextureHandle GetRefTexture() const noexcept;

		/// @brief Returns a pointer to the referenced texture. This can be null if the reference is empty().
		CPPX_ATTR_NO_DISCARD ETextureFormat GetRefTextureFormat() const noexcept;

		/// @brief
		CPPX_ATTR_NO_DISCARD const TextureSubResource & GetRefSubResource() const noexcept;

		/// @brief Returns true if this instance represents a valid binding.
		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept;

		/// @brief
		CPPX_ATTR_NO_DISCARD bool IsValid() const noexcept;

		/// @brief
		bool SetRefTexture( TextureHandle pTexture );

		/// @brief
		bool SetRefTexture( TextureHandle pTexture, const TextureSubResource & pReferencedSubResource );

		/// @brief
		bool Reset();

	private:
		bool _SetRefTextureInternal( TextureHandle pTexture, const TextureSubResource & pReferencedSubResource = {} );

	private:
		/// A handle to the referenced texture object. If the reference is empty, the handle is null.
		TextureHandle _refTextureObject = nullptr;

		/// The format of the referenced texture. If the reference is empty, the format is ETextureFormat::UNKNOWN.
		ETextureFormat _refTextureFormat = ETextureFormat::Undefined;

		/// The referenced sub-resource of the texture.
		TextureSubResource _refSubResource;
	};

	inline Texture * TextureReference::operator->() const noexcept
	{
		return _refTextureObject.get();
	}

	inline TextureReference::operator bool() const noexcept
	{
		return !IsEmpty();
	}

	inline TextureHandle TextureReference::GetRefTexture() const noexcept
	{
		return _refTextureObject;
	}

	inline ETextureFormat TextureReference::GetRefTextureFormat() const noexcept
	{
		return _refTextureFormat;
	}

	inline const TextureSubResource & TextureReference::GetRefSubResource() const noexcept
	{
		return _refSubResource;
	}

	inline bool TextureReference::IsEmpty() const noexcept
	{
		return !_refTextureObject || !_refSubResource;
	}

	inline bool TextureReference::IsValid() const noexcept
	{
		return _refTextureObject && RCU::CheckTextureSubResource( _refTextureObject, _refSubResource );
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_TEXTURE_REFERENCE_H__
