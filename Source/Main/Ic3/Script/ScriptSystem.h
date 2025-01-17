
#ifndef __IC3_SCRIPT_SCRIPT_SYSTEM_H__
#define __IC3_SCRIPT_SCRIPT_SYSTEM_H__

#include "Prerequisites.h"
#include <cppx/byteArray.h>
#include <Ic3/System/FileCommon.h>

namespace Ic3::Script
{

	struct CompiledScript
	{
		std::string name;
		cppx::dynamic_byte_array binary;

		explicit operator bool() const noexcept
		{
			return !name.empty() && !binary.empty();
		}
	};

	class ScriptSystem : public IDynamicObject
	{
	public:
		System::SysContextHandle const mSysContext;

	public:
		explicit ScriptSystem( System::SysContextHandle pSysContext );
		virtual ~ScriptSystem() = default;

		virtual void initialize();
		virtual void release();

		[[nodiscard]] virtual const void * getContext() const noexcept = 0;

		virtual bool compileSource( const char * pName, const void * pSource, size_t pLength, cppx::dynamic_byte_array & pOutput ) noexcept = 0;

		virtual bool executeCompiledScript( const char * pName, const void * pBinary, size_t pLength ) noexcept = 0;

		virtual bool executeTextScript( const char * pName, const char * pSource, size_t pLength ) noexcept = 0;

		CompiledScript compileFile( const char * pName, const char * pFileName );

		CompiledScript compileScript( const char * pName, const cppx::dynamic_byte_array & pScriptSource );

		bool executeCompiledScript( const CompiledScript & pCompiledScript );

		template <typename TContext>
		[[nodiscard]] const TContext * getContextAs() const noexcept
		{
			return static_cast<const TContext *>( getContext() );
		}

	protected:
		System::FileManagerHandle const _sysFileManager;
	};

} // namespace Ic3::Script

#endif //__IC3_SCRIPT_SCRIPT_SYSTEM_H__
