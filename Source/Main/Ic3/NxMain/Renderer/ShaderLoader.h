
#pragma once

#ifndef __IC3_NXMAIN_SHADER_LOADER_H__
#define __IC3_NXMAIN_SHADER_LOADER_H__

#include "CommonRendererDefs.h"
#include <Ic3/Graphics/GCI/Resources/ShaderCommon.h>
#include <Ic3/System/IO/AssetCommon.h>

namespace Ic3
{

	Ic3DeclareClassHandle( ShaderLoader );
	Ic3DeclareClassHandle( ShaderLoaderMemoryBased );
	Ic3DeclareClassHandle( ShaderLoaderFileBased );

	using ShaderFileLoadCallback = std::function<cppx::dynamic_memory_buffer( const std::string & )>;
	using ShaderSourceReadCallback = std::function<cppx::dynamic_memory_buffer()>;

	enum class EShaderLoadDescType : enum_default_value_t
	{
		Unknown,
		LDFile,
		LDMemory,
	};

	/**
	 *
	 */
	struct ShaderLoadDescBase
    {
		EShaderLoadDescType descType = EShaderLoadDescType::Unknown;

	    GCI::EShaderType shaderType = GCI::EShaderType::Unknown;

	    cppx::immutable_string shaderName;

	    GfxObjectID shaderID = Graphics::kGfxObjectIDEmpty;

		explicit ShaderLoadDescBase( EShaderLoadDescType pDescType )
		: descType( pDescType )
		{}

		ShaderLoadDescBase(
			EShaderLoadDescType pDescType,
			GCI::EShaderType pShaderType,
			cppx::immutable_string pShaderName,
			GfxObjectID pShaderID = Graphics::kGfxObjectIDEmpty )
		: descType( pDescType )
		, shaderType( pShaderType )
		, shaderName( pShaderName )
		, shaderID( pShaderID )
		{}

	    void InitFromOther( const ShaderLoadDescBase & pOther )
	    {
		    *this = pOther;
    	}
    };

	#define Ic3ShaderLoadDescVS( pShaderName, pShaderID ) GCI::EShaderType::GSVertex,     pShaderName, pShaderID
	#define Ic3ShaderLoadDescVSAutoID( pShaderName )      GCI::EShaderType::GSVertex,     pShaderName, Graphics::kGfxObjectIDAuto
	#define Ic3ShaderLoadDescVSNoID( pShaderName )        GCI::EShaderType::GSVertex,     pShaderName, Graphics::kGfxObjectIDEmpty
	#define Ic3ShaderLoadDescHS( pShaderName, pShaderID ) GCI::EShaderType::GSTessHull,   pShaderName, pShaderID
	#define Ic3ShaderLoadDescHSAutoID( pShaderName )      GCI::EShaderType::GSTessHull,   pShaderName, Graphics::kGfxObjectIDAuto
	#define Ic3ShaderLoadDescHSNoID( pShaderName )        GCI::EShaderType::GSTessHull,   pShaderName, Graphics::kGfxObjectIDEmpty
	#define Ic3ShaderLoadDescDS( pShaderName, pShaderID ) GCI::EShaderType::GSTessDomain, pShaderName, pShaderID
	#define Ic3ShaderLoadDescDSAutoID( pShaderName )      GCI::EShaderType::GSTessDomain, pShaderName, Graphics::kGfxObjectIDAuto
	#define Ic3ShaderLoadDescDSNoID( pShaderName )        GCI::EShaderType::GSTessDomain, pShaderName, Graphics::kGfxObjectIDEmpty
	#define Ic3ShaderLoadDescGS( pShaderName, pShaderID ) GCI::EShaderType::GSGeometry,   pShaderName, pShaderID
	#define Ic3ShaderLoadDescGSAutoID( pShaderName )      GCI::EShaderType::GSGeometry,   pShaderName, Graphics::kGfxObjectIDAuto
	#define Ic3ShaderLoadDescGSNoID( pShaderName )        GCI::EShaderType::GSGeometry,   pShaderName, Graphics::kGfxObjectIDEmpty
	#define Ic3ShaderLoadDescPS( pShaderName, pShaderID ) GCI::EShaderType::GSPixel,      pShaderName, pShaderID
	#define Ic3ShaderLoadDescPSAutoID( pShaderName )      GCI::EShaderType::GSPixel,      pShaderName, Graphics::kGfxObjectIDAuto
	#define Ic3ShaderLoadDescPSNoID( pShaderName )        GCI::EShaderType::GSPixel,      pShaderName, Graphics::kGfxObjectIDEmpty

