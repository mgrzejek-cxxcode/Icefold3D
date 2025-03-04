
#include "TextureReference.h"
#include "Texture.h"

namespace Ic3::Graphics::GCI
{

	TextureReference::TextureReference( TextureHandle pTexture )
	{
		SetRefTexture( pTexture );
	}

	TextureReference::TextureReference( TextureHandle pTexture, const TextureSubResource & pReferencedSubResource )
	{
		SetRefTexture( pTexture, pReferencedSubResource );
	}

	TextureReference::~TextureReference() = default;

	bool TextureReference::SetRefTexture( TextureHandle pTexture )
	{
		return _SetRefTextureInternal( pTexture, pTexture->GetDefaultSubResourceRef() );
	}

	bool TextureReference::SetRefTexture( TextureHandle pTexture, const TextureSubResource & pReferencedSubResource )
	{
		return _SetRefTextureInternal( pTexture, pReferencedSubResource );
	}

	bool TextureReference::Reset()
	{
		return _SetRefTextureInternal( nullptr );
	}

	bool TextureReference::_SetRefTextureInternal( TextureHandle pTexture, const TextureSubResource & pReferencedSubResource )
	{
		bool referenceUpdated = false;

		// If an empty texture handle is specified, reset this reference object: the texture gets cleared (and its
		// active use counter decremented by one), texture info and referenced region are both reset to default.
		if( !pTexture )
		{
			// Of course, assuming there is something to reset in the first place.
			if( _refTextureObject )
			{
				_refTextureObject->ReleaseActiveRef();
				_refTextureObject = nullptr;
				_refTextureFormat = ETextureFormat::Undefined;
				_refSubResource.Reset();

				referenceUpdated = true;
			}
		}
		// Otherwise, check if there is anything to update: either the texture is a
		// different texture or the referenced subresource is different from the current one.
		else if( ( pTexture != _refTextureObject ) || !RCU::CmpEqTextureSubResource( pReferencedSubResource, _refSubResource ) )
		{
			// Validate if the specified subresource is valid for the given texture object.
			if( RCU::CheckTextureSubResource( pTexture, pReferencedSubResource ) )
			{
				// Update the texture only if necessary.
				if( pTexture != _refTextureObject )
				{
					// This can be null (empty reference).
					if( _refTextureObject )
					{
						_refTextureObject->ReleaseActiveRef();
						_refTextureObject = nullptr;
					}

					_refTextureObject = pTexture;
					_refTextureObject->AddActiveRef();
					_refTextureFormat = pTexture->mTextureLayout.internalFormat;
				}

				// Update the referenced subresource.
				_refSubResource = pReferencedSubResource;

				referenceUpdated = true;
			}
		}

		return referenceUpdated;
	}

}
