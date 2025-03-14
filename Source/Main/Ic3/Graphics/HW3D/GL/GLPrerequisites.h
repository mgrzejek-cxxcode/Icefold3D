
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_PREREQUISITES_H__
#define __IC3_GRAPHICS_HW3D_GLC_PREREQUISITES_H__

#include <Ic3/Graphics/GCI/Memory/CommonGPUMemoryDefs.h>
#include <Ic3/System/Gfx/OpenGLDriver.h>

#define IC3_GX_GL_PLATFORM_TYPE_DESKTOP 0x7100
#define IC3_GX_GL_PLATFORM_TYPE_ES   0x7200

#define IC3_GX_GL_TARGET_GL32 0x8100
#define IC3_GX_GL_TARGET_GL43 0x8200
#define IC3_GX_GL_TARGET_ES31 0x8300

#define IC3_GX_GL_OPTION_FORCE_DEBUG_OUTPUT 0

#if( IC3_SYSTEM_GL_PLATFORM_TYPE == IC3_SYSTEM_GL_PLATFORM_TYPE_DESKTOP )
#  define IC3_GX_GL_PLATFORM_TYPE IC3_GX_GL_PLATFORM_TYPE_DESKTOP
#elif( IC3_SYSTEM_GL_PLATFORM_TYPE == IC3_SYSTEM_GL_PLATFORM_TYPE_ES )
#  define IC3_GX_GL_PLATFORM_TYPE IC3_GX_GL_PLATFORM_TYPE_ES
#else
#  if( !GL_PLATFORM_TYPE_DESKTOP && GL_PLATFORM_TYPE_ES )
#    define IC3_GX_GL_PLATFORM_TYPE IC3_GX_GL_PLATFORM_TYPE_ES
#  elif( GL_PLATFORM_TYPE_DESKTOP )
#    define IC3_GX_GL_PLATFORM_TYPE IC3_GX_GL_PLATFORM_TYPE_DESKTOP
#  else
#    error "Invalid GL Platform Type"
#  endif
#endif

#if !defined( IC3_GX_GL_TARGET )
#  if( IC3_GX_GL_PLATFORM_TYPE == IC3_GX_GL_PLATFORM_TYPE_DESKTOP )
#    if defined( IC3_GRAPHICS_HW3D_GL3_BUILD )
#      define IC3_GX_GL_TARGET IC3_GX_GL_TARGET_GL32
#    elif defined( IC3_GRAPHICS_HW3D_GL4_BUILD )
#      define IC3_GX_GL_TARGET IC3_GX_GL_TARGET_GL43
#    endif
#  elif( IC3_GX_GL_PLATFORM_TYPE == IC3_GX_GL_PLATFORM_TYPE_ES )
#    if defined( IC3_GRAPHICS_HW3D_GLES_BUILD )
#      define IC3_GX_GL_TARGET IC3_GX_GL_TARGET_ES31
#    endif
#  endif
#endif

#if( IC3_GX_GL_OPTION_FORCE_DEBUG_OUTPUT )
#  define IC3_GX_GL_FEATURE_SUPPORT_DEBUG_OUTPUT 1
#endif

#if !defined( IC3_GX_GL_FEATURE_SUPPORT_DEBUG_OUTPUT )
#  define IC3_GX_GL_FEATURE_SUPPORT_DEBUG_OUTPUT \
		( IC3_SYSTEM_GL_PLATFORM_TYPE == IC3_SYSTEM_GL_PLATFORM_TYPE_DESKTOP )
#endif

#if !defined( IC3_GX_GL_FEATURE_SUPPORT_BUFFER_PERSISTENT_MAP )
#  define IC3_GX_GL_FEATURE_SUPPORT_BUFFER_PERSISTENT_MAP \
		( IC3_GX_GL_TARGET == IC3_GX_GL_TARGET_GL43 )
#endif

#if !defined( IC3_GX_GL_FEATURE_SUPPORT_PRIMITIVE_FILL_MODE )
#  define IC3_GX_GL_FEATURE_SUPPORT_PRIMITIVE_FILL_MODE \
		( IC3_SYSTEM_GL_PLATFORM_TYPE == IC3_SYSTEM_GL_PLATFORM_TYPE_DESKTOP )
#endif

#if !defined( IC3_GX_GL_FEATURE_SUPPORT_PRIMITIVE_TYPE_ADJACENCY )
#  define IC3_GX_GL_FEATURE_SUPPORT_PRIMITIVE_TYPE_ADJACENCY \
		( IC3_SYSTEM_GL_PLATFORM_TYPE == IC3_SYSTEM_GL_PLATFORM_TYPE_DESKTOP )
#endif

#if !defined( IC3_GX_GL_FEATURE_SUPPORT_PRIMITIVE_TYPE_PATCHES )
#  define IC3_GX_GL_FEATURE_SUPPORT_PRIMITIVE_TYPE_PATCHES \
		( IC3_GX_GL_TARGET == IC3_GX_GL_TARGET_GL43 )
#endif

#if !defined( IC3_GX_GL_FEATURE_SUPPORT_SHADER_PIPELINE_OBJECT )
#  define IC3_GX_GL_FEATURE_SUPPORT_SHADER_PIPELINE_OBJECT \
		( ( IC3_GX_GL_TARGET == IC3_GX_GL_TARGET_GL43 ) || ( IC3_GX_GL_TARGET == IC3_GX_GL_TARGET_ES31 ) )
#endif

#if !defined( IC3_GX_GL_FEATURE_SUPPORT_SHADER_TYPE_GEOMETRY )
#  define IC3_GX_GL_FEATURE_SUPPORT_SHADER_TYPE_GEOMETRY \
		( IC3_SYSTEM_GL_PLATFORM_TYPE == IC3_SYSTEM_GL_PLATFORM_TYPE_DESKTOP )