    /**
     *
     */
	struct ShaderLoadDescFile : public ShaderLoadDescBase
	{
		std::string shaderSourceFileName;

		ShaderLoadDescFile()
		: ShaderLoadDescBase( EShaderLoadDescType::LDFile )
		{}

		ShaderLoadDescFile(
				GCI::EShaderType pShaderType,
				cppx::immutable_string pShaderName,
				GfxObjectID pShaderID,
				std::string pShaderSourceFileName )
		: ShaderLoadDescBase( EShaderLoadDescType::LDFile, pShaderType, pShaderName, pShaderID )
		, shaderSourceFileName( std::move( pShaderSourceFileName ) )
		{}
	};

	/**
	 *
	 */
	struct ShaderLoadDescMemory : public ShaderLoadDescBase
	{
		ShaderSourceReadCallback shaderSourceReadCallback;

		ShaderLoadDescMemory()
		: ShaderLoadDescBase( EShaderLoadDescType::LDFile )
		{}

		ShaderLoadDescMemory(
				GCI::EShaderType pShaderType,
				cppx::immutable_string pShaderName,
				GfxObjectID pShaderID = Graphics::kGfxObjectIDEmpty )
		: ShaderLoadDescBase( EShaderLoadDescType::LDFile, pShaderType, pShaderName, pShaderID )
		{}
	};

	/**
	 *
	 */
	class IC3_NXMAIN_CLASS ShaderLoader : public CoreEngineObject
	{
	public:
		explicit ShaderLoader( const CoreEngineState & pCES );
		virtual ~ShaderLoader();

		virtual GCI::ShaderHandle LoadShader( const ShaderLoadDescBase & pShaderLoadDesc ) = 0;

		template <typename TPLoadDesc>
		std::vector<GCI::ShaderHandle> LoadShaders( std::initializer_list<TPLoadDesc> pShaderLoadDescList )
		{
			std::vector<GCI::ShaderHandle> shaderHandles{};
			if( pShaderLoadDescList.size() > 0 )
			{
				shaderHandles.reserve( pShaderLoadDescList.size() );
				for( const auto & shaderLoadDesc : pShaderLoadDescList )
				{
					if( const auto shaderObjectHandle = LoadShader( shaderLoadDesc ) )
					{
						shaderHandles.push_back( shaderObjectHandle );
					}
				}
			}

			return shaderHandles;
		}

		template<typename TPLoadDesc>
		bool LoadShaderIntoLibrary( ShaderLibrary & pShaderLibrary, const TPLoadDesc & pShaderLoadDesc )
		{
			if( const auto shaderObjectHandle = LoadShader( pShaderLoadDesc ) )
			{
				AddShaderToLibrary( pShaderLibrary, shaderObjectHandle, pShaderLoadDesc.shaderName );
				return true;
			}

			return false;
		}

		template<typename TPLoadDesc>
		uint32 LoadShadersIntoLibrary( ShaderLibrary & pShaderLibrary, std::initializer_list<TPLoadDesc> pShaderLoadDescList )
		{
			uint32 loadedShadersNum = 0;
			if( pShaderLoadDescList.size() > 0 )
			{
				for( const auto & shaderLoadDesc : pShaderLoadDescList )
				{
					if( LoadShaderIntoLibrary( pShaderLibrary, shaderLoadDesc ) )
					{
						loadedShadersNum += 1;
					}
				}
			}

			return loadedShadersNum;
		}

