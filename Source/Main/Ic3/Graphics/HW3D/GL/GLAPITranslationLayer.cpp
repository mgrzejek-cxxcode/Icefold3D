
#include "GLAPITranslationLayer.h"
#include <Ic3/Graphics/GCI/Resources/GPUBufferCommon.h>
#include <Ic3/Graphics/GCI/Resources/ShaderCommon.h>
#include <Ic3/Graphics/GCI/Resources/TextureCommon.h>
#include <Ic3/Graphics/GCI/State/SamplerCommon.h>
#include <Ic3/Cppx/STLHelperAlgo.h>
#include <unordered_map>

namespace Ic3::Graphics::GCI
{
#if( ICFGX_GL_TARGET == ICFGX_GL_TARGET_GL43 )
	GLenum ATL::chooseGLBufferStorageFlags( GLenum pBindTarget, Bitmask<resource_flags_value_t> pBufferFlags, Bitmask<EGPUMemoryFlags> pMemoryFlags )
	{
		Bitmask<GLenum> storageFlags = 0;

		if( !pBufferFlags.isSet( E_GPU_RESOURCE_CONTENT_FLAG_IMMUTABLE_BIT ) )
		{
			// DYNAMIC_STORAGE is required in order to perform *ANY* kind of buffer data update.
			// Without this flag, it is impossible to change the buffer's content even via glBufferSubData.
			// Because of that, we apply this to every buffer, except those specified explicitly as IMMUTABLE.
			storageFlags.set( GL_DYNAMIC_STORAGE_BIT );
		}
		if( pBufferFlags.isSet( E_GPU_RESOURCE_CONTENT_FLAG_TEMPORARY_BIT ) )
		{
			storageFlags.set( GL_CLIENT_STORAGE_BIT );
		}
		if( pMemoryFlags.isSet( E_GPU_MEMORY_ACCESS_FLAG_CPU_READ_BIT ) )
		{
			storageFlags.set( GL_MAP_READ_BIT );
		}
		if( pMemoryFlags.isSet( E_GPU_MEMORY_ACCESS_FLAG_CPU_WRITE_BIT ) )
		{
			storageFlags.set( GL_MAP_WRITE_BIT );
		}
		if( pMemoryFlags.isSet( E_GPU_MEMORY_HEAP_PROPERTY_FLAG_CPU_CACHED_BIT ) )
		{
			storageFlags.set( GL_CLIENT_STORAGE_BIT );
		}
		if( pMemoryFlags.isSet( E_GPU_MEMORY_HEAP_PROPERTY_FLAG_PERSISTENT_MAP_BIT ) )
		{
			storageFlags.set( GL_MAP_PERSISTENT_BIT );
			if( pMemoryFlags.isSetAnyOf( E_GPU_MEMORY_HEAP_PROPERTY_FLAG_CPU_COHERENT_BIT | E_GPU_MEMORY_HEAP_PROPERTY_FLAG_GPU_COHERENT_BIT ) )
			{
				storageFlags.set( GL_MAP_COHERENT_BIT );
			}
		}

		return storageFlags;
	}
#endif

	GLenum ATL::chooseGLBufferUsagePolicy( GLenum pBindTarget, Bitmask<resource_flags_value_t> pBufferFlags )
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
		if( pBufferFlags.isSet( E_GPU_RESOURCE_CONTENT_FLAG_DYNAMIC_BIT ) )
		{
			usagePolicy = GL_DYNAMIC_DRAW;
		}
		// Temporary buffers, updated once and used few times. That fits with STREAM policy.
		else if( pBufferFlags.isSet( E_GPU_RESOURCE_CONTENT_FLAG_TEMPORARY_BIT ) )
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

	GLenum ATL::chooseGLTextureMagFilter( ETextureFilter pMagFilter, ETextureMipMode pMipMode )
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

	GLenum ATL::chooseGLTextureMinFilter( ETextureFilter pMinFilter, ETextureMipMode pMipMode )
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