#endif

#if !defined( IC3_GX_GL_FEATURE_SUPPORT_SHADER_TYPE_TESSELATION )
#  define IC3_GX_GL_FEATURE_SUPPORT_SHADER_TYPE_TESSELATION \
		( IC3_GX_GL_TARGET == IC3_GX_GL_TARGET_GL43 )
#endif

#if !defined( IC3_GX_GL_FEATURE_SUPPORT_SHADER_TYPE_COMPUTE )
#  define IC3_GX_GL_FEATURE_SUPPORT_SHADER_TYPE_COMPUTE \
		( ( IC3_GX_GL_TARGET == IC3_GX_GL_TARGET_GL43 ) || ( IC3_GX_GL_TARGET == IC3_GX_GL_TARGET_ES31 ) )
#endif

#if !defined( IC3_GX_GL_FEATURE_SUPPORT_TEXTURE_ANISOTROPIC_FILTER )
#  define IC3_GX_GL_FEATURE_SUPPORT_TEXTURE_ANISOTROPIC_FILTER \
		( IC3_SYSTEM_GL_PLATFORM_TYPE == IC3_SYSTEM_GL_PLATFORM_TYPE_DESKTOP )
#endif

#if !defined( IC3_GX_GL_FEATURE_SUPPORT_TEXTURE_EXTENDED_ADDRESS_MODE )
#  define IC3_GX_GL_FEATURE_SUPPORT_TEXTURE_EXTENDED_ADDRESS_MODE \
		( IC3_SYSTEM_GL_PLATFORM_TYPE == IC3_SYSTEM_GL_PLATFORM_TYPE_DESKTOP )
#endif

#if !defined( IC3_GX_GL_FEATURE_SUPPORT_TEXTURE_FORMAT_BGR )
#  define IC3_GX_GL_FEATURE_SUPPORT_TEXTURE_FORMAT_BGR \
		( IC3_SYSTEM_GL_PLATFORM_TYPE == IC3_SYSTEM_GL_PLATFORM_TYPE_DESKTOP )
#endif

#if !defined( IC3_GX_GL_FEATURE_SUPPORT_TEXTURE_FORMAT_COMPRESSED_BCX )
#  define IC3_GX_GL_FEATURE_SUPPORT_TEXTURE_FORMAT_COMPRESSED_BCX \
		( IC3_SYSTEM_GL_PLATFORM_TYPE == IC3_SYSTEM_GL_PLATFORM_TYPE_DESKTOP )
#endif


#if( IC3_GX_GL_FEATURE_SUPPORT_DEBUG_OUTPUT )
#  define IC3_GX_GL_ENABLE_EXPLICIT_ERROR_CHECKS 0
#endif

#if !defined( IC3_GX_GL_ENABLE_EXPLICIT_ERROR_CHECKS )
#  define IC3_GX_GL_ENABLE_EXPLICIT_ERROR_CHECKS 1
#endif


#if( IC3_GX_GL_ENABLE_EXPLICIT_ERROR_CHECKS )
#  define Ic3OpenGLCheckLastResult()            Ic3SysOpenGLCheckLastResult()
#  define Ic3OpenGLCheckLastError( pErrorCode ) Ic3SysOpenGLCheckLastError( pErrorCode )
#  define Ic3OpenGLHandleLastError()            Ic3SysOpenGLHandleLastError()
#  define Ic3OpenGLResetErrorQueue()            Ic3SysOpenGLResetErrorQueue()
#else
#  define Ic3OpenGLCheckLastResult()
#  define Ic3OpenGLCheckLastError( pErrorCode )
#  define Ic3OpenGLHandleLastError()
#  define Ic3OpenGLResetErrorQueue()
#endif

#if( IC3_BUILD_STATIC )
#  define IC3_GX_GL_API
#  define IC3_GX_GL_CLASS
#  define IC3_GX_GL_OBJ    extern
#else
#  if( IC3_GRAPHICS_HW3D_GL_BUILD )
#    define IC3_GX_GL_API    PCL_ATTR_DLL_EXPORT
#    define IC3_GX_GL_CLASS  PCL_ATTR_DLL_EXPORT
#    define IC3_GX_GL_OBJ    PCL_ATTR_DLL_EXPORT extern
#  else
#    define IC3_GX_GL_API    PCL_ATTR_DLL_IMPORT
#    define IC3_GX_GL_CLASS  PCL_ATTR_DLL_IMPORT
#    define IC3_GX_GL_OBJ    PCL_ATTR_DLL_IMPORT extern
#  endif
#endif

#include "Prerequisites/GLCommonDefs.h"
#include "Prerequisites/GLDebugOutput.h"
#include "Prerequisites/GLObject.h"

namespace Ic3::Graphics::GCI
{

	class RootSignatureDescriptorGeneric;

	class GLBlendStateDescriptor;
	class GLDepthStencilStateDescriptor;
	class GLRasterizerStateDescriptor;
	class GLGraphicsShaderLinkageDescriptor;
	class GLVertexAttributeLayoutDescriptor;
	class GLRenderTargetBindingCompiledState;
	class GLVertexSourceBindingDescriptor;

	Ic3GCIDeclareClassHandle( GLCommandList );
	Ic3GCIDeclareClassHandle( GLCommandSystem );
	Ic3GCIDeclareClassHandle( GLGPUDevice );
	Ic3GCIDeclareClassHandle( GLGPUDriver );
	Ic3GCIDeclareClassHandle( GLPresentationLayer );

	Ic3GCIDeclareTypedefHandle( GLRootSignatureDescriptor, RootSignatureDescriptorGeneric );

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_PREREQUISITES_H__
