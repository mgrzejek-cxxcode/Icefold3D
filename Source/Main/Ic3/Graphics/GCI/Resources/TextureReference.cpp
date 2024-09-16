
#include "TextureReference.h"
#include "Texture.h"

namespace Ic3::Graphics::GCI
{

	TextureReference::TextureReference( TextureHandle pTexture )
	{
		setRefTexture( pTexture );
	}

	TextureReference::TextureReference( TextureHandle pTexture, const TextureSubResource & pReferencedSubResource )
	{
		setRefTexture( pTexture, pReferencedSubResource );
	}

	TextureReference::~TextureReference() = default;

	bool TextureReference::setRefTexture( TextureHandle pTexture )
	{
		return _setRefTextureInternal( pTexture, pTexture->getDefaultSubResourceRef() );
	}

	bool TextureReference::setRefTexture( TextureHandle pTexture, const TextureSubResource & pReferencedSubResource )
	{
		return _setRefTextureInternal( pTexture, pReferencedSubResource );
	}

	bool TextureReference::reset()
	{
		return _setRefTextureInternal( nullptr );
	}

	bool TextureReference::_setRefTextureInternal( TextureHandle pTexture, const TextureSubResource & pReferencedSubResource )
	{
		bool referenceUpdated = false;

		// If an empty texture handle is specified, reset this reference object: the texture gets cleared (and its
		// active use counter decremented by one), texture info and referenced region are both reset to default.
		if( !pTexture )
		{
			// Of course, assuming there is something to reset in the first place.
			if( _refTextureObject )
			{
				_refTextureObject->releaseActiveRef();
				_refTextureObject = nullptr;
				_refTextureFormat = ETextureFormat::Unknown;
				_refSubResource.reset();

				referenceUpdated = true;
			}
		}
		// Otherwise, check if there is anything to update: either the texture is a
		// different texture or the referenced subresource is different from the current one.
		else if( ( pTexture != _refTextureObject ) || !RCU::cmpEqTextureSubResource( pReferencedSubResource, _refSubResource ) )
		{
			// Validate if the specified subresource is valid for the given texture object.
			if( RCU::checkTextureSubResource( pTexture, pReferencedSubResource ) )
			{
				// Update the texture only if necessary.
				if( pTexture != _refTextureObject )
				{
					// This can be null (empty reference).
					if( _refTextureObject )
					{
						_refTextureObject->releaseActiveRef();
						_refTextureObject = nullptr;
					}

					_refTextureObject = pTexture;
					_refTextureObject->addActiveRef();
					_refTextureFormat = pTexture->mTextureLayout.mInternalFormat;
				}

				// Update the referenced subresource.
				_refSubResource = pReferencedSubResource;

				referenceUpdated = true;
			}
		}

		return referenceUpdated;
	}

}