	GLsizei ATL::queryGLTextureInternalFormatBPP( GLenum pGLInternalFormat )
	{
		switch( pGLInternalFormat )
		{
			ic3CaseReturn( GL_R32F         , 32  );
			ic3CaseReturn( GL_R32I         , 32  );
			ic3CaseReturn( GL_R32UI        , 32  );
			ic3CaseReturn( GL_RG32F        , 64  );
			ic3CaseReturn( GL_RG32I        , 64  );
			ic3CaseReturn( GL_RG32UI       , 64  );
			ic3CaseReturn( GL_RGB32F       , 96  );
			ic3CaseReturn( GL_RGB32I       , 96  );
			ic3CaseReturn( GL_RGB32UI      , 96  );
			ic3CaseReturn( GL_RGBA32F      , 128 );
			ic3CaseReturn( GL_RGBA32I      , 128 );
			ic3CaseReturn( GL_RGBA32UI     , 128 );
			ic3CaseReturn( GL_R16F         , 16  );
			ic3CaseReturn( GL_R16I         , 16  );
			ic3CaseReturn( GL_R16UI        , 16  );
			ic3CaseReturn( GL_RG16F        , 32  );
			ic3CaseReturn( GL_RG16I        , 32  );
			ic3CaseReturn( GL_RG16UI       , 32  );
			ic3CaseReturn( GL_RGBA16F      , 64  );
			ic3CaseReturn( GL_RGBA16I      , 64  );
			ic3CaseReturn( GL_RGBA16UI     , 64  );
			ic3CaseReturn( GL_R8I          , 8   );
			ic3CaseReturn( GL_R8UI         , 8   );
			ic3CaseReturn( GL_R8_SNORM     , 8   );
			ic3CaseReturn( GL_R8           , 8   );
			ic3CaseReturn( GL_RG8I         , 16  );
			ic3CaseReturn( GL_RG8UI        , 16  );
			ic3CaseReturn( GL_RG8_SNORM    , 16  );
			ic3CaseReturn( GL_RG8          , 16  );
			ic3CaseReturn( GL_RGB8         , 24  );
			ic3CaseReturn( GL_SRGB8        , 24  );
			ic3CaseReturn( GL_RGBA8        , 32  );
			ic3CaseReturn( GL_SRGB8_ALPHA8 , 32  );
			ic3CaseReturn( GL_RGBA8I       , 32  );
			ic3CaseReturn( GL_RGBA8UI      , 32  );
			ic3CaseReturn( GL_RGBA8_SNORM  , 32  );

			ic3CaseReturn( GL_RGB5_A1            , 16 );
			ic3CaseReturn( GL_RGB565             , 16 );
			ic3CaseReturn( GL_RGB9_E5            , 32 );
			ic3CaseReturn( GL_RGB10_A2UI         , 32 );
			ic3CaseReturn( GL_RGB10_A2           , 32 );
			ic3CaseReturn( GL_R11F_G11F_B10F     , 32 );
			ic3CaseReturn( GL_DEPTH_COMPONENT16  , 16 );
			ic3CaseReturn( GL_DEPTH24_STENCIL8   , 32 );
			ic3CaseReturn( GL_DEPTH_COMPONENT24  , 24 );
			ic3CaseReturn( GL_DEPTH_COMPONENT32F , 32 );
			ic3CaseReturn( GL_DEPTH32F_STENCIL8  , 64 );

		#if( ICFGX_GL_PLATFORM_TYPE == ICFGX_GL_PLATFORM_TYPE_DESKTOP )
			ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC1        , 4 );
			ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC1_SRGBA  , 4 );
			ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC2        , 4 );
			ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC2_SRGB   , 8 );
			ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC3        , 8 );
			ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC3_SRGB   , 8 );
			ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC4_SNORM  , 8 );
			ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC4_UNORM  , 8 );
			ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC5_SNORM  , 8 );
			ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC5_UNORM  , 0 );
			ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC6H_SFLOAT, 0 );
			ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC6H_UFLOAT, 0 );
			ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC7        , 0 );
			ic3CaseReturn( GL_TEX_FORMAT_COMPRESSED_BC7_SRGB   , 0 );
		#endif
		}
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::translateGLBaseDataType( EBaseDataType pBaseDataType )
	{
		switch( pBaseDataType )
		{
			ic3CaseReturn( EBaseDataType::Undefined, GL_INVALID_ENUM      );
			ic3CaseReturn( EBaseDataType::Byte     , GL_BYTE              );
			ic3CaseReturn( EBaseDataType::Ubyte    , GL_UNSIGNED_BYTE     );
			ic3CaseReturn( EBaseDataType::Int16    , GL_SHORT             );
			ic3CaseReturn( EBaseDataType::Uint16   , GL_UNSIGNED_SHORT    );
			ic3CaseReturn( EBaseDataType::Int32    , GL_INT               );
			ic3CaseReturn( EBaseDataType::Uint32   , GL_UNSIGNED_INT      );
			ic3CaseReturn( EBaseDataType::Float16  , GL_HALF_FLOAT        );
			ic3CaseReturn( EBaseDataType::Float32  , GL_FLOAT             );
			ic3CaseReturn( EBaseDataType::Uint24S8 , GL_UNSIGNED_INT_24_8 );
		}
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::translateGLBlendFactor( EBlendFactor pBlendFactor )
	{
		switch( pBlendFactor )
		{
			ic3CaseReturn( EBlendFactor::Undefined   , GL_INVALID_ENUM             );
			ic3CaseReturn( EBlendFactor::Zero        , GL_ZERO                     );
			ic3CaseReturn( EBlendFactor::One         , GL_ONE                      );
			ic3CaseReturn( EBlendFactor::Const       , GL_CONSTANT_COLOR           );
			ic3CaseReturn( EBlendFactor::ConstInv    , GL_ONE_MINUS_CONSTANT_COLOR );
			ic3CaseReturn( EBlendFactor::SrcColor    , GL_SRC_COLOR                );
			ic3CaseReturn( EBlendFactor::SrcAlpha    , GL_SRC_ALPHA                );
			ic3CaseReturn( EBlendFactor::DstColor    , GL_DST_COLOR                );
			ic3CaseReturn( EBlendFactor::DstAlpha    , GL_DST_ALPHA                );
			ic3CaseReturn( EBlendFactor::SrcColorInv , GL_ONE_MINUS_SRC_COLOR      );
			ic3CaseReturn( EBlendFactor::SrcAlphaInv , GL_ONE_MINUS_SRC_ALPHA      );
			ic3CaseReturn( EBlendFactor::DstColorInv , GL_ONE_MINUS_DST_COLOR      );
			ic3CaseReturn( EBlendFactor::DstAlphaInv , GL_ONE_MINUS_DST_ALPHA      );
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::translateGLBlendOp( EBlendOp pBlendOp )
	{
		switch( pBlendOp )
		{
			ic3CaseReturn( EBlendOp::Undefined   , GL_INVALID_ENUM          );
			ic3CaseReturn( EBlendOp::Add         , GL_FUNC_ADD              );
			ic3CaseReturn( EBlendOp::Min         , GL_MIN                   );
			ic3CaseReturn( EBlendOp::Max         , GL_MAX                   );
			ic3CaseReturn( EBlendOp::Subtract    , GL_FUNC_SUBTRACT         );
			ic3CaseReturn( EBlendOp::SubtractRev , GL_FUNC_REVERSE_SUBTRACT );
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::translateGLBufferBindTarget( EGPUBufferTarget pBufferTarget )
	{
		switch( pBufferTarget )
		{
			ic3CaseReturn( EGPUBufferTarget::Unknown                , GL_INVALID_ENUM              );
			ic3CaseReturn( EGPUBufferTarget::ConstantBuffer         , GL_UNIFORM_BUFFER            );
			ic3CaseReturn( EGPUBufferTarget::VertexBuffer           , GL_ARRAY_BUFFER              );
			ic3CaseReturn( EGPUBufferTarget::IndexBuffer            , GL_ELEMENT_ARRAY_BUFFER      );
			ic3CaseReturn( EGPUBufferTarget::ShaderInputBuffer      , GL_SHADER_STORAGE_BUFFER     );
			ic3CaseReturn( EGPUBufferTarget::ShaderUAVBuffer        , GL_SHADER_STORAGE_BUFFER     );
			ic3CaseReturn( EGPUBufferTarget::StreamOutputBuffer     , GL_TRANSFORM_FEEDBACK_BUFFER );
			ic3CaseReturn( EGPUBufferTarget::IndirectDispatchBuffer , GL_DISPATCH_INDIRECT_BUFFER  );
			ic3CaseReturn( EGPUBufferTarget::IndirectDrawBuffer     , GL_DRAW_INDIRECT_BUFFER      );
			ic3CaseReturn( EGPUBufferTarget::TransferSourceBuffer   , GL_COPY_READ_BUFFER          );
			ic3CaseReturn( EGPUBufferTarget::TransferTargetBuffer   , GL_COPY_WRITE_BUFFER         );
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::translateGLBufferMapFlags( EGPUMemoryMapMode pMapMode, Bitmask<EGPUMemoryFlags> pMemoryFlags )
	{
		Bitmask<uint32> resourceMapFlags = static_cast<uint32>( pMapMode );
		Bitmask<GLenum> openglMapFlags = 0;

		if( resourceMapFlags.isSet( E_GPU_MEMORY_MAP_FLAG_ACCESS_READ_BIT ) )
		{
			openglMapFlags.set( GL_MAP_READ_BIT );
		}
		if( resourceMapFlags.isSet( E_GPU_MEMORY_MAP_FLAG_ACCESS_WRITE_BIT ) )
		{
			openglMapFlags.set( GL_MAP_WRITE_BIT );
		}
		if( resourceMapFlags.isSet( E_GPU_MEMORY_MAP_FLAG_WRITE_INVALIDATE_BIT ) )
		{
			openglMapFlags.set( GL_MAP_INVALIDATE_BUFFER_BIT );
		}

	#if( ICFGX_GL_FEATURE_SUPPORT_BUFFER_PERSISTENT_MAP )
		if( pMemoryFlags.isSet( E_GPU_MEMORY_HEAP_PROPERTY_FLAG_PERSISTENT_MAP_BIT ) )
		{
			openglMapFlags.set( GL_MAP_PERSISTENT_BIT );
		}
		if( pMemoryFlags.isSet( E_GPU_MEMORY_HEAP_PROPERTY_FLAG_CPU_COHERENT_BIT ) )
		{
			openglMapFlags.set( GL_MAP_COHERENT_BIT );
		}
		if( openglMapFlags.isSet( GL_MAP_PERSISTENT_BIT ) && !openglMapFlags.isSet( GL_MAP_COHERENT_BIT ) )
		{
			openglMapFlags.set( GL_MAP_FLUSH_EXPLICIT_BIT );
		}
	#endif

		return openglMapFlags;
	}

	GLenum ATL::translateGLCompFunc( ECompFunc pCompFunc )
	{
		switch( pCompFunc )
		{
			ic3CaseReturn( ECompFunc::Undefined    , GL_INVALID_ENUM );
			ic3CaseReturn( ECompFunc::Never        , GL_NEVER    );
			ic3CaseReturn( ECompFunc::Always       , GL_ALWAYS   );
			ic3CaseReturn( ECompFunc::Equal        , GL_EQUAL    );
			ic3CaseReturn( ECompFunc::NotEqual     , GL_NOTEQUAL );
			ic3CaseReturn( ECompFunc::Greater      , GL_GREATER  );
			ic3CaseReturn( ECompFunc::GreaterEqual , GL_GEQUAL   );
			ic3CaseReturn( ECompFunc::Less         , GL_LESS     );
			ic3CaseReturn( ECompFunc::LessEqual    , GL_LEQUAL   );
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::translateGLCullMode( ECullMode pCullMode )
	{
		switch( pCullMode )
		{
			ic3CaseReturn( ECullMode::Undefined  , GL_INVALID_ENUM );
			ic3CaseReturn( ECullMode::None       , GL_NONE );
			ic3CaseReturn( ECullMode::Back       , GL_BACK  );
			ic3CaseReturn( ECullMode::Front      , GL_FRONT );
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::translateGLIndexDataFormat( EIndexDataFormat pIndexDataFormat )
	{
		switch( pIndexDataFormat )
		{
			ic3CaseReturn( EIndexDataFormat::Undefined , GL_INVALID_ENUM   );
			ic3CaseReturn( EIndexDataFormat::Uint16    , GL_UNSIGNED_SHORT );
			ic3CaseReturn( EIndexDataFormat::Uint32    , GL_UNSIGNED_INT   );
		}
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::translateGLPrimitiveFillMode( EPrimitiveFillMode pFillMode )
	{
	#if( ICFGX_GL_FEATURE_SUPPORT_PRIMITIVE_FILL_MODE )
		switch( pFillMode )
		{
			ic3CaseReturn( EPrimitiveFillMode::Undefined , GL_INVALID_ENUM );
			ic3CaseReturn( EPrimitiveFillMode::Solid     , GL_FILL );
			ic3CaseReturn( EPrimitiveFillMode::Wireframe , GL_LINE );
		};
	#endif
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::translateGLPrimitiveTopology( EPrimitiveTopology pTopology )
	{
		switch( pTopology )
		{
			ic3CaseReturn( EPrimitiveTopology::Undefined        , GL_INVALID_ENUM );
			ic3CaseReturn( EPrimitiveTopology::PointList        , GL_POINTS );
			ic3CaseReturn( EPrimitiveTopology::LineList         , GL_LINES );
			ic3CaseReturn( EPrimitiveTopology::LineStrip        , GL_LINE_STRIP );
			ic3CaseReturn( EPrimitiveTopology::TriangleList     , GL_TRIANGLES );
			ic3CaseReturn( EPrimitiveTopology::TriangleStrip    , GL_TRIANGLE_STRIP );
		#if( ICFGX_GL_FEATURE_SUPPORT_PRIMITIVE_TYPE_ADJACENCY )
			ic3CaseReturn( EPrimitiveTopology::LineListAdj      , GL_LINES_ADJACENCY );
			ic3CaseReturn( EPrimitiveTopology::LineStripAdj     , GL_LINE_STRIP_ADJACENCY );
			ic3CaseReturn( EPrimitiveTopology::TriangleListAdj  , GL_TRIANGLES_ADJACENCY );
			ic3CaseReturn( EPrimitiveTopology::TriangleStripAdj , GL_TRIANGLE_STRIP_ADJACENCY );
        #endif
		#if( ICFGX_GL_FEATURE_SUPPORT_PRIMITIVE_TYPE_PATCHES )
			ic3CaseReturn( EPrimitiveTopology::TesselationPatch , GL_PATCHES );
		#endif
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::translateShaderType( EShaderType pShaderType )
	{
		switch( pShaderType )
		{
			ic3CaseReturn( EShaderType::Unknown    , GL_INVALID_ENUM );
			ic3CaseReturn( EShaderType::GSVertex   , GL_VERTEX_SHADER );
			ic3CaseReturn( EShaderType::GSPixel    , GL_FRAGMENT_SHADER );
		#if( ICFGX_GL_FEATURE_SUPPORT_SHADER_TYPE_GEOMETRY )
			ic3CaseReturn( EShaderType::GSGeometry , GL_GEOMETRY_SHADER );
        #endif
		#if( ICFGX_GL_FEATURE_SUPPORT_SHADER_TYPE_TESSELATION )
			ic3CaseReturn( EShaderType::GSHull   , GL_TESS_CONTROL_SHADER );
			ic3CaseReturn( EShaderType::GSDomain , GL_TESS_EVALUATION_SHADER );
        #endif
		#if( ICFGX_GL_FEATURE_SUPPORT_SHADER_TYPE_COMPUTE )
			ic3CaseReturn( EShaderType::CSCompute , GL_COMPUTE_SHADER );
		#endif
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::translateGLStencilOp( EStencilOp pStencilOp )
	{
		switch( pStencilOp )
		{
			ic3CaseReturn( EStencilOp::Undefined , GL_INVALID_ENUM );
			ic3CaseReturn( EStencilOp::Zero      , GL_ZERO         );
			ic3CaseReturn( EStencilOp::Keep      , GL_KEEP         );
			ic3CaseReturn( EStencilOp::Replace   , GL_REPLACE      );
			ic3CaseReturn( EStencilOp::IncrClamp , GL_INCR         );
			ic3CaseReturn( EStencilOp::IncrWrap  , GL_INCR_WRAP    );
			ic3CaseReturn( EStencilOp::DecrClamp , GL_DECR         );
			ic3CaseReturn( EStencilOp::DecrWrap  , GL_DECR_WRAP    );
			ic3CaseReturn( EStencilOp::Invert    , GL_INVERT       );
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::translateGLTextureAddressMode( ETextureAddressMode pAddressMode )
	{
		switch( pAddressMode )
		{
			ic3CaseReturn( ETextureAddressMode::Undefined    , GL_INVALID_ENUM    );
			ic3CaseReturn( ETextureAddressMode::ClampToEdge  , GL_CLAMP_TO_EDGE   );
			ic3CaseReturn( ETextureAddressMode::MirrorRepeat , GL_MIRRORED_REPEAT );
			ic3CaseReturn( ETextureAddressMode::Repeat       , GL_REPEAT          );

		#if( ICFGX_GL_FEATURE_SUPPORT_TEXTURE_EXTENDED_ADDRESS_MODE )
			ic3CaseReturn( ETextureAddressMode::ClampToColor      , GL_CLAMP_TO_BORDER      );
			ic3CaseReturn( ETextureAddressMode::MirrorClampToEdge , GL_MIRROR_CLAMP_TO_EDGE );
		#endif
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::translateGLTextureBindTarget( ETextureClass pTextureDimensionClass )
	{
		switch( pTextureDimensionClass )
		{
			ic3CaseReturn( ETextureClass::Unknown    , GL_INVALID_ENUM                 );
			ic3CaseReturn( ETextureClass::T2D        , GL_TEXTURE_2D                   );
			ic3CaseReturn( ETextureClass::T2DArray   , GL_TEXTURE_2D_ARRAY             );
			ic3CaseReturn( ETextureClass::T2DMS      , GL_TEXTURE_2D_MULTISAMPLE       );
			ic3CaseReturn( ETextureClass::T2DMSArray , GL_TEXTURE_2D_MULTISAMPLE_ARRAY );
			ic3CaseReturn( ETextureClass::T3D        , GL_TEXTURE_3D                   );
			ic3CaseReturn( ETextureClass::TCubeMap   , GL_TEXTURE_CUBE_MAP             );
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::translateGLTextureInternalFormat( ETextureFormat pTextureFormat )
	{
		switch( pTextureFormat )
		{
			ic3CaseReturn( ETextureFormat::UNKNOWN    , GL_INVALID_ENUM );
			ic3CaseReturn( ETextureFormat::R32F       , GL_R32F         );
			ic3CaseReturn( ETextureFormat::R32I       , GL_R32I         );
			ic3CaseReturn( ETextureFormat::R32U       , GL_R32UI        );
			ic3CaseReturn( ETextureFormat::RG32F      , GL_RG32F        );
			ic3CaseReturn( ETextureFormat::RG32I      , GL_RG32I        );
			ic3CaseReturn( ETextureFormat::RG32U      , GL_RG32UI       );
			ic3CaseReturn( ETextureFormat::RGB32F     , GL_RGB32F       );
			ic3CaseReturn( ETextureFormat::RGB32I     , GL_RGB32I       );
			ic3CaseReturn( ETextureFormat::RGB32U     , GL_RGB32UI      );
			ic3CaseReturn( ETextureFormat::RGBA32F    , GL_RGBA32F      );
			ic3CaseReturn( ETextureFormat::RGBA32I    , GL_RGBA32I      );
			ic3CaseReturn( ETextureFormat::RGBA32U    , GL_RGBA32UI     );
			ic3CaseReturn( ETextureFormat::R16F       , GL_R16F         );
			ic3CaseReturn( ETextureFormat::R16I       , GL_R16I         );
			ic3CaseReturn( ETextureFormat::R16U       , GL_R16UI        );
			ic3CaseReturn( ETextureFormat::RG16F      , GL_RG16F        );
			ic3CaseReturn( ETextureFormat::RG16I      , GL_RG16I        );
			ic3CaseReturn( ETextureFormat::RG16U      , GL_RG16UI       );
			ic3CaseReturn( ETextureFormat::RGBA16F    , GL_RGBA16F      );
			ic3CaseReturn( ETextureFormat::RGBA16I    , GL_RGBA16I      );
			ic3CaseReturn( ETextureFormat::RGBA16U    , GL_RGBA16UI     );
			ic3CaseReturn( ETextureFormat::R8I        , GL_R8I          );
			ic3CaseReturn( ETextureFormat::R8U        , GL_R8UI         );
			ic3CaseReturn( ETextureFormat::R8IN       , GL_R8_SNORM     );
			ic3CaseReturn( ETextureFormat::R8UN       , GL_R8           );
			ic3CaseReturn( ETextureFormat::RG8I       , GL_RG8I         );
			ic3CaseReturn( ETextureFormat::RG8U       , GL_RG8UI        );
			ic3CaseReturn( ETextureFormat::RG8IN      , GL_RG8_SNORM    );
			ic3CaseReturn( ETextureFormat::RG8UN      , GL_RG8          );
		#if( ICFGX_GL_FEATURE_SUPPORT_TEXTURE_FORMAT_BGR )
			ic3CaseReturn( ETextureFormat::BGRX8UN    , GL_RGB8         );
			ic3CaseReturn( ETextureFormat::BGRX8SRGB  , GL_SRGB8        );
			ic3CaseReturn( ETextureFormat::BGRA8UN    , GL_RGBA8        );
			ic3CaseReturn( ETextureFormat::BGRA8SRGB  , GL_SRGB8_ALPHA8 );
		#endif
			ic3CaseReturn( ETextureFormat::RGBA8I     , GL_RGBA8I       );
			ic3CaseReturn( ETextureFormat::RGBA8U     , GL_RGBA8UI      );
			ic3CaseReturn( ETextureFormat::RGBA8IN    , GL_RGBA8_SNORM  );
			ic3CaseReturn( ETextureFormat::RGBA8UN    , GL_RGBA8        );
			ic3CaseReturn( ETextureFormat::RGBA8SRGB  , GL_SRGB8_ALPHA8 );

			ic3CaseReturn( ETextureFormat::R5G5B5A1   , GL_RGB5_A1            );
			ic3CaseReturn( ETextureFormat::R5G6B5     , GL_RGB565             );
			ic3CaseReturn( ETextureFormat::R9G9B9E5   , GL_RGB9_E5            );
			ic3CaseReturn( ETextureFormat::RGB10A2U   , GL_RGB10_A2UI         );
			ic3CaseReturn( ETextureFormat::RGB10A2UN  , GL_RGB10_A2           );
			ic3CaseReturn( ETextureFormat::R11G11B10F , GL_R11F_G11F_B10F     );
			ic3CaseReturn( ETextureFormat::D16UN      , GL_DEPTH_COMPONENT16  );
			ic3CaseReturn( ETextureFormat::D24UNS8U   , GL_DEPTH24_STENCIL8   );
			ic3CaseReturn( ETextureFormat::D24UNX8    , GL_DEPTH_COMPONENT24  );
			ic3CaseReturn( ETextureFormat::X24S8U     , GL_DEPTH24_STENCIL8   );
			ic3CaseReturn( ETextureFormat::D32F       , GL_DEPTH_COMPONENT32F );
		#if( ICFGX_GL_FEATURE_SUPPORT_TEXTURE_FORMAT_COMPRESSED_BCX )
			ic3CaseReturn( ETextureFormat::BC1        , GL_TEX_FORMAT_COMPRESSED_BC1         );
			ic3CaseReturn( ETextureFormat::BC1SRGB    , GL_TEX_FORMAT_COMPRESSED_BC1_SRGBA   );
			ic3CaseReturn( ETextureFormat::BC2        , GL_TEX_FORMAT_COMPRESSED_BC2         );
			ic3CaseReturn( ETextureFormat::BC2SRGB    , GL_TEX_FORMAT_COMPRESSED_BC2_SRGB    );
			ic3CaseReturn( ETextureFormat::BC3        , GL_TEX_FORMAT_COMPRESSED_BC3         );
			ic3CaseReturn( ETextureFormat::BC3SRGB    , GL_TEX_FORMAT_COMPRESSED_BC3_SRGB    );
			ic3CaseReturn( ETextureFormat::BC4IN      , GL_TEX_FORMAT_COMPRESSED_BC4_SNORM   );
			ic3CaseReturn( ETextureFormat::BC4UN      , GL_TEX_FORMAT_COMPRESSED_BC4_UNORM   );
			ic3CaseReturn( ETextureFormat::BC5IN      , GL_TEX_FORMAT_COMPRESSED_BC5_SNORM   );
			ic3CaseReturn( ETextureFormat::BC5UN      , GL_TEX_FORMAT_COMPRESSED_BC5_UNORM   );
			ic3CaseReturn( ETextureFormat::BC6HSF     , GL_TEX_FORMAT_COMPRESSED_BC6H_SFLOAT );
			ic3CaseReturn( ETextureFormat::BC6HUF     , GL_TEX_FORMAT_COMPRESSED_BC6H_UFLOAT );
			ic3CaseReturn( ETextureFormat::BC7        , GL_TEX_FORMAT_COMPRESSED_BC7         );
			ic3CaseReturn( ETextureFormat::BC7SRGB    , GL_TEX_FORMAT_COMPRESSED_BC7_SRGB    );
		#endif
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::translateGLTexturePixelDataLayout( ETextureFormat pTextureFormat )
	{
		switch( pTextureFormat )
		{
			ic3CaseReturn( ETextureFormat::UNKNOWN    , GL_INVALID_ENUM );
			ic3CaseReturn( ETextureFormat::R32F       , GL_RED  );
			ic3CaseReturn( ETextureFormat::R32I       , GL_RED  );
			ic3CaseReturn( ETextureFormat::R32U       , GL_RED  );
			ic3CaseReturn( ETextureFormat::RG32F      , GL_RG   );
			ic3CaseReturn( ETextureFormat::RG32I      , GL_RG   );
			ic3CaseReturn( ETextureFormat::RG32U      , GL_RG   );
			ic3CaseReturn( ETextureFormat::RGB32F     , GL_RGB  );
			ic3CaseReturn( ETextureFormat::RGB32I     , GL_RGB  );
			ic3CaseReturn( ETextureFormat::RGB32U     , GL_RGB  );
			ic3CaseReturn( ETextureFormat::RGBA32F    , GL_RGBA );
			ic3CaseReturn( ETextureFormat::RGBA32I    , GL_RGBA );
			ic3CaseReturn( ETextureFormat::RGBA32U    , GL_RGBA );
			ic3CaseReturn( ETextureFormat::R16F       , GL_RED  );
			ic3CaseReturn( ETextureFormat::R16I       , GL_RED  );
			ic3CaseReturn( ETextureFormat::R16U       , GL_RED  );
			ic3CaseReturn( ETextureFormat::RG16F      , GL_RG   );
			ic3CaseReturn( ETextureFormat::RG16I      , GL_RG   );
			ic3CaseReturn( ETextureFormat::RG16U      , GL_RG   );
			ic3CaseReturn( ETextureFormat::RGBA16F    , GL_RGBA );
			ic3CaseReturn( ETextureFormat::RGBA16I    , GL_RGBA );
			ic3CaseReturn( ETextureFormat::RGBA16U    , GL_RGBA );
			ic3CaseReturn( ETextureFormat::R8I        , GL_RED  );
			ic3CaseReturn( ETextureFormat::R8U        , GL_RED  );
			ic3CaseReturn( ETextureFormat::R8IN       , GL_RED  );
			ic3CaseReturn( ETextureFormat::R8UN       , GL_RED  );
			ic3CaseReturn( ETextureFormat::RG8I       , GL_RG   );
			ic3CaseReturn( ETextureFormat::RG8U       , GL_RG   );
			ic3CaseReturn( ETextureFormat::RG8IN      , GL_RG   );
			ic3CaseReturn( ETextureFormat::RG8UN      , GL_RG   );
		#if( ICFGX_GL_FEATURE_SUPPORT_TEXTURE_FORMAT_BGR )
			ic3CaseReturn( ETextureFormat::BGRX8UN    , GL_BGR  );
			ic3CaseReturn( ETextureFormat::BGRX8SRGB  , GL_BGR  );
			ic3CaseReturn( ETextureFormat::BGRA8UN    , GL_BGRA );
			ic3CaseReturn( ETextureFormat::BGRA8SRGB  , GL_BGRA );
		#endif
			ic3CaseReturn( ETextureFormat::RGBA8I     , GL_RGBA );
			ic3CaseReturn( ETextureFormat::RGBA8U     , GL_RGBA );
			ic3CaseReturn( ETextureFormat::RGBA8IN    , GL_RGBA );
			ic3CaseReturn( ETextureFormat::RGBA8UN    , GL_RGBA );
			ic3CaseReturn( ETextureFormat::RGBA8SRGB  , GL_RGBA );
			ic3CaseReturn( ETextureFormat::R5G5B5A1   , GL_RGB5_A1            );
			ic3CaseReturn( ETextureFormat::R5G6B5     , GL_RGB565             );
			ic3CaseReturn( ETextureFormat::R9G9B9E5   , GL_RGB9_E5            );
			ic3CaseReturn( ETextureFormat::RGB10A2U   , GL_RGB10_A2UI         );
			ic3CaseReturn( ETextureFormat::RGB10A2UN  , GL_RGB10_A2           );
			ic3CaseReturn( ETextureFormat::R11G11B10F , GL_R11F_G11F_B10F     );
			ic3CaseReturn( ETextureFormat::D16UN      , GL_DEPTH_COMPONENT16  );
			ic3CaseReturn( ETextureFormat::D24UNS8U   , GL_DEPTH_STENCIL      );
			ic3CaseReturn( ETextureFormat::D24UNX8    , GL_DEPTH_STENCIL      );
			ic3CaseReturn( ETextureFormat::X24S8U     , GL_DEPTH_STENCIL      );
			ic3CaseReturn( ETextureFormat::D32F       , GL_DEPTH_COMPONENT    );
		#if( ICFGX_GL_FEATURE_SUPPORT_TEXTURE_FORMAT_COMPRESSED_BCX )
			ic3CaseReturn( ETextureFormat::BC1        , GL_TEX_FORMAT_COMPRESSED_BC1         );
			ic3CaseReturn( ETextureFormat::BC1SRGB    , GL_TEX_FORMAT_COMPRESSED_BC1_SRGBA   );
			ic3CaseReturn( ETextureFormat::BC2        , GL_TEX_FORMAT_COMPRESSED_BC2         );
			ic3CaseReturn( ETextureFormat::BC2SRGB    , GL_TEX_FORMAT_COMPRESSED_BC2_SRGB    );
			ic3CaseReturn( ETextureFormat::BC3        , GL_TEX_FORMAT_COMPRESSED_BC3         );
			ic3CaseReturn( ETextureFormat::BC3SRGB    , GL_TEX_FORMAT_COMPRESSED_BC3_SRGB    );
			ic3CaseReturn( ETextureFormat::BC4IN      , GL_TEX_FORMAT_COMPRESSED_BC4_SNORM   );
			ic3CaseReturn( ETextureFormat::BC4UN      , GL_TEX_FORMAT_COMPRESSED_BC4_UNORM   );
			ic3CaseReturn( ETextureFormat::BC5IN      , GL_TEX_FORMAT_COMPRESSED_BC5_SNORM   );
			ic3CaseReturn( ETextureFormat::BC5UN      , GL_TEX_FORMAT_COMPRESSED_BC5_UNORM   );
			ic3CaseReturn( ETextureFormat::BC6HSF     , GL_TEX_FORMAT_COMPRESSED_BC6H_SFLOAT );
			ic3CaseReturn( ETextureFormat::BC6HUF     , GL_TEX_FORMAT_COMPRESSED_BC6H_UFLOAT );
			ic3CaseReturn( ETextureFormat::BC7        , GL_TEX_FORMAT_COMPRESSED_BC7         );
			ic3CaseReturn( ETextureFormat::BC7SRGB    , GL_TEX_FORMAT_COMPRESSED_BC7_SRGB    );
		#endif
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

	GLenum ATL::translateGLTriangleVerticesOrder( ETriangleVerticesOrder pVerticesOrder )
	{
		switch( pVerticesOrder )
		{
			ic3CaseReturn( ETriangleVerticesOrder::Clockwise        , GL_CW  );
			ic3CaseReturn( ETriangleVerticesOrder::CounterClockwise , GL_CCW );
		}
		return GL_IC3_ERR_INVALID_PARAM;
	}

	const char * ATL::translateGLDebugOutputExtensionName( GLDebugOutputVersion pDebugOutputVersion )
	{
		switch( pDebugOutputVersion )
		{
			ic3CaseReturn( GLDebugOutputVersion::Unknown , "" );
			ic3CaseReturn( GLDebugOutputVersion::AMDExt  , "GL_AMD_debug_output" );
			ic3CaseReturn( GLDebugOutputVersion::ARBExt  , "GL_ARB_debug_output" );
			ic3CaseReturn( GLDebugOutputVersion::KHRCore , "GL_KHR_debug"        );
		}
		return "Unknown";
	}

	const char * ATL::translateGLDebugEventCategoryStrAMD( GLenum pEventCategory )
	{
	#if( ICFGX_GL_FEATURE_SUPPORT_DEBUG_OUTPUT )
		switch( pEventCategory )
		{
			ic3CaseReturn( GL_DEBUG_CATEGORY_API_ERROR_AMD          , "API error"          );
			ic3CaseReturn( GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD      , "Window system"      );
			ic3CaseReturn( GL_DEBUG_CATEGORY_DEPRECATION_AMD        , "Deprecation"        );
			ic3CaseReturn( GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD , "Undefined behavior" );
			ic3CaseReturn( GL_DEBUG_CATEGORY_PERFORMANCE_AMD        , "Performance"        );
			ic3CaseReturn( GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD    , "Shader compiler"    );
			ic3CaseReturn( GL_DEBUG_CATEGORY_APPLICATION_AMD        , "Application"        );
			ic3CaseReturn( GL_DEBUG_CATEGORY_OTHER_AMD              , "Other"              );
		}
	#endif
		return "Unknown";
	}

	const char * ATL::translateGLDebugEventSeverityStr( GLenum pEventSeverity )
	{
	#if( ICFGX_GL_FEATURE_SUPPORT_DEBUG_OUTPUT )
	    switch( pEventSeverity )
		{
			ic3CaseReturn( GL_DEBUG_SEVERITY_HIGH_ARB     , "High"         );
			ic3CaseReturn( GL_DEBUG_SEVERITY_MEDIUM_ARB   , "Medium"       );
			ic3CaseReturn( GL_DEBUG_SEVERITY_LOW_ARB      , "Low"          );
			ic3CaseReturn( GL_DEBUG_SEVERITY_NOTIFICATION , "Notification" );
		}
	#endif
		return "Unknown";
	}

	const char * ATL::translateGLDebugEventSourceStr( GLenum pEventSource )
	{
	#if( ICFGX_GL_FEATURE_SUPPORT_DEBUG_OUTPUT )
	    switch( pEventSource )
		{
			ic3CaseReturn( GL_DEBUG_SOURCE_API_ARB             , "GL API"          );
			ic3CaseReturn( GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB   , "Window system"   );
			ic3CaseReturn( GL_DEBUG_SOURCE_SHADER_COMPILER_ARB , "Shader compiler" );
			ic3CaseReturn( GL_DEBUG_SOURCE_THIRD_PARTY_ARB     , "Third party"     );
			ic3CaseReturn( GL_DEBUG_SOURCE_APPLICATION_ARB     , "Application"     );
			ic3CaseReturn( GL_DEBUG_SOURCE_OTHER_ARB           , "Other"           );
		}
	#endif
		return "Unknown";
	}

	const char * ATL::translateGLDebugEventTypeStr( GLenum pEventType )
	{
	#if( ICFGX_GL_FEATURE_SUPPORT_DEBUG_OUTPUT )
	    switch( pEventType )
		{
			ic3CaseReturn( GL_DEBUG_TYPE_ERROR_ARB               , "GL API"          );
			ic3CaseReturn( GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB , "Window system"   );
			ic3CaseReturn( GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB  , "Shader compiler" );
			ic3CaseReturn( GL_DEBUG_TYPE_PORTABILITY_ARB         , "Third party"     );
			ic3CaseReturn( GL_DEBUG_TYPE_PERFORMANCE_ARB         , "Application"     );
			ic3CaseReturn( GL_DEBUG_TYPE_OTHER_ARB               , "Other"           );
		}
	#endif
		return "Unknown";
	}

} // namespace Ic3::Graphics::GCI
