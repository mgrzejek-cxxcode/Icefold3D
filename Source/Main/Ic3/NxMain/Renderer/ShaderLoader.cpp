
#include "ShaderLoader.h"
#include "ShaderLibrary.h"
#include <Ic3/Graphics/GCI/GPUUtils.h>
#include <Ic3/System/AssetSystem.h>

#include <Ic3/Graphics/GCI/Resources/Shader.h>
#include <Ic3/NxMain/GCI/ShaderUtils.h>

namespace Ic3
{

	ShaderLoader::ShaderLoader( const CoreEngineState & pCES )
	: CoreEngineObject( pCES )
	{}

	ShaderLoader::~ShaderLoader() = default;

	GCI::ShaderHandle ShaderLoader::CreateShaderImpl(
			GCI::EShaderType pShaderType,
			GfxObjectID pShaderID,
			const GfxObjectName & pShaderName,
			const cppx::dynamic_memory_buffer & pShaderSource )
	{
		GCI::ShaderHandle shaderObject = nullptr;

		if( !pShaderSource.empty() )
		{
			shaderObject = GCIUtils::CreateShaderFromSource(
					*( mCES.mGPUDevice ),
					pShaderType,
					pShaderSource.data(),
					pShaderSource.size() );
		}

		if( shaderObject )
		{
			if( pShaderID == Graphics::kGfxObjectIDAuto )
			{
				Ic3DebugAssert( !pShaderName.empty() );
				pShaderID = Graphics::GenerateGfxObjectID( pShaderName );
			}

			if( Graphics::IsGfxObjectIDValid( pShaderID ) )
			{
				shaderObject->SetObjectID( pShaderID );
			}
		}

		return shaderObject;
	}

	bool ShaderLoader::ValidateShaderLoadDescBase( const ShaderLoadDescBase & pLoadDescBase )
	{
		return
			( pLoadDescBase.shaderType != GCI::EShaderType::Unknown ) &&
			( !pLoadDescBase.shaderName.empty() || Graphics::IsGfxObjectIDValid( pLoadDescBase.shaderID ) );
	}

	ShaderLibraryHandle ShaderLoader::CreateNewShaderLibrary() const
	{
		return CreateDynamicObject<ShaderLibrary>( mCES );
	}

	void ShaderLoader::AddShaderToLibrary(
			ShaderLibrary & pShaderLibrary,
			GCI::ShaderHandle pShader,
			const cppx::immutable_string & pShaderName )
	{
		if( Graphics::IsGfxObjectIDValid( pShader->GetObjectID() ) )
		{
			pShaderLibrary.RegisterShader( pShader, pShader->GetObjectID() );
		}

		if( !pShaderName.empty() )
		{
			pShaderLibrary.RegisterShader( pShader, pShaderName );
		}
	}


	ShaderLoaderMemoryBased::ShaderLoaderMemoryBased( const CoreEngineState & pCES )
	: ShaderLoader( pCES )
	{}

	ShaderLoaderMemoryBased::~ShaderLoaderMemoryBased() = default;

	GCI::ShaderHandle ShaderLoaderMemoryBased::LoadShader( const ShaderLoadDescBase & pShaderLoadDesc )
	{
		if( const auto * loadDescMemory = ValidateShaderLoadDescMemory( pShaderLoadDesc ) )
		{
			auto shaderSource = loadDescMemory->shaderSourceReadCallback();
			if( !shaderSource.empty() )
			{
				return CreateShaderImpl(
						loadDescMemory->shaderType,
						loadDescMemory->shaderID,
						loadDescMemory->shaderName,
						shaderSource );
			}

		}

		return nullptr;
	}

	TSharedHandle<ShaderLoaderMemoryBased> ShaderLoaderMemoryBased::CreateLoader( const CoreEngineState & pCES )
	{
		return CreateDynamicObject<ShaderLoaderMemoryBased>( pCES );
	}

	const ShaderLoadDescMemory * ShaderLoaderMemoryBased::ValidateShaderLoadDescMemory( const ShaderLoadDescBase & pLoadDescBase )
	{
		if( pLoadDescBase.descType == EShaderLoadDescType::LDMemory )
		{
			const auto * loadDescMemory = reinterpret_cast<const ShaderLoadDescMemory *>( &pLoadDescBase );
			if( ValidateShaderLoadDescBase( pLoadDescBase ) && loadDescMemory->shaderSourceReadCallback )
			{
				return loadDescMemory;
			}
		}

		return nullptr;
	}


	ShaderLoaderFileBased::ShaderLoaderFileBased(
			const CoreEngineState & pCES,
			ShaderFileLoadCallback pFileLoadCallback )
	: ShaderLoader( pCES )
	, _fileLoadCallback( std::move( pFileLoadCallback ) )
	{}

	ShaderLoaderFileBased::ShaderLoaderFileBased(
			const CoreEngineState & pCES,
			System::AssetLoaderHandle pAssetLoader,
			std::string pShaderBaseSubDirectory )
	: ShaderLoader( pCES )
	, _fileLoadCallback( BindShaderFileLoadCallback( pAssetLoader, std::move( pShaderBaseSubDirectory ) ) )
	{}

	ShaderLoaderFileBased::~ShaderLoaderFileBased() = default;

	GCI::ShaderHandle ShaderLoaderFileBased::LoadShader( const ShaderLoadDescBase & pShaderLoadDesc )
	{
		if( const auto * loadDescFile = ValidateShaderLoadDescFile( pShaderLoadDesc ) )
		{
			auto shaderSource = _fileLoadCallback( loadDescFile->shaderSourceFileName );
			if( !shaderSource.empty() )
			{
				return CreateShaderImpl(
						loadDescFile->shaderType,
						loadDescFile->shaderID,
						loadDescFile->shaderName,
						shaderSource );
			}
		}

		return nullptr;
	}

	TSharedHandle<ShaderLoaderFileBased> ShaderLoaderFileBased::CreateLoader(
			const CoreEngineState & pCES,
			ShaderFileLoadCallback pFileLoadCallback )
	{
		return CreateDynamicObject<ShaderLoaderFileBased>( pCES, std::move( pFileLoadCallback ) );
	}

	TSharedHandle<ShaderLoaderFileBased> ShaderLoaderFileBased::CreateLoader(
			const CoreEngineState & pCES,
			System::AssetLoaderHandle pAssetLoader,
			std::string pShaderBaseSubDirectory )
	{
		return CreateDynamicObject<ShaderLoaderFileBased>( pCES, pAssetLoader, std::move( pShaderBaseSubDirectory ) );
	}

	ShaderFileLoadCallback ShaderLoaderFileBased::BindShaderFileLoadCallback(
			System::AssetLoaderHandle pAssetLoader,
			std::string pShaderBaseSubDirectory )
	{
		return [pAssetLoader, shaderSubDir = std::move( pShaderBaseSubDirectory )]( const auto & pShaderFileName ) {
				return System::AssetLoader::LoadAsset( *pAssetLoader, shaderSubDir + "/" + pShaderFileName, true );
			};
	}

	const ShaderLoadDescFile * ShaderLoaderFileBased::ValidateShaderLoadDescFile( const ShaderLoadDescBase & pLoadDescBase )
	{
		if( pLoadDescBase.descType == EShaderLoadDescType::LDFile )
		{
			const auto * loadDescFile = reinterpret_cast<const ShaderLoadDescFile *>( &pLoadDescBase );
			if( ValidateShaderLoadDescBase( pLoadDescBase ) && !loadDescFile->shaderSourceFileName.empty() )
			{
				return loadDescFile;
			}
		}

		return nullptr;
	}

} // namespace Ic3
