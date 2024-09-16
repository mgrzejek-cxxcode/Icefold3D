
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
		IC3_ATTR_NO_DISCARD TextureHandle getRefTexture() const noexcept;

		/// @brief Returns a pointer to the referenced texture. This can be null if the reference is empty().
		IC3_ATTR_NO_DISCARD ETextureFormat getRefTextureFormat() const noexcept;

		/// @brief
		IC3_ATTR_NO_DISCARD const TextureSubResource & getRefSubResource() const noexcept;

		/// @brief Returns true if this instance represents a valid binding.
		IC3_ATTR_NO_DISCARD bool empty() const noexcept;

		/// @brief
		IC3_ATTR_NO_DISCARD bool valid() const noexcept;

		/// @brief
		bool setRefTexture( TextureHandle pTexture );

		/// @brief
		bool setRefTexture( TextureHandle pTexture, const TextureSubResource & pReferencedSubResource );

		/// @brief
		bool reset();

	private:
		bool _setRefTextureInternal( TextureHandle pTexture, const TextureSubResource & pReferencedSubResource = {} );

	private:
		/// A handle to the referenced texture object. If the reference is empty, the handle is null.
		TextureHandle _refTextureObject = nullptr;

		/// The format of the referenced texture. If the reference is empty, the format is ETextureFormat::UNKNOWN.
		ETextureFormat _refTextureFormat = ETextureFormat::Unknown;

		/// The referenced sub-resource of the texture.
		TextureSubResource _refSubResource;
	};

	inline Texture * TextureReference::operator->() const noexcept
	{
		return _refTextureObject.get();
	}

	inline TextureReference::operator bool() const noexcept
	{
		return !empty();
	}

	inline TextureHandle TextureReference::getRefTexture() const noexcept
	{
		return _refTextureObject;
	}

	inline ETextureFormat TextureReference::getRefTextureFormat() const noexcept
	{
		return _refTextureFormat;
	}

	inline const TextureSubResource & TextureReference::getRefSubResource() const noexcept
	{
		return _refSubResource;
	}

	inline bool TextureReference::empty() const noexcept
	{
		return !_refTextureObject || !_refSubResource;
	}

	inline bool TextureReference::valid() const noexcept
	{
		return _refTextureObject && RCU::checkTextureSubResource( _refTextureObject, _refSubResource );
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_TEXTURE_REFERENCE_H__
