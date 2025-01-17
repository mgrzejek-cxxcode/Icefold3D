
#pragma once

#ifndef __IC3_NXMAIN_SHADER_LOADER_H__
#define __IC3_NXMAIN_SHADER_LOADER_H__

#include "CommonRendererDefs.h"
#include <Ic3/Graphics/GCI/Resources/shaderCommon.h>
#include <Ic3/System/AssetCommon.h>

namespace Ic3
{

	using ShaderFileLoadCallback = std::function<dynamic_memory_buffer( const std::string & )>;
	using ShaderInputReadCallback = std::function<dynamic_memory_buffer()>;

	struct ShaderFileDefinition
	{
		std::string shaderName;
		GCI::EShaderType shaderType;
		std::string shaderFileName;
	};

	struct ShaderInputDefinition
	{
		std::string shaderName;
		GCI::EShaderType shaderType;
		ShaderInputReadCallback inputReadCallback;
	};

	class ShaderLoader : public CoreEngineObject
	{
	public:
		explicit ShaderLoader( const CoreEngineState & pCES );

		virtual ~ShaderLoader();

		GCI::ShaderHandle loadShader( const ShaderInputDefinition & pShaderDefinition );

		ShaderLibraryHandle loadShaders( std::initializer_list<ShaderInputDefinition> pShaderDefinitions );

		uint32 loadShaders(
				ShaderLibrary & pShaderLibrary,
				std::initializer_list<ShaderInputDefinition> pShaderDefinitions );

		static ShaderLibraryHandle createShaderLibrary(
				const CoreEngineState & pCES,
				std::initializer_list<ShaderInputDefinition> pShaderDefinitions );
	};

	class FileShaderLoader : public ShaderLoader
	{
	public:
		FileShaderLoader(
				const CoreEngineState & pCES,
				ShaderFileLoadCallback pFileLoadCallback );

		FileShaderLoader(
				const CoreEngineState & pCES,
				System::AssetLoaderHandle pAssetLoader,
				std::string pShaderBaseSubDirectory );

		virtual ~FileShaderLoader();

		GCI::ShaderHandle loadShader( const ShaderFileDefinition & pShaderDefinition );

		ShaderLibraryHandle loadShaders( std::initializer_list<ShaderFileDefinition> pShaderDefinitions );

		uint32 loadShaders(
				ShaderLibrary & pShaderLibrary,
				std::initializer_list<ShaderFileDefinition> pShaderDefinitions );

		static ShaderLibraryHandle createShaderLibrary(
				const CoreEngineState & pCES,
				ShaderFileLoadCallback pFileLoadCallback,
				std::initializer_list<ShaderFileDefinition> pShaderDefinitions );

		static ShaderLibraryHandle createShaderLibrary(
				const CoreEngineState & pCES,
				System::AssetLoaderHandle pAssetLoader,
				std::string pShaderBaseSubDirectory,
				std::initializer_list<ShaderFileDefinition> pShaderDefinitions );

	private:
		static ShaderFileLoadCallback bindShaderFileLoadCallback(
				System::AssetLoaderHandle pAssetLoader,
				std::string pShaderBaseSubDirectory );

		static dynamic_memory_buffer loadShaderFile( System::AssetLoader & pAssetLoader, const std::string & pFilename );

	private:
		ShaderFileLoadCallback _fileLoadCallback;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_SHADER_LOADER_H__