		template<typename TPLoadDesc>
		CPPX_ATTR_NO_DISCARD ShaderLibraryHandle LoadShaderIntoNewLibrary( const TPLoadDesc & pShaderLoadDesc )
		{
			ShaderLibraryHandle shaderLibrary = nullptr;
			if( const auto shaderObjectHandle = LoadShader( pShaderLoadDesc ) )
			{
				shaderLibrary = CreateNewShaderLibrary();
				AddShaderToLibrary( *shaderLibrary, shaderObjectHandle, pShaderLoadDesc.shaderName );
			}

			return shaderLibrary;
		}

		template<typename TPLoadDesc>
		CPPX_ATTR_NO_DISCARD ShaderLibraryHandle LoadShadersIntoNewLibrary( std::initializer_list<TPLoadDesc> pShaderLoadDescList )
		{
			ShaderLibraryHandle shaderLibrary = nullptr;
			if( pShaderLoadDescList.size() > 0 )
			{
				shaderLibrary = CreateNewShaderLibrary();
				for( const auto & shaderLoadDesc : pShaderLoadDescList )
				{
					if( const auto shaderObjectHandle = LoadShader( shaderLoadDesc ) )
					{
						AddShaderToLibrary( *shaderLibrary, shaderObjectHandle, shaderLoadDesc.shaderName );
					}
				}
			}

			return shaderLibrary;
		}

	protected:
		GCI::ShaderHandle CreateShaderImpl(
			GCI::EShaderType pShaderType,
			GfxObjectID pShaderID,
			const GfxObjectName & pShaderName,
			const cppx::dynamic_memory_buffer & pShaderSource );

		static bool ValidateShaderLoadDescBase( const ShaderLoadDescBase & pLoadDescBase );

	private:
		CPPX_ATTR_NO_DISCARD ShaderLibraryHandle CreateNewShaderLibrary() const;

		static void AddShaderToLibrary(
				ShaderLibrary & pShaderLibrary,
				GCI::ShaderHandle pShader,
				const cppx::immutable_string & pShaderName );
	};

	/**
	 *
	 */
	class IC3_NXMAIN_CLASS ShaderLoaderMemoryBased : public ShaderLoader
	{
	public:
		explicit ShaderLoaderMemoryBased( const CoreEngineState & pCES );
		virtual ~ShaderLoaderMemoryBased();

		virtual GCI::ShaderHandle LoadShader( const ShaderLoadDescBase & pShaderLoadDesc ) override final;

		CPPX_ATTR_NO_DISCARD static TSharedHandle<ShaderLoaderMemoryBased> CreateLoader( const CoreEngineState & pCES );

	protected:
		static const ShaderLoadDescMemory * ValidateShaderLoadDescMemory( const ShaderLoadDescBase & pLoadDescBase );
	};

	/**
	 *
	 */
	class IC3_NXMAIN_CLASS ShaderLoaderFileBased : public ShaderLoader
	{
	public:
		ShaderLoaderFileBased(
				const CoreEngineState & pCES,
				ShaderFileLoadCallback pFileLoadCallback );

		ShaderLoaderFileBased(
				const CoreEngineState & pCES,
				System::AssetLoaderHandle pAssetLoader,
				std::string pShaderBaseSubDirectory );

		virtual ~ShaderLoaderFileBased();

		virtual GCI::ShaderHandle LoadShader( const ShaderLoadDescBase & pShaderLoadDesc ) override final;

		CPPX_ATTR_NO_DISCARD static TSharedHandle<ShaderLoaderFileBased> CreateLoader(
				const CoreEngineState & pCES,
				ShaderFileLoadCallback pFileLoadCallback );

		CPPX_ATTR_NO_DISCARD static TSharedHandle<ShaderLoaderFileBased> CreateLoader(
				const CoreEngineState & pCES,
				System::AssetLoaderHandle pAssetLoader,
				std::string pShaderBaseSubDirectory );

	private:
		static ShaderFileLoadCallback BindShaderFileLoadCallback(
				System::AssetLoaderHandle pAssetLoader,
				std::string pShaderBaseSubDirectory );

		static const ShaderLoadDescFile * ValidateShaderLoadDescFile( const ShaderLoadDescBase & pLoadDescBase );

	private:
		ShaderFileLoadCallback _fileLoadCallback;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_SHADER_LOADER_H__
