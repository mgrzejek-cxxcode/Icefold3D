
#include <Ic3/System/FileManager.h>
#include <Ic3/System/SysContextNative.h>
#include <Ic3/Script/Lua/LuaScriptSystem.h>
#include <Ic3/Script/Lua/RefData.h>
#include <Ic3/Script/ScriptModule.h>

#include <cppx/stringUtils.h>

#include <Ic3/NxMain/GCI/VertexFormatDescriptor.h>
#include <Ic3/NxMain/GCI/VertexFormatDescriptorUtils.h>

int main( int pArgc, const char ** pArgv )
{
	{
		using namespace Ic3;

		VertexFormatDescriptorBuilder vfdBuilder{};
		vfdBuilder.addAttribute( 0, cxStandardVertexAttributeKeyPosition );
		vfdBuilder.addAttribute( 0, cxStandardVertexAttributeKeyNormal );
		vfdBuilder.addAttribute( 0, cxStandardVertexAttributeKeyTangent );
		vfdBuilder.addAttribute( 0, cxStandardVertexAttributeKeyBiTangent );
		vfdBuilder.addAttribute( 0, cxStandardVertexAttributeKeyTexCoord0 );
		vfdBuilder.addAttribute( 0, 6, { "CustomPosRef" }, GCI::EVertexAttribFormat::Vec4F32 );
		vfdBuilder.addAttribute( 1, cxStandardVertexAttributeKeyInstanceMatrix );
		vfdBuilder.addAttribute( 2, cxStandardVertexAttributeKeyInstanceUserData );
		auto vfd = vfdBuilder.createVertexFormatDescriptor( GCI::EPrimitiveTopology::TriangleList, GCI::EIndexDataFormat::Uint32 );

		const auto sid1 = vfd->generateVertexFormatStringID();
		const auto defArray0 = vfd->generateAttributeDefinitionArray();
		const auto defArray1 = VertexFormatDescriptorBuilder::parseVertexFormatString( sid1 );

		VertexFormatDescriptorBuilder vfdBuilder0{};
		vfdBuilder0.addAttributeList( cppx::bind_array_view( defArray0 ) );
		auto vfdCopy0 = vfdBuilder0.createVertexFormatDescriptor( GCI::EPrimitiveTopology::TriangleList, GCI::EIndexDataFormat::Uint32 );
		const auto sidCopy0 = vfd->generateVertexFormatStringID();

		VertexFormatDescriptorBuilder vfdBuilder1{};
		vfdBuilder1.addAttributeList( cppx::bind_array_view( defArray1 ) );
		auto vfdCopy1 = vfdBuilder1.createVertexFormatDescriptor( GCI::EPrimitiveTopology::TriangleList, GCI::EIndexDataFormat::Uint32 );
		const auto sidCopy1 = vfd->generateVertexFormatStringID();

		Ic3DebugOutput( sid1.c_str() );
		Ic3DebugOutput( sidCopy0.c_str() );
		Ic3DebugOutput( sidCopy1.c_str() );

		return 0;
	}

	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	// "s0<a0POSITION:3F32|a1NORMAL:3F32|a2TANGENT:3F32|a3BITANGENT:3F32|a5TEXCOORD0:2F32>_s1<a10INSTANCE_MATRIX:4F32[4]>_s2<a14INSTANCE_UDATA:2F32[2]>"

	printf("%u\n", (uint32)sizeof(std::exception_ptr));

	Ic3::System::SysContextCreateInfo sysContextCreateInfo;
	auto sysContext = Ic3::System::Platform::CreateSysContext( sysContextCreateInfo );

	auto * luaScriptSystem = new Ic3::Script::LuaScriptSystem( sysContext );
	Ic3::Script::ScriptSystem * scriptSystem = luaScriptSystem;
	luaScriptSystem->initialize();

	const std::string_view sv = "Mati";
	const auto svs = sv.size();

	using TMI = Ic3::Script::SmTypeMetaInfo<Ic3::Script::SomeInterface::ExampleClass>;
	const auto & tmiName = TMI::sName;

//	cppx::dynamic_byte_array luaScript;
//	auto luaFile = fsManager->openFile( "sx.lua", Ic3::System::EFileOpenMode::ReadOnly );
//	luaFile->readAuto( luaScript );
//
//	std::string scriptText{};
//	scriptText.assign( ( const char * )luaScript.data(), luaScript.size() );

	// auto compiledScript = luaScriptSystem->compileFile( "sx2", "sx2.lua" );

	Ic3::Script::RefData::registerType( luaScriptSystem );

	luaL_dofile( luaScriptSystem->getLuaContext()->luaState, "sx.lua" );
	luaL_dofile( luaScriptSystem->getLuaContext()->luaState, "sx2.lua" );

	// scriptSystem->executeCompiledScript( compiledScript );

//	CompiledScript* cs = nullptr;
//
//	File ux_file = File::Open("sx.lua", FileOpenMode::Open_Existing);
//
//	if(ux_file)
//	{
//		DynamicDataArray<Byte> uxd;
//		ux_file.ReadAll(uxd);
//
//		cs = sce.CompileScript("", uxd.GetDataPtr(), uxd.Size());
//
//		sce.Execute(cs);
//		sce.Parse(String((const char*)uxd.GetDataPtr(), uxd.Size()));
//	}

	return 0;
}
