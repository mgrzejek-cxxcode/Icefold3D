
#include "../Prerequisites.h"

namespace Ic3::Graphics::GCI
{

	Ic3TypeInfoEnumDefine( EBaseDataType )
	{
		Ic3TypeInfoEnumBegin( EBaseDataType );
		Ic3TypeInfoEnumRegisterClassConstant( EBaseDataType::Undefined );
		Ic3TypeInfoEnumRegisterClassConstant( EBaseDataType::Bool      );
		Ic3TypeInfoEnumRegisterClassConstant( EBaseDataType::Byte      );
		Ic3TypeInfoEnumRegisterClassConstant( EBaseDataType::Ubyte     );
		Ic3TypeInfoEnumRegisterClassConstant( EBaseDataType::Int16     );
		Ic3TypeInfoEnumRegisterClassConstant( EBaseDataType::Uint16    );
		Ic3TypeInfoEnumRegisterClassConstant( EBaseDataType::Int32     );
		Ic3TypeInfoEnumRegisterClassConstant( EBaseDataType::Uint32    );
		Ic3TypeInfoEnumRegisterClassConstant( EBaseDataType::Float16   );
		Ic3TypeInfoEnumRegisterClassConstant( EBaseDataType::Float32   );
		Ic3TypeInfoEnumRegisterClassConstant( EBaseDataType::Double    );
		Ic3TypeInfoEnumRegisterClassConstant( EBaseDataType::Uint24S8  );
	}

	Ic3TypeInfoEnumDefine( EIndexDataFormat )
	{
		Ic3TypeInfoEnumBegin( EIndexDataFormat );
		Ic3TypeInfoEnumRegisterClassConstant( EIndexDataFormat::Undefined );
		Ic3TypeInfoEnumRegisterClassConstant( EIndexDataFormat::Uint16 );
		Ic3TypeInfoEnumRegisterClassConstant( EIndexDataFormat::Uint32 );
	}

	Ic3TypeInfoEnumDefine( ETextureFormat )
	{
		Ic3TypeInfoEnumBegin( ETextureFormat );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::R32F    );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::R32I    );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::R32U    );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RG32F   );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RG32I   );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RG32U   );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RGB32F  );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RGB32I  );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RGB32U  );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RGBA32F );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RGBA32I );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RGBA32U );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::R16F    );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::R16I    );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::R16U    );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RG16F   );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RG16I   );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RG16U   );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RGBA16F );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RGBA16I );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RGBA16U );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::R8I        );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::R8U        );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::R8IN       );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::R8UN       );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RG8I       );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RG8U       );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RG8IN      );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RG8UN      );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::BGRX8UN    );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::BGRX8SRGB  );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::BGRA8UN    );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::BGRA8SRGB  );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RGBA8I     );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RGBA8U     );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RGBA8IN    );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RGBA8UN    );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RGBA8SRGB  );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RGBX8UN    );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::R5G5B5A1   );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::R5G6B5     );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::R9G9B9E5   );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RGB10A2U   );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::RGB10A2UN  );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::R11G11B10F );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::D16UN      );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::D24UNS8U   );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::D24UNX8    );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::X24S8U     );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::D32F       );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::D32FS8     );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::BC1     );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::BC1SRGB );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::BC2     );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::BC2SRGB );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::BC3     );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::BC3SRGB );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::BC4IN   );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::BC4UN   );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::BC5IN   );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::BC5UN   );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::BC6HSF  );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::BC6HUF  );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::BC7     );
		Ic3TypeInfoEnumRegisterClassConstant( ETextureFormat::BC7SRGB );
	}

	Ic3TypeInfoEnumDefine( EVertexAttribFormat )
	{
		Ic3TypeInfoEnumBegin( EVertexAttribFormat );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Undefined );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::F16      );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::F32      );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::I8       );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::I16      );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::I32      );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::U8       );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::U16      );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::U32      );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::I8N      );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::I16N     );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::U8N      );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::U16N     );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec2F16  );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec2F32  );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec2I8   );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec2I16  );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec2I32  );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec2U8   );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec2U16  );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec2U32  );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec2I8N  );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec2I16N );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec2U8N  );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec2U16N );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec3F32  );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec3I32  );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec3U32  );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec4F16  );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec4F32  );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec4I8   );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec4I16  );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec4I32  );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec4U8   );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec4U16  );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec4U32  );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec4I8N  );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec4I16N );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec4U8N  );
		Ic3TypeInfoEnumRegisterClassConstant( EVertexAttribFormat::Vec4U16N );
	}

} // namespace Ic3::Graphics::GCI
