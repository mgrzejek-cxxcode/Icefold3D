
#include "GraphicsShaderDefs.h"
#include <Ic3/Graphics/GCI/Resources/Shader.h>

namespace Ic3::Graphics::GCI
{

	void GraphicsShaderBinding::AddShader( Shader & pShader )
	{
		try
		{
			const auto shaderHandle = pShader.GetHandle<Shader>();

			AddShader( shaderHandle );
		}
		catch( const std::bad_weak_ptr & pHandleException )
		{
			// TODO: Error
			Ic3UnusedParam( pHandleException );
		}
	}

	void GraphicsShaderBinding::AddShader( ShaderHandle pShader )
	{
		if( pShader )
		{
			const auto stageIndex = CXU::SHGetShaderStageIndex( pShader->mShaderType );
			const auto stageBit = CXU::SHGetShaderStageBit( pShader->mShaderType );

			commonShaderArray[stageIndex] = pShader;

			if( !activeStagesMask.is_set( stageBit ) )
			{
				activeStagesMask.set( stageBit );
			}
		}
	}

	void GraphicsShaderBinding::SetShaders( const GraphicsShaderArray & pShaderArray )
	{
		activeStagesMask.clear();

		for( uint32 stageIndex = 0; stageIndex < GCM::kShaderGraphicsStagesNum; ++stageIndex )
		{
			const auto & shaderHandle = pShaderArray[stageIndex];
			const auto stageBit = CXU::SHGetShaderStageBit( shaderHandle->mShaderType );

			if( shaderHandle )
			{
				commonShaderArray[stageIndex] = shaderHandle;

				activeStagesMask.set( stageBit );
			}
			else
			{
				commonShaderArray[stageIndex].reset();
			}
		}
	}

	void GraphicsShaderBinding::Reset() noexcept
	{
		for( auto & shaderHandle : commonShaderArray )
		{
			shaderHandle.reset();
		}

		ResetActiveStagesInfo();
	}

	void GraphicsShaderBinding::ResetStage( uint32 pStageIndex ) noexcept
	{
		Ic3DebugAssert( CXU::SHIsGraphicsShaderStageIndexValid( pStageIndex ) );

		if( auto & shaderHandle = commonShaderArray[pStageIndex] )
		{
			const auto stageBit = CXU::SHGetShaderStageBit( shaderHandle->mShaderType );

			shaderHandle.reset();

			if( activeStagesMask.is_set( stageBit ) )
			{
				activeStagesMask.unset( stageBit );
			}
		}
	}

	void GraphicsShaderBinding::ResetStage( EShaderType pShaderType ) noexcept
	{
		Ic3DebugAssert( CXU::SHIsShaderTypeGraphics( pShaderType ) );
		const auto stageIndex = CXU::SHGetShaderStageIndex( pShaderType );
		ResetStage( stageIndex );
	}

	void GraphicsShaderBinding::UpdateActiveStagesInfo() noexcept
	{
		activeStagesMask.clear();

		for( const auto & shaderHandle : commonShaderArray )
		{
			if( shaderHandle )
			{
				const auto stageBit = CXU::SHGetShaderStageBit( shaderHandle->mShaderType );

				activeStagesMask.set( stageBit );
			}
		}
	}

	namespace Utilities
	{

		bool SHValidateGraphicsShaderArray( const GraphicsShaderArray & pShaderArray ) noexcept
		{
			for( uint32 stageIndex = 0; stageIndex < GCM::kShaderGraphicsStagesNum; ++stageIndex )
			{
				if( const auto & shaderHandle = pShaderArray[stageIndex] )
				{
					const auto expectedShaderType = CXU::SHGetShaderTypeFromStageIndex( stageIndex );

					if( shaderHandle->mShaderType != expectedShaderType )
					{
						return false;
					}
				}
			}

			return true;
		}

		bool SHValidateGraphicsShaderBinding( const GraphicsShaderBinding & pBindingConfiguration ) noexcept
		{
			for( uint32 stageIndex = 0; stageIndex < GCM::kShaderGraphicsStagesNum; ++stageIndex )
			{
				const auto & shaderHandle = pBindingConfiguration.commonShaderArray[stageIndex];
				const auto stageBit = CXU::SHMakeGraphicsShaderStageBit( stageIndex );

				if( shaderHandle )
				{
					const auto expectedShaderType = CXU::SHGetShaderTypeFromStageIndex( stageIndex );

					if( shaderHandle->mShaderType != expectedShaderType )
					{
						return false;
					}

					if( !pBindingConfiguration.activeStagesMask.is_set( stageBit ) )
					{
						return false;
					}
				}
				else
				{
					if( pBindingConfiguration.activeStagesMask.is_set( stageBit ) )
					{
						return false;
					}
				}
			}

			return true;
		}

	}

	namespace Utilities
	{

		cppx::bitmask<EShaderStageFlags> GetActiveShaderStagesMask( const GraphicsShaderArray & pShaderArray ) noexcept
		{
			cppx::bitmask<EShaderStageFlags> activeStagesMask = 0;

			for( uint32 stageIndex = 0; stageIndex < GCM::kShaderGraphicsStagesNum; ++stageIndex )
			{
				if( const auto shader = pShaderArray[stageIndex] )
				{
					const auto shaderTypeAtIndex = CXU::SHGetShaderTypeFromStageIndex( stageIndex );
					if( shader->mShaderType == shaderTypeAtIndex )
					{
						const auto shaderTypeBit = CXU::SHMakeShaderStageBit( stageIndex );
						activeStagesMask.set( shaderTypeBit );
					}
				}
			}

			return activeStagesMask;
		}

		uint32 GetActiveShaderStagesNum( const GraphicsShaderArray & pShaderArray ) noexcept
		{
			const auto activeStagesMask = GetActiveShaderStagesMask( pShaderArray );
			const auto activeStagesNum = pop_count( activeStagesMask );
			return activeStagesNum;
		}

	}

}
