
#pragma once

#ifndef __IC3_NXMAIN_SHADER_LIBRARY_H__
#define __IC3_NXMAIN_SHADER_LIBRARY_H__

#include "CommonRendererDefs.h"
#include <Ic3/Graphics/GCI/Resources/shaderCommon.h>
#include <unordered_map>

namespace Ic3
{

	class ShaderLibrary : public CoreEngineObject
	{
	public:
		explicit ShaderLibrary( const CoreEngineState & pCES );
		~ShaderLibrary();

		IC3_ATTR_NO_DISCARD bool empty() const noexcept;

		IC3_ATTR_NO_DISCARD GCI::ShaderHandle getShader( GpaUniqueObjectID pShaderID ) const noexcept;

		IC3_ATTR_NO_DISCARD GCI::ShaderHandle getShader( const GpaUniqueObjectName & pShaderName ) const noexcept;

		uint32 append( const ShaderLibrary & pOtherLibrary );

		bool registerShader( GpaUniqueObjectID pShaderID, GCI::ShaderHandle pShaderObject );

		bool registerShader( const GpaUniqueObjectName & pShaderName, GCI::ShaderHandle pShaderObject );

	private:
		std::unordered_map<GpaUniqueObjectID, GCI::ShaderHandle> _shaderMap;
	};

	inline bool ShaderLibrary::empty() const noexcept
	{
		return _shaderMap.empty();
	}

} // namespace Ic3

#endif // __IC3_NXMAIN_SHADER_LIBRARY_H__
