
#include "GLAPITranslationLayer.h"
#include <Ic3/Graphics/GCI/Resources/GPUBufferCommon.h>
#include <Ic3/Graphics/GCI/Resources/ShaderCommon.h>
#include <Ic3/Graphics/GCI/Resources/TextureCommon.h>
#include <Ic3/Graphics/GCI/State/SamplerCommon.h>
#include <cppx/stdHelperAlgo.h>
#include <unordered_map>

namespace Ic3::Graphics::GCI
{
#if( IC3_GX_GL_TARGET == IC3_GX_GL_TARGET_GL43 )
	GLenum ATL::ChooseGLBufferStorageFlags( GLenum pBindTarget, cppx::bitmask<resource_flags_value_t> pBufferFlags, cppx::bitmask<EGPUMemoryFlags> pMemoryFlags )
	{
		cppx::bitmask<GLenum> storageFlags = 0;

		if( !pBufferFlags.is_set( eGPUResourceContentFlagImmutableBit ) )
		{
			// DYNAMIC_STORAGE is required in order to perform *ANY* kind of buffer data update.
			// Without this flag, it is impossible to change the buffer's content even via glBufferSubData.
			// Because of that, we Apply this to every buffer, except those specified explicitly as IMMUTABLE.
			storageFlags.set( GL_DYNAMIC_STORAGE_BIT );
		}
		if( pBufferFlags.is_set( eGPUResourceContentFlagTemporaryBit ) )
		{
			storageFlags.set( GL_CLIENT_STORAGE_BIT );
		}
		if( pMemoryFlags.is_set( eGPUMemoryAccessFlagCPUReadBit ) )
		{
			storageFlags.set( GL_MAP_READ_BIT );
		}
		if( pMemoryFlags.is_set( eGPUMemoryAccessFlagCPUWriteBit ) )
		{
			storageFlags.set( GL_MAP_WRITE_BIT );
		}
		if( pMemoryFlags.is_set( eGPUMemoryHeapPropertyFlagCPUCachedBit ) )
		{
			storageFlags.set( GL_CLIENT_STORAGE_BIT );
		}
		if( pMemoryFlags.is_set( eGPUMemoryHeapPropertyFlagPersistentMapBit ) )
		{
			storageFlags.set( GL_MAP_PERSISTENT_BIT );
			if( pMemoryFlags.is_set_any_of(
					eGPUMemoryHeapPropertyFlagCPUCoherentBit | eGPUMemoryHeapPropertyFlagGPUCoherentBit ) )
			{
				storageFlags.set( GL_MAP_COHERENT_BIT );
			}
		}

		return storageFlags;
	}
#endif

	GLenum ATL::ChooseGLBufferUsagePolicy( GLenum pBindTarget, cppx::bitmask<resource_flags_value_t> pBufferFlags )
	{
		// Usage constants have standard values. Use that fact to avoid huge if-else.
		// Basically, we first select between DYNAMIC, STREAM and STATIC and then advance
		// selected value according to buffer bind target (i.e. how the data will be used).
		//
		// GL_STREAM_DRAW  = 0x88E0
		// GL_STREAM_READ  = 0x88E1
		// GL_STREAM_COPY  = 0x88E2
		// GL_STATIC_DRAW  = 0x88E4
		// GL_STATIC_READ  = 0x88E5
		// GL_STATIC_COPY  = 0x88E6
		// GL_DYNAMIC_DRAW = 0x88E8
		// GL_DYNAMIC_READ = 0x88E9
		// GL_DYNAMIC_COPY = 0x88EA

		GLenum usagePolicy = 0;

		// The nature of the data has been explicitly specified as dynamic.
		if( pBufferFlags.is_set( eGPUResourceContentFlagDynamicBit ) )
		{
			usagePolicy = GL_DYNAMIC_DRAW;
		}
		// Temporary buffers, updated once and used few times. That fits with STREAM policy.
		else if( pBufferFlags.is_set( eGPUResourceContentFlagTemporaryBit ) )
		{
			usagePolicy = GL_STREAM_DRAW;
		}
		// The most common usage will be a static buffer (usually a VBO/IBO), updated from time to time and used for drawing.
		else
		{
			usagePolicy = GL_STATIC_DRAW;
		}

		if( ( pBindTarget == GL_COPY_READ_BUFFER ) || ( pBindTarget == GL_TRANSFORM_FEEDBACK_BUFFER ) )
		{
			// Advance from GL_XXX_DRAW to GL_XXX_READ
			usagePolicy += 1;
		}
		else if( ( pBindTarget == GL_COPY_WRITE_BUFFER ) || ( pBindTarget == GL_PIXEL_UNPACK_BUFFER ) )
		{
			// Advance from GL_XXX_DRAW to GL_XXX_COPY
			usagePolicy += 2;
		}

		return usagePolicy;
	}

	GLenum ATL::ChooseGLTextureMagFilter( ETextureFilter pMagFilter, ETextureMipMode pMipMode )
	{
		GLenum magFilter = GL_INVALID_VALUE;

		if( pMagFilter == ETextureFilter::Point )
		{
			magFilter = GL_NEAREST;
		}
		else if( pMagFilter == ETextureFilter::Linear )
		{
			magFilter = GL_LINEAR;
		}
		else if( pMagFilter == ETextureFilter::Anisotropic )
		{
			magFilter = GL_LINEAR;
		}

		return magFilter;
	}

