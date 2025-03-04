
#pragma once

#ifndef __IC3_NXMAIN_SHADER_LIBRARY_H__
#define __IC3_NXMAIN_SHADER_LIBRARY_H__

#include "CommonRendererDefs.h"
#include <Ic3/Graphics/GCI/Resources/ShaderCommon.h>
#include <unordered_map>

namespace Ic3
{

	class ShaderLibrary : public CoreEngineObject
	{
	public:
		explicit ShaderLibrary( const CoreEngineState & pCES );
		~ShaderLibrary();

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept;

		CPPX_ATTR_NO_DISCARD GCI::ShaderHandle GetShader( GfxObjectID pShaderID ) const noexcept;

		CPPX_ATTR_NO_DISCARD GCI::ShaderHandle GetShader( const GfxObjectName & pShaderName ) const noexcept;

		uint32 Append( const ShaderLibrary & pOtherLibrary, bool pOverwriteExisting = false );

		bool RegisterShader( GCI::ShaderHandle pShaderObject, GfxObjectID pShaderID, bool pOverwriteExisting = false );

		bool RegisterShader( GCI::ShaderHandle pShaderObject, const GfxObjectName & pShaderName, bool pOverwriteExisting = false );

		bool RegisterShader( GCI::ShaderHandle pShaderObject, GfxObjectID pShaderID, const GfxObjectName & pShaderName, bool pOverwriteExisting = false );

	private:
		std::unordered_map<GfxObjectID, GCI::ShaderHandle> _shaderMapByID;
		std::unordered_map<GfxObjectName, GCI::ShaderHandle> _shaderMapByName;
	};

	inline bool ShaderLibrary::IsEmpty() const noexcept
	{
		return _shaderMapByID.empty() && _shaderMapByName.empty();
	}

} // namespace Ic3

#endif // __IC3_NXMAIN_SHADER_LIBRARY_H__
