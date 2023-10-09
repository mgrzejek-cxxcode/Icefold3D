
#ifndef __IC3_NXMAIN_MATERIAL_COMMON_H__
#define __IC3_NXMAIN_MATERIAL_COMMON_H__

#include "../Prerequisites.h"
#include <Ic3/Graphics/GCI/Resources/textureCommon.h>

namespace Ic3
{

	enum EMaterialTextureID: uint32
	{
		Albedo,
		DiffuseMap,
		NormalMap,
		RoughnessMap,
		_Reserved
	};

	namespace CxDefs
	{

		constexpr uint32 MATERIAL_MAX_TEXTURES_NUM = static_cast<uint32>( EMaterialTextureID::_Reserved );

	}

	using MaterialTextureArray = std::array<GCI::TextureHandle, CxDefs::MATERIAL_MAX_TEXTURES_NUM>;

	struct MaterialParameters
	{
	};

	struct MaterialTextureSet
	{
	private:
		MaterialTextureArray _textureArray;

	public:
		const MaterialTextureArray & commonShaderArray;

		GCI::TextureHandle & albedoTexture;
		
		GCI::TextureHandle & diffuseMap;
		
		GCI::TextureHandle & normalMap;
		
		GCI::TextureHandle & roughnessMap;

	public:
		MaterialTextureSet();

		MaterialTextureSet( const MaterialTextureSet & pSource );

		explicit MaterialTextureSet( const MaterialTextureArray & pTextureArray );

		MaterialTextureSet & operator=( const MaterialTextureSet & pRhs );

		MaterialTextureSet & operator=( const MaterialTextureArray & pRhs );

		IC3_ATTR_NO_DISCARD GCI::Texture * operator[]( size_t pIndex ) const noexcept;

		IC3_ATTR_NO_DISCARD GCI::Texture * operator[]( EMaterialTextureID pTextureID ) const noexcept;

		IC3_ATTR_NO_DISCARD bool empty() const noexcept;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_MATERIAL_COMMON_H__