	GLenum ATL::ChooseGLTextureMinFilter( ETextureFilter pMinFilter, ETextureMipMode pMipMode )
	{
		GLenum minFilter = GL_INVALID_VALUE;

		if( pMinFilter == ETextureFilter::Point )
		{
			if( pMipMode == ETextureMipMode::Disable )
			{
				minFilter = GL_NEAREST;
			}
			else if( pMipMode == ETextureMipMode::Nearest )
			{
				minFilter = GL_NEAREST_MIPMAP_NEAREST;
			}
			else if( pMipMode == ETextureMipMode::Linear )
			{
				minFilter = GL_NEAREST_MIPMAP_LINEAR;
			}
		}
		else if( pMinFilter == ETextureFilter::Linear )
		{
			if( pMipMode == ETextureMipMode::Disable )
			{
				minFilter = GL_LINEAR;
			}
			else if( pMipMode == ETextureMipMode::Nearest )
			{
				minFilter = GL_LINEAR_MIPMAP_NEAREST;
			}
			else if( pMipMode == ETextureMipMode::Linear )
			{
				minFilter = GL_LINEAR_MIPMAP_LINEAR;
			}
		}
		else if( pMinFilter == ETextureFilter::Anisotropic )
		{
			if( pMipMode == ETextureMipMode::Disable )
			{
				minFilter = GL_LINEAR;
			}
			else if( pMipMode == ETextureMipMode::Nearest )
			{
				minFilter = GL_LINEAR_MIPMAP_NEAREST;
			}
			else if( pMipMode == ETextureMipMode::Linear )
			{
				minFilter = GL_LINEAR_MIPMAP_LINEAR;
			}
		}

		return minFilter;
	}

