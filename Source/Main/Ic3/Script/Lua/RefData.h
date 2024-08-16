
#pragma once

#ifndef __IC3_SCRIPT_REF_DATA_H__
#define __IC3_SCRIPT_REF_DATA_H__

#include "LuaPrerequisites.h"

namespace Ic3::Script
{

	class ScriptSystem;

	class RefData
	{
	public:
		enum class Color
		{
			None,
			White,
			Black,
			Green
		};

		std::string name;
		uint64 uid;

		int value = 0;
		double multiplier = 1.0;

	private:
		std::vector<std::string>	_data;

		static int _cnt;

	public:
		RefData( const char * pName = "default" );
		~RefData();

		void add( const char * pStr );
		void addTwo( const char * pStr1, const char * pStr2 );
		void print();

		bool equals( RefData & pOther ) const;

		static int getCnt();

		[[nodiscard]] size_t getSize() const;
		[[nodiscard]] const std::string & getName() const;

		static void registerType( ScriptSystem * pScriptSystem );
	};

	inline int RefData::getCnt()
	{
		return _cnt;
	}

	inline size_t RefData::getSize() const
	{
		return this->name.length();
	}

	inline const std::string & RefData::getName() const
	{
		return name;
	}


	ic3DeclareScriptTypeExt( RefData, "Ic3.NxMain" );

}


#endif /* __IC3_SCRIPT_REF_DATA_H__ */
