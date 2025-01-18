
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

		CPPX_ATTR_NO_DISCARD bool empty() const noexcept;

		CPPX_ATTR_NO_DISCARD GCI::ShaderHandle getShader( GfxObjectID pShaderID ) const noexcept;

		CPPX_ATTR_NO_DISCARD GCI::ShaderHandle getShader( const GfxObjectName & pShaderName ) const noexcept;

		uint32 append( const ShaderLibrary & pOtherLibrary );

		bool registerShader( GfxObjectID pShaderID, GCI::ShaderHandle pShaderObject );

		bool registerShader( const GfxObjectName & pShaderName, GCI::ShaderHandle pShaderObject );

	private:
		std::unordered_map<GfxObjectID, GCI::ShaderHandle> _shaderMap;
	};

	inline bool ShaderLibrary::empty() const noexcept
	{
		return _shaderMap.empty();
	}

} // namespace Ic3

#endif // __IC3_NXMAIN_SHADER_LIBRARY_H__