	GLsizei ATL::QueryGLTextureInternalFormatBPP( GLenum pGLInternalFormat )
	{
		switch( pGLInternalFormat )
		{
			Ic3CaseReturn( GL_R32F         , 32  );
			Ic3CaseReturn( GL_R32I         , 32  );
			Ic3CaseReturn( GL_R32UI        , 32  );
			Ic3CaseReturn( GL_RG32F        , 64  );
			Ic3CaseReturn( GL_RG32I        , 64  );
			Ic3CaseReturn( GL_RG32UI       , 64  );
			Ic3CaseReturn( GL_RGB32F       , 96  );
			Ic3CaseReturn( GL_RGB32I       , 96  );
			Ic3CaseReturn( GL_RGB32UI      , 96  );
			Ic3CaseReturn( GL_RGBA32F      , 128 );
			Ic3CaseReturn( GL_RGBA32I      , 128 );
			Ic3CaseReturn( GL_RGBA32UI     , 128 );
			Ic3CaseReturn( GL_R16F         , 16  );
			Ic3CaseReturn( GL_R16I         , 16  );
			Ic3CaseReturn( GL_R16UI        , 16  );
			Ic3CaseReturn( GL_RG16F        , 32  );
			Ic3CaseReturn( GL_RG16I        , 32  );
			Ic3CaseReturn( GL_RG16UI       , 32  );
			Ic3CaseReturn( GL_RGBA16F      , 64  );
			Ic3CaseReturn( GL_RGBA16I      , 64  );
			Ic3CaseReturn( GL_RGBA16UI     , 64  );
			Ic3CaseReturn( GL_R8I          , 8   );
			Ic3CaseReturn( GL_R8UI         , 8   );
			Ic3CaseReturn( GL_R8_SNORM     , 8   );
			Ic3CaseReturn( GL_R8           , 8   );
			Ic3CaseReturn( GL_RG8I         , 16  );
			Ic3CaseReturn( GL_RG8UI        , 16  );
			Ic3CaseReturn( GL_RG8_SNORM    , 16  );
			Ic3CaseReturn( GL_RG8          , 16  );
			Ic3CaseReturn( GL_RGB8         , 24  );
			Ic3CaseReturn( GL_SRGB8        , 24  );
			Ic3CaseReturn( GL_RGBA8        , 32  );
			Ic3CaseReturn( GL_SRGB8_ALPHA8 , 32  );
			Ic3CaseReturn( GL_RGBA8I       , 32  );
			Ic3CaseReturn( GL_RGBA8UI      , 32  );
			Ic3CaseReturn( GL_RGBA8_SNORM  , 32  );

			Ic3CaseReturn( GL_RGB5_A1            , 16 );
			Ic3CaseReturn( GL_RGB565             , 16 );
			Ic3CaseReturn( GL_RGB9_E5            , 32 );
			Ic3CaseReturn( GL_RGB10_A2UI         , 32 );
			Ic3CaseReturn( GL_RGB10_A2           , 32 );
			Ic3CaseReturn( GL_R11F_G11F_B10F     , 32 );
			Ic3CaseReturn( GL_DEPTH_COMPONENT16  , 16 );
			Ic3CaseReturn( GL_DEPTH24_STENCIL8   , 32 );
			Ic3CaseReturn( GL_DEPTH_COMPONENT24  , 24 );
			Ic3CaseReturn( GL_DEPTH_COMPONENT32F , 32 );
			Ic3CaseReturn( GL_DEPTH32F_STENCIL8  , 64 );

		#if( IC3_GX_GL_PLATFORM_TYPE == IC3_GX_GL_PLATFORM_TYPE_DESKTOP )
			Ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC1        , 4 );
			Ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC1_SRGBA  , 4 );
			Ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC2        , 4 );
			Ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC2_SRGB   , 8 );
			Ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC3        , 8 );
			Ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC3_SRGB   , 8 );
			Ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC4_SNORM  , 8 );
			Ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC4_UNORM  , 8 );
			Ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC5_SNORM  , 8 );
			Ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC5_UNORM  , 0 );
			Ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC6H_SFLOAT, 0 );
			Ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC6H_UFLOAT, 0 );
			Ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC7        , 0 );
			Ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC7_SRGB   , 0 );
		#endif
		}
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::TranslateGLBaseDataType( EBaseDataType pBaseDataType )
	{
		switch( pBaseDataType )
		{
			Ic3CaseReturn( EBaseDataType::Undefined, GL_INVALID_ENUM      );
			Ic3CaseReturn( EBaseDataType::Byte     , GL_BYTE              );
			Ic3CaseReturn( EBaseDataType::Ubyte    , GL_UNSIGNED_BYTE     );
			Ic3CaseReturn( EBaseDataType::Int16    , GL_SHORT             );
			Ic3CaseReturn( EBaseDataType::Uint16   , GL_UNSIGNED_SHORT    );
			Ic3CaseReturn( EBaseDataType::Int32    , GL_INT               );
			Ic3CaseReturn( EBaseDataType::Uint32   , GL_UNSIGNED_INT      );
			Ic3CaseReturn( EBaseDataType::Float16  , GL_HALF_FLOAT        );
			Ic3CaseReturn( EBaseDataType::Float32  , GL_FLOAT             );
			Ic3CaseReturn( EBaseDataType::Uint24S8 , GL_UNSIGNED_INT_24_8 );
		}
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::TranslateGLBlendFactor( EBlendFactor pBlendFactor )
	{
		switch( pBlendFactor )
		{
			Ic3CaseReturn( EBlendFactor::Undefined   , GL_INVALID_ENUM             );
			Ic3CaseReturn( EBlendFactor::Zero        , GL_ZERO                     );
			Ic3CaseReturn( EBlendFactor::One         , GL_ONE                      );
			Ic3CaseReturn( EBlendFactor::Const       , GL_CONSTANT_COLOR           );
			Ic3CaseReturn( EBlendFactor::ConstInv    , GL_ONE_MINUS_CONSTANT_COLOR );
			Ic3CaseReturn( EBlendFactor::SrcColor    , GL_SRC_COLOR                );
			Ic3CaseReturn( EBlendFactor::SrcAlpha    , GL_SRC_ALPHA                );
			Ic3CaseReturn( EBlendFactor::DstColor    , GL_DST_COLOR                );
			Ic3CaseReturn( EBlendFactor::DstAlpha    , GL_DST_ALPHA                );
			Ic3CaseReturn( EBlendFactor::SrcColorInv , GL_ONE_MINUS_SRC_COLOR      );
			Ic3CaseReturn( EBlendFactor::SrcAlphaInv , GL_ONE_MINUS_SRC_ALPHA      );
			Ic3CaseReturn( EBlendFactor::DstColorInv , GL_ONE_MINUS_DST_COLOR      );
			Ic3CaseReturn( EBlendFactor::DstAlphaInv , GL_ONE_MINUS_DST_ALPHA      );
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::TranslateGLBlendOp( EBlendOp pBlendOp )
	{
		switch( pBlendOp )
		{
			Ic3CaseReturn( EBlendOp::Undefined   , GL_INVALID_ENUM          );
			Ic3CaseReturn( EBlendOp::Add         , GL_FUNC_ADD              );
			Ic3CaseReturn( EBlendOp::Min         , GL_MIN                   );
			Ic3CaseReturn( EBlendOp::Max         , GL_MAX                   );
			Ic3CaseReturn( EBlendOp::Subtract    , GL_FUNC_SUBTRACT         );
			Ic3CaseReturn( EBlendOp::SubtractRev , GL_FUNC_REVERSE_SUBTRACT );
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::TranslateGLBufferBindTarget( EGPUBufferTarget pBufferTarget )
	{
		switch( pBufferTarget )
		{
			Ic3CaseReturn( EGPUBufferTarget::Unknown                , GL_INVALID_ENUM              );
			Ic3CaseReturn( EGPUBufferTarget::ConstantBuffer         , GL_UNIFORM_BUFFER            );
			Ic3CaseReturn( EGPUBufferTarget::VertexBuffer           , GL_ARRAY_BUFFER              );
			Ic3CaseReturn( EGPUBufferTarget::IndexBuffer            , GL_ELEMENT_ARRAY_BUFFER      );
			Ic3CaseReturn( EGPUBufferTarget::ShaderInputBuffer      , GL_SHADER_STORAGE_BUFFER     );
			Ic3CaseReturn( EGPUBufferTarget::ShaderUAVBuffer        , GL_SHADER_STORAGE_BUFFER     );
			Ic3CaseReturn( EGPUBufferTarget::StreamOutputBuffer     , GL_TRANSFORM_FEEDBACK_BUFFER );
			Ic3CaseReturn( EGPUBufferTarget::IndirectDispatchBuffer , GL_DISPATCH_INDIRECT_BUFFER  );
			Ic3CaseReturn( EGPUBufferTarget::IndirectDrawBuffer     , GL_DRAW_INDIRECT_BUFFER      );
			Ic3CaseReturn( EGPUBufferTarget::TransferSourceBuffer   , GL_COPY_READ_BUFFER          );
			Ic3CaseReturn( EGPUBufferTarget::TransferTargetBuffer   , GL_COPY_WRITE_BUFFER         );
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::TranslateGLBufferMapFlags( EGPUMemoryMapMode pMapMode, cppx::bitmask<EGPUMemoryFlags> pMemoryFlags )
	{
		cppx::bitmask<uint32> resourceMapFlags = static_cast<uint32>( pMapMode );
		cppx::bitmask<GLenum> openglMapFlags = 0;

		if( resourceMapFlags.is_set( eGPUMemoryMapFlagAccessReadBit ) )
		{
			openglMapFlags.set( GL_MAP_READ_BIT );
		}
		if( resourceMapFlags.is_set( eGPUMemoryMapFlagAccessWriteBit ) )
		{
			openglMapFlags.set( GL_MAP_WRITE_BIT );
		}
		if( resourceMapFlags.is_set( eGPUMemoryMapFlagWriteInvalidateBit ) )
		{
			openglMapFlags.set( GL_MAP_INVALIDATE_BUFFER_BIT );
		}

	#if( IC3_GX_GL_FEATURE_SUPPORT_BUFFER_PERSISTENT_MAP )
		if( pMemoryFlags.is_set( eGPUMemoryHeapPropertyFlagPersistentMapBit ) )
		{
			openglMapFlags.set( GL_MAP_PERSISTENT_BIT );
		}
		if( pMemoryFlags.is_set( eGPUMemoryHeapPropertyFlagCPUCoherentBit ) )
		{
			openglMapFlags.set( GL_MAP_COHERENT_BIT );
		}
		if( openglMapFlags.is_set( GL_MAP_PERSISTENT_BIT ) && !openglMapFlags.is_set( GL_MAP_COHERENT_BIT ) )
		{
			openglMapFlags.set( GL_MAP_FLUSH_EXPLICIT_BIT );
		}
	#endif

		return openglMapFlags;
	}

	GLenum ATL::TranslateGLCompFunc( ECompFunc pCompFunc )
	{
		switch( pCompFunc )
		{
			Ic3CaseReturn( ECompFunc::Undefined    , GL_INVALID_ENUM );
			Ic3CaseReturn( ECompFunc::Never        , GL_NEVER    );
			Ic3CaseReturn( ECompFunc::Always       , GL_ALWAYS   );
			Ic3CaseReturn( ECompFunc::Equal        , GL_EQUAL    );
			Ic3CaseReturn( ECompFunc::NotEqual     , GL_NOTEQUAL );
			Ic3CaseReturn( ECompFunc::Greater      , GL_GREATER  );
			Ic3CaseReturn( ECompFunc::GreaterEqual , GL_GEQUAL   );
			Ic3CaseReturn( ECompFunc::Less         , GL_LESS     );
			Ic3CaseReturn( ECompFunc::LessEqual    , GL_LEQUAL   );
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::TranslateGLCullMode( ECullMode pCullMode )
	{
		switch( pCullMode )
		{
			Ic3CaseReturn( ECullMode::Undefined  , GL_INVALID_ENUM );
			Ic3CaseReturn( ECullMode::None       , GL_NONE );
			Ic3CaseReturn( ECullMode::Back       , GL_BACK  );
			Ic3CaseReturn( ECullMode::Front      , GL_FRONT );
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::TranslateGLIndexDataFormat( EIndexDataFormat pIndexDataFormat )
	{
		switch( pIndexDataFormat )
		{
			Ic3CaseReturn( EIndexDataFormat::Undefined , GL_INVALID_ENUM   );
			Ic3CaseReturn( EIndexDataFormat::Uint16    , GL_UNSIGNED_SHORT );
			Ic3CaseReturn( EIndexDataFormat::Uint32    , GL_UNSIGNED_INT   );
		}
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::TranslateGLPrimitiveFillMode( EPrimitiveFillMode pFillMode )
	{
	#if( IC3_GX_GL_FEATURE_SUPPORT_PRIMITIVE_FILL_MODE )
		switch( pFillMode )
		{
			Ic3CaseReturn( EPrimitiveFillMode::Undefined , GL_INVALID_ENUM );
			Ic3CaseReturn( EPrimitiveFillMode::Solid     , GL_FILL );
			Ic3CaseReturn( EPrimitiveFillMode::Wireframe , GL_LINE );
		};
	#endif
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::TranslateGLPrimitiveTopology( EPrimitiveTopology pTopology )
	{
		switch( pTopology )
		{
			Ic3CaseReturn( EPrimitiveTopology::Undefined        , GL_INVALID_ENUM );
			Ic3CaseReturn( EPrimitiveTopology::PointList        , GL_POINTS );
			Ic3CaseReturn( EPrimitiveTopology::LineList         , GL_LINES );
			Ic3CaseReturn( EPrimitiveTopology::LineStrip        , GL_LINE_STRIP );
			Ic3CaseReturn( EPrimitiveTopology::TriangleList     , GL_TRIANGLES );
			Ic3CaseReturn( EPrimitiveTopology::TriangleStrip    , GL_TRIANGLE_STRIP );
		#if( IC3_GX_GL_FEATURE_SUPPORT_PRIMITIVE_TYPE_ADJACENCY )
			Ic3CaseReturn( EPrimitiveTopology::LineListAdj      , GL_LINES_ADJACENCY );
			Ic3CaseReturn( EPrimitiveTopology::LineStripAdj     , GL_LINE_STRIP_ADJACENCY );
			Ic3CaseReturn( EPrimitiveTopology::TriangleListAdj  , GL_TRIANGLES_ADJACENCY );
			Ic3CaseReturn( EPrimitiveTopology::TriangleStripAdj , GL_TRIANGLE_STRIP_ADJACENCY );
        #endif
		#if( IC3_GX_GL_FEATURE_SUPPORT_PRIMITIVE_TYPE_PATCHES )
			Ic3CaseReturn( EPrimitiveTopology::TesselationPatch , GL_PATCHES );
		#endif
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::translateShaderType( EShaderType pShaderType )
	{
		switch( pShaderType )
		{
			Ic3CaseReturn( EShaderType::Unknown    , GL_INVALID_ENUM );
			Ic3CaseReturn( EShaderType::GSVertex   , GL_VERTEX_SHADER );
			Ic3CaseReturn( EShaderType::GSPixel    , GL_FRAGMENT_SHADER );
		#if( IC3_GX_GL_FEATURE_SUPPORT_SHADER_TYPE_GEOMETRY )
			Ic3CaseReturn( EShaderType::GSGeometry , GL_GEOMETRY_SHADER );
        #endif
		#if( IC3_GX_GL_FEATURE_SUPPORT_SHADER_TYPE_TESSELATION )
			Ic3CaseReturn( EShaderType::GSTessHull   , GL_TESS_CONTROL_SHADER );
			Ic3CaseReturn( EShaderType::GSTessDomain , GL_TESS_EVALUATION_SHADER );
        #endif
		#if( IC3_GX_GL_FEATURE_SUPPORT_SHADER_TYPE_COMPUTE )
			Ic3CaseReturn( EShaderType::CSCompute , GL_COMPUTE_SHADER );
		#endif
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::TranslateGLStencilOp( EStencilOp pStencilOp )
	{
		switch( pStencilOp )
		{
			Ic3CaseReturn( EStencilOp::Undefined , GL_INVALID_ENUM );
			Ic3CaseReturn( EStencilOp::Zero      , GL_ZERO         );
			Ic3CaseReturn( EStencilOp::Keep      , GL_KEEP         );
			Ic3CaseReturn( EStencilOp::Replace   , GL_REPLACE      );
			Ic3CaseReturn( EStencilOp::IncrClamp , GL_INCR         );
			Ic3CaseReturn( EStencilOp::IncrWrap  , GL_INCR_WRAP    );
			Ic3CaseReturn( EStencilOp::DecrClamp , GL_DECR         );
			Ic3CaseReturn( EStencilOp::DecrWrap  , GL_DECR_WRAP    );
			Ic3CaseReturn( EStencilOp::Invert    , GL_INVERT       );
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::TranslateGLTextureAddressMode( ETextureAddressMode pAddressMode )
	{
		switch( pAddressMode )
		{
			Ic3CaseReturn( ETextureAddressMode::Undefined    , GL_INVALID_ENUM    );
			Ic3CaseReturn( ETextureAddressMode::ClampToEdge  , GL_CLAMP_TO_EDGE   );
			Ic3CaseReturn( ETextureAddressMode::MirrorRepeat , GL_MIRRORED_REPEAT );
			Ic3CaseReturn( ETextureAddressMode::Repeat       , GL_REPEAT          );

		#if( IC3_GX_GL_FEATURE_SUPPORT_TEXTURE_EXTENDED_ADDRESS_MODE )
			Ic3CaseReturn( ETextureAddressMode::ClampToColor      , GL_CLAMP_TO_BORDER      );
			Ic3CaseReturn( ETextureAddressMode::MirrorClampToEdge , GL_MIRROR_CLAMP_TO_EDGE );
		#endif
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::TranslateGLTextureBindTarget( ETextureClass pTextureDimensionClass )
	{
		switch( pTextureDimensionClass )
		{
			Ic3CaseReturn( ETextureClass::Unknown    , GL_INVALID_ENUM                 );
			Ic3CaseReturn( ETextureClass::T2D        , GL_TEXTURE_2D                   );
			Ic3CaseReturn( ETextureClass::T2DArray   , GL_TEXTURE_2D_ARRAY             );
			Ic3CaseReturn( ETextureClass::T2DMS      , GL_TEXTURE_2D_MULTISAMPLE       );
			Ic3CaseReturn( ETextureClass::T2DMSArray , GL_TEXTURE_2D_MULTISAMPLE_ARRAY );
			Ic3CaseReturn( ETextureClass::T3D        , GL_TEXTURE_3D                   );
			Ic3CaseReturn( ETextureClass::TCubeMap   , GL_TEXTURE_CUBE_MAP             );
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::TranslateGLTextureInternalFormat( ETextureFormat pTextureFormat )
	{
		switch( pTextureFormat )
		{
			Ic3CaseReturn( ETextureFormat::Undefined  , GL_INVALID_ENUM );
			Ic3CaseReturn( ETextureFormat::R32F       , GL_R32F         );
			Ic3CaseReturn( ETextureFormat::R32I       , GL_R32I         );
			Ic3CaseReturn( ETextureFormat::R32U       , GL_R32UI        );
			Ic3CaseReturn( ETextureFormat::RG32F      , GL_RG32F        );
			Ic3CaseReturn( ETextureFormat::RG32I      , GL_RG32I        );
			Ic3CaseReturn( ETextureFormat::RG32U      , GL_RG32UI       );
			Ic3CaseReturn( ETextureFormat::RGB32F     , GL_RGB32F       );
			Ic3CaseReturn( ETextureFormat::RGB32I     , GL_RGB32I       );
			Ic3CaseReturn( ETextureFormat::RGB32U     , GL_RGB32UI      );
			Ic3CaseReturn( ETextureFormat::RGBA32F    , GL_RGBA32F      );
			Ic3CaseReturn( ETextureFormat::RGBA32I    , GL_RGBA32I      );
			Ic3CaseReturn( ETextureFormat::RGBA32U    , GL_RGBA32UI     );
			Ic3CaseReturn( ETextureFormat::R16F       , GL_R16F         );
			Ic3CaseReturn( ETextureFormat::R16I       , GL_R16I         );
			Ic3CaseReturn( ETextureFormat::R16U       , GL_R16UI        );
			Ic3CaseReturn( ETextureFormat::RG16F      , GL_RG16F        );
			Ic3CaseReturn( ETextureFormat::RG16I      , GL_RG16I        );
			Ic3CaseReturn( ETextureFormat::RG16U      , GL_RG16UI       );
			Ic3CaseReturn( ETextureFormat::RGBA16F    , GL_RGBA16F      );
			Ic3CaseReturn( ETextureFormat::RGBA16I    , GL_RGBA16I      );
			Ic3CaseReturn( ETextureFormat::RGBA16U    , GL_RGBA16UI     );
			Ic3CaseReturn( ETextureFormat::R8I        , GL_R8I          );
			Ic3CaseReturn( ETextureFormat::R8U        , GL_R8UI         );
			Ic3CaseReturn( ETextureFormat::R8IN       , GL_R8_SNORM     );
			Ic3CaseReturn( ETextureFormat::R8UN       , GL_R8           );
			Ic3CaseReturn( ETextureFormat::RG8I       , GL_RG8I         );
			Ic3CaseReturn( ETextureFormat::RG8U       , GL_RG8UI        );
			Ic3CaseReturn( ETextureFormat::RG8IN      , GL_RG8_SNORM    );
			Ic3CaseReturn( ETextureFormat::RG8UN      , GL_RG8          );
		#if( IC3_GX_GL_FEATURE_SUPPORT_TEXTURE_FORMAT_BGR )
			Ic3CaseReturn( ETextureFormat::BGRX8UN    , GL_RGB8         );
			Ic3CaseReturn( ETextureFormat::BGRX8SRGB  , GL_SRGB8        );
			Ic3CaseReturn( ETextureFormat::BGRA8UN    , GL_RGBA8        );
			Ic3CaseReturn( ETextureFormat::BGRA8SRGB  , GL_SRGB8_ALPHA8 );
		#endif
			Ic3CaseReturn( ETextureFormat::RGBA8I     , GL_RGBA8I       );
			Ic3CaseReturn( ETextureFormat::RGBA8U     , GL_RGBA8UI      );
			Ic3CaseReturn( ETextureFormat::RGBA8IN    , GL_RGBA8_SNORM  );
			Ic3CaseReturn( ETextureFormat::RGBA8UN    , GL_RGBA8        );
			Ic3CaseReturn( ETextureFormat::RGBA8SRGB  , GL_SRGB8_ALPHA8 );

			Ic3CaseReturn( ETextureFormat::R5G5B5A1   , GL_RGB5_A1            );
			Ic3CaseReturn( ETextureFormat::R5G6B5     , GL_RGB565             );
			Ic3CaseReturn( ETextureFormat::R9G9B9E5   , GL_RGB9_E5            );
			Ic3CaseReturn( ETextureFormat::RGB10A2U   , GL_RGB10_A2UI         );
			Ic3CaseReturn( ETextureFormat::RGB10A2UN  , GL_RGB10_A2           );
			Ic3CaseReturn( ETextureFormat::R11G11B10F , GL_R11F_G11F_B10F     );
			Ic3CaseReturn( ETextureFormat::D16UN      , GL_DEPTH_COMPONENT16  );
			Ic3CaseReturn( ETextureFormat::D24UNS8U   , GL_DEPTH24_STENCIL8   );
			Ic3CaseReturn( ETextureFormat::D24UNX8    , GL_DEPTH_COMPONENT24  );
			Ic3CaseReturn( ETextureFormat::X24S8U     , GL_DEPTH24_STENCIL8   );
			Ic3CaseReturn( ETextureFormat::D32F       , GL_DEPTH_COMPONENT32F );
		#if( IC3_GX_GL_FEATURE_SUPPORT_TEXTURE_FORMAT_COMPRESSED_BCX )
			Ic3CaseReturn( ETextureFormat::BC1        , GL_TEX_FORMAT_COMPRESSED_BC1         );
			Ic3CaseReturn( ETextureFormat::BC1SRGB    , GL_TEX_FORMAT_COMPRESSED_BC1_SRGBA   );
			Ic3CaseReturn( ETextureFormat::BC2        , GL_TEX_FORMAT_COMPRESSED_BC2         );
			Ic3CaseReturn( ETextureFormat::BC2SRGB    , GL_TEX_FORMAT_COMPRESSED_BC2_SRGB    );
			Ic3CaseReturn( ETextureFormat::BC3        , GL_TEX_FORMAT_COMPRESSED_BC3         );
			Ic3CaseReturn( ETextureFormat::BC3SRGB    , GL_TEX_FORMAT_COMPRESSED_BC3_SRGB    );
			Ic3CaseReturn( ETextureFormat::BC4IN      , GL_TEX_FORMAT_COMPRESSED_BC4_SNORM   );
			Ic3CaseReturn( ETextureFormat::BC4UN      , GL_TEX_FORMAT_COMPRESSED_BC4_UNORM   );
			Ic3CaseReturn( ETextureFormat::BC5IN      , GL_TEX_FORMAT_COMPRESSED_BC5_SNORM   );
			Ic3CaseReturn( ETextureFormat::BC5UN      , GL_TEX_FORMAT_COMPRESSED_BC5_UNORM   );
			Ic3CaseReturn( ETextureFormat::BC6HSF     , GL_TEX_FORMAT_COMPRESSED_BC6H_SFLOAT );
			Ic3CaseReturn( ETextureFormat::BC6HUF     , GL_TEX_FORMAT_COMPRESSED_BC6H_UFLOAT );
			Ic3CaseReturn( ETextureFormat::BC7        , GL_TEX_FORMAT_COMPRESSED_BC7         );
			Ic3CaseReturn( ETextureFormat::BC7SRGB    , GL_TEX_FORMAT_COMPRESSED_BC7_SRGB    );
		#endif
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::TranslateGLTexturePixelDataLayout( ETextureFormat pTextureFormat )
	{
		switch( pTextureFormat )
		{
			Ic3CaseReturn( ETextureFormat::Undefined    , GL_INVALID_ENUM );
			Ic3CaseReturn( ETextureFormat::R32F       , GL_RED  );
			Ic3CaseReturn( ETextureFormat::R32I       , GL_RED  );
			Ic3CaseReturn( ETextureFormat::R32U       , GL_RED  );
			Ic3CaseReturn( ETextureFormat::RG32F      , GL_RG   );
			Ic3CaseReturn( ETextureFormat::RG32I      , GL_RG   );
			Ic3CaseReturn( ETextureFormat::RG32U      , GL_RG   );
			Ic3CaseReturn( ETextureFormat::RGB32F     , GL_RGB  );
			Ic3CaseReturn( ETextureFormat::RGB32I     , GL_RGB  );
			Ic3CaseReturn( ETextureFormat::RGB32U     , GL_RGB  );
			Ic3CaseReturn( ETextureFormat::RGBA32F    , GL_RGBA );
			Ic3CaseReturn( ETextureFormat::RGBA32I    , GL_RGBA );
			Ic3CaseReturn( ETextureFormat::RGBA32U    , GL_RGBA );
			Ic3CaseReturn( ETextureFormat::R16F       , GL_RED  );
			Ic3CaseReturn( ETextureFormat::R16I       , GL_RED  );
			Ic3CaseReturn( ETextureFormat::R16U       , GL_RED  );
			Ic3CaseReturn( ETextureFormat::RG16F      , GL_RG   );
			Ic3CaseReturn( ETextureFormat::RG16I      , GL_RG   );
			Ic3CaseReturn( ETextureFormat::RG16U      , GL_RG   );
			Ic3CaseReturn( ETextureFormat::RGBA16F    , GL_RGBA );
			Ic3CaseReturn( ETextureFormat::RGBA16I    , GL_RGBA );
			Ic3CaseReturn( ETextureFormat::RGBA16U    , GL_RGBA );
			Ic3CaseReturn( ETextureFormat::R8I        , GL_RED  );
			Ic3CaseReturn( ETextureFormat::R8U        , GL_RED  );
			Ic3CaseReturn( ETextureFormat::R8IN       , GL_RED  );
			Ic3CaseReturn( ETextureFormat::R8UN       , GL_RED  );
			Ic3CaseReturn( ETextureFormat::RG8I       , GL_RG   );
			Ic3CaseReturn( ETextureFormat::RG8U       , GL_RG   );
			Ic3CaseReturn( ETextureFormat::RG8IN      , GL_RG   );
			Ic3CaseReturn( ETextureFormat::RG8UN      , GL_RG   );
		#if( IC3_GX_GL_FEATURE_SUPPORT_TEXTURE_FORMAT_BGR )
			Ic3CaseReturn( ETextureFormat::BGRX8UN    , GL_BGR  );
			Ic3CaseReturn( ETextureFormat::BGRX8SRGB  , GL_BGR  );
			Ic3CaseReturn( ETextureFormat::BGRA8UN    , GL_BGRA );
			Ic3CaseReturn( ETextureFormat::BGRA8SRGB  , GL_BGRA );
		#endif
			Ic3CaseReturn( ETextureFormat::RGBA8I     , GL_RGBA );
			Ic3CaseReturn( ETextureFormat::RGBA8U     , GL_RGBA );
			Ic3CaseReturn( ETextureFormat::RGBA8IN    , GL_RGBA );
			Ic3CaseReturn( ETextureFormat::RGBA8UN    , GL_RGBA );
			Ic3CaseReturn( ETextureFormat::RGBA8SRGB  , GL_RGBA );
			Ic3CaseReturn( ETextureFormat::R5G5B5A1   , GL_RGB5_A1            );
			Ic3CaseReturn( ETextureFormat::R5G6B5     , GL_RGB565             );
			Ic3CaseReturn( ETextureFormat::R9G9B9E5   , GL_RGB9_E5            );
			Ic3CaseReturn( ETextureFormat::RGB10A2U   , GL_RGB10_A2UI         );
			Ic3CaseReturn( ETextureFormat::RGB10A2UN  , GL_RGB10_A2           );
			Ic3CaseReturn( ETextureFormat::R11G11B10F , GL_R11F_G11F_B10F     );
			Ic3CaseReturn( ETextureFormat::D16UN      , GL_DEPTH_COMPONENT16  );
			Ic3CaseReturn( ETextureFormat::D24UNS8U   , GL_DEPTH_STENCIL      );
			Ic3CaseReturn( ETextureFormat::D24UNX8    , GL_DEPTH_STENCIL      );
			Ic3CaseReturn( ETextureFormat::X24S8U     , GL_DEPTH_STENCIL      );
			Ic3CaseReturn( ETextureFormat::D32F       , GL_DEPTH_COMPONENT    );
		#if( IC3_GX_GL_FEATURE_SUPPORT_TEXTURE_FORMAT_COMPRESSED_BCX )
			Ic3CaseReturn( ETextureFormat::BC1        , GL_TEX_FORMAT_COMPRESSED_BC1         );
			Ic3CaseReturn( ETextureFormat::BC1SRGB    , GL_TEX_FORMAT_COMPRESSED_BC1_SRGBA   );
			Ic3CaseReturn( ETextureFormat::BC2        , GL_TEX_FORMAT_COMPRESSED_BC2         );
			Ic3CaseReturn( ETextureFormat::BC2SRGB    , GL_TEX_FORMAT_COMPRESSED_BC2_SRGB    );
			Ic3CaseReturn( ETextureFormat::BC3        , GL_TEX_FORMAT_COMPRESSED_BC3         );
			Ic3CaseReturn( ETextureFormat::BC3SRGB    , GL_TEX_FORMAT_COMPRESSED_BC3_SRGB    );
			Ic3CaseReturn( ETextureFormat::BC4IN      , GL_TEX_FORMAT_COMPRESSED_BC4_SNORM   );
			Ic3CaseReturn( ETextureFormat::BC4UN      , GL_TEX_FORMAT_COMPRESSED_BC4_UNORM   );
			Ic3CaseReturn( ETextureFormat::BC5IN      , GL_TEX_FORMAT_COMPRESSED_BC5_SNORM   );
			Ic3CaseReturn( ETextureFormat::BC5UN      , GL_TEX_FORMAT_COMPRESSED_BC5_UNORM   );
			Ic3CaseReturn( ETextureFormat::BC6HSF     , GL_TEX_FORMAT_COMPRESSED_BC6H_SFLOAT );
			Ic3CaseReturn( ETextureFormat::BC6HUF     , GL_TEX_FORMAT_COMPRESSED_BC6H_UFLOAT );
			Ic3CaseReturn( ETextureFormat::BC7        , GL_TEX_FORMAT_COMPRESSED_BC7         );
			Ic3CaseReturn( ETextureFormat::BC7SRGB    , GL_TEX_FORMAT_COMPRESSED_BC7_SRGB    );
		#endif
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::TranslateGLTriangleVerticesOrder( ETriangleVerticesOrder pVerticesOrder )
	{
		switch( pVerticesOrder )
		{
			Ic3CaseReturn( ETriangleVerticesOrder::Clockwise        , GL_CW  );
			Ic3CaseReturn( ETriangleVerticesOrder::CounterClockwise , GL_CCW );
		}
		return GL_IC3_ERR_INVALID_PARAM;
	}

	const char * ATL::TranslateGLDebugOutputExtensionName( GLDebugOutputVersion pDebugOutputVersion )
	{
		switch( pDebugOutputVersion )
		{
			Ic3CaseReturn( GLDebugOutputVersion::Unknown , "" );
			Ic3CaseReturn( GLDebugOutputVersion::AMDExt  , "GL_AMD_debug_output" );
			Ic3CaseReturn( GLDebugOutputVersion::ARBExt  , "GL_ARB_debug_output" );
			Ic3CaseReturn( GLDebugOutputVersion::KHRCore , "GL_KHR_debug"        );
		}
		return "Unknown";
	}

	const char * ATL::TranslateGLDebugEventCategoryStrAMD( GLenum pEventCategory )
	{
	#if( IC3_GX_GL_FEATURE_SUPPORT_DEBUG_OUTPUT )
		switch( pEventCategory )
		{
			Ic3CaseReturn( GL_DEBUG_CATEGORY_API_ERROR_AMD          , "API error"          );
			Ic3CaseReturn( GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD      , "Window system"      );
			Ic3CaseReturn( GL_DEBUG_CATEGORY_DEPRECATION_AMD        , "Deprecation"        );
			Ic3CaseReturn( GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD , "Undefined behavior" );
			Ic3CaseReturn( GL_DEBUG_CATEGORY_PERFORMANCE_AMD        , "Performance"        );
			Ic3CaseReturn( GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD    , "Shader compiler"    );
			Ic3CaseReturn( GL_DEBUG_CATEGORY_APPLICATION_AMD        , "Application"        );
			Ic3CaseReturn( GL_DEBUG_CATEGORY_OTHER_AMD              , "Other"              );
		}
	#endif
		return "Unknown";
	}

	const char * ATL::TranslateGLDebugEventSeverityStr( GLenum pEventSeverity )
	{
	#if( IC3_GX_GL_FEATURE_SUPPORT_DEBUG_OUTPUT )
	    switch( pEventSeverity )
		{
			Ic3CaseReturn( GL_DEBUG_SEVERITY_HIGH_ARB     , "High"         );
			Ic3CaseReturn( GL_DEBUG_SEVERITY_MEDIUM_ARB   , "Medium"       );
			Ic3CaseReturn( GL_DEBUG_SEVERITY_LOW_ARB      , "Low"          );
			Ic3CaseReturn( GL_DEBUG_SEVERITY_NOTIFICATION , "Notification" );
		}
	#endif
		return "Unknown";
	}

	const char * ATL::TranslateGLDebugEventSourceStr( GLenum pEventSource )
	{
	#if( IC3_GX_GL_FEATURE_SUPPORT_DEBUG_OUTPUT )
	    switch( pEventSource )
		{
			Ic3CaseReturn( GL_DEBUG_SOURCE_API_ARB             , "GL API"          );
			Ic3CaseReturn( GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB   , "Window system"   );
			Ic3CaseReturn( GL_DEBUG_SOURCE_SHADER_COMPILER_ARB , "Shader compiler" );
			Ic3CaseReturn( GL_DEBUG_SOURCE_THIRD_PARTY_ARB     , "Third party"     );
			Ic3CaseReturn( GL_DEBUG_SOURCE_APPLICATION_ARB     , "Application"     );
			Ic3CaseReturn( GL_DEBUG_SOURCE_OTHER_ARB           , "Other"           );
		}
	#endif
		return "Unknown";
	}

	const char * ATL::TranslateGLDebugEventTypeStr( GLenum pEventType )
	{
	#if( IC3_GX_GL_FEATURE_SUPPORT_DEBUG_OUTPUT )
	    switch( pEventType )
		{
			Ic3CaseReturn( GL_DEBUG_TYPE_ERROR_ARB               , "GL API"          );
			Ic3CaseReturn( GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB , "Window system"   );
			Ic3CaseReturn( GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB  , "Shader compiler" );
			Ic3CaseReturn( GL_DEBUG_TYPE_PORTABILITY_ARB         , "Third party"     );
			Ic3CaseReturn( GL_DEBUG_TYPE_PERFORMANCE_ARB         , "Application"     );
			Ic3CaseReturn( GL_DEBUG_TYPE_OTHER_ARB               , "Other"           );
		}
	#endif
		return "Unknown";
	}

} // namespace Ic3::Graphics::GCI
