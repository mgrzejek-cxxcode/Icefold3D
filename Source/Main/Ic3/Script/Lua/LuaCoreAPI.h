
#pragma once

#ifndef __IC3_SCRIPT_LUA_CORE_API_H__
#define __IC3_SCRIPT_LUA_CORE_API_H__

#include "LuaPrerequisites.h"

namespace Ic3::Script
{

	namespace LuaCore
	{

		/// @brief Opens standard LUA library, allowing for automatic removal from the stack (default).
		IC3_SCRIPT_API void openLibrary( lua_State * pLuaState, LuaCallback pLoadCallback );

		/// @brief
		IC3_SCRIPT_API bool checkResult( lua_State * pLuaState, int pResult );

		/// @brief
		IC3_SCRIPT_API void printError( lua_State * pLuaState, int pResult );

		/// @brief Queries metatable with the given name. Returns true if it has been found or false otherwise.
		IC3_SCRIPT_API bool queryMetatable( lua_State * pLuaState, const char * pMetatableName );

		/// @brief Raises type error caused by value at index <c>index</c>.
		IC3_SCRIPT_API void typeError( lua_State * pLuaState, int pIndex, const char * pExpectedStr );

		/// @brief Allocates new userdata of specified size and sets metatable of given name.
		IC3_SCRIPT_API void * newObject( lua_State * pLuaState, size_t pSize, const char* pMetatableName );

		/// @brief Prints on the screen all values currently residing on the stack.
		IC3_SCRIPT_API void dumpStack( lua_State * pLuaState );

		/// @brief Removes specified number of values from the top of the stack.
		IC3_SCRIPT_API void popStack( lua_State * pLuaState, int pCount );

		/// @brief Retrieves value from the top of the stack as boolean. Performs basic type checking. Does not remove value from the stack.
		IC3_SCRIPT_API bool getBoolean( lua_State * pLuaState, int pIndex );

		/// @brief Retrieves value from the top of the stack as integral number. Performs basic type checking. Does not remove value from the stack.
		IC3_SCRIPT_API lua_Integer getInteger( lua_State * pLuaState, int pIndex );

		/// @brief Retrieves value from the top of the stack as floating point number. Performs basic type checking. Does not remove value from the stack.
		IC3_SCRIPT_API lua_Number getFloatingPoint( lua_State * pLuaState, int pIndex );

		/// @brief Retrieves value from the top of the stack as raw pointer. Performs basic type checking. Does not remove value from the stack.
		IC3_SCRIPT_API void * getPointer( lua_State * pLuaState, int pIndex );

		/// @brief Retrieves value from the top of the stack as string literal. Performs basic type checking. Does not remove value from the stack.
		IC3_SCRIPT_API const char * getString( lua_State * pLuaState, int pIndex );

		/// @brief Retrieves value from the top of the stack as user data. Performs basic type checking. Does not remove value from the stack.
		IC3_SCRIPT_API void * getUserData( lua_State * pLuaState, int pIndex, const char * pMetatableName );

		/// @brief pushes boolean value to the top of the stack.
		IC3_SCRIPT_API void pushBoolean( lua_State * pLuaState, bool pBooleanValue );

		/// @brief pushes integral number to the top of the stack.
		IC3_SCRIPT_API void pushInteger( lua_State * pLuaState, lua_Integer pIntegerValue );

		/// @brief pushes floating point number to the top of the stack.
		IC3_SCRIPT_API void pushFloatingPoint( lua_State * pLuaState, lua_Number pNumberValue );

		/// @brief pushes raw pointer to the top of the stack.
		IC3_SCRIPT_API void pushPointer( lua_State * pLuaState, void * pPointer );

		/// @brief pushes string literal to the top of the stack.
		IC3_SCRIPT_API void pushString( lua_State * pLuaState, const char * pString, size_t pLength = CxDef::INVALID_LENGTH );

		/// @brief pushes user data to the top of the stack.
		IC3_SCRIPT_API bool pushUserData( lua_State * pLuaState, void * pUserData, const char * pMetatableName );


		/// @brief QQQ
		template <class T>
		inline typename LuaDataType<T>::Type getValue( lua_State * pLuaState, int pIndex )
		{
			typedef typename LuaDataType<T>::Type ResultType;

			int argumentIndex = static_cast<int>( pIndex );
			void * argument_ptr = getUserData( pLuaState, argumentIndex, getTypeMetatableName<T>() );

			return reinterpret_cast<ResultType>(argument_ptr);
		}

		template <>
		inline typename LuaDataType<void*>::Type getValue<void*>( lua_State * pLuaState, int pIndex )
		{
			auto result = getPointer( pLuaState, static_cast<int>( pIndex ) );
			return static_cast<void*>( result );
		}

		template <>
		inline typename LuaDataType<const char*>::Type getValue<const char*>( lua_State * pLuaState, int pIndex )
		{
			auto result = getString( pLuaState, static_cast<int>( pIndex ) );
			return static_cast<const char*>( result );
		}

		template <>
		inline typename LuaDataType<bool>::Type getValue<bool>( lua_State * pLuaState, int pIndex )
		{
			auto result = getBoolean( pLuaState, static_cast<int>( pIndex ) );
			return static_cast<bool>( result );
		}

		template <>
		inline typename LuaDataType<int32>::Type getValue<int32>( lua_State * pLuaState, int pIndex )
		{
			auto result = getInteger( pLuaState, static_cast<int>( pIndex ) );
			return static_cast<int32>( result );
		}

		template <>
		inline typename LuaDataType<uint32>::Type getValue<uint32>( lua_State * pLuaState, int pIndex )
		{
			auto result = getInteger( pLuaState, static_cast<int>( pIndex ) );
			return static_cast<uint32>( result );
		}

		template <>
		inline typename LuaDataType<int64>::Type getValue<int64>( lua_State * pLuaState, int pIndex )
		{
			auto result = getInteger( pLuaState, static_cast<int>( pIndex ) );
			return static_cast<int64>( result );
		}

		template <>
		inline typename LuaDataType<uint64>::Type getValue<uint64>( lua_State * pLuaState, int pIndex )
		{
			auto result = getInteger( pLuaState, static_cast<int>( pIndex ) );
			return static_cast<uint64>( result );
		}

		template <>
		inline typename LuaDataType<float>::Type getValue<float>( lua_State * pLuaState, int pIndex )
		{
			auto result = getFloatingPoint( pLuaState, static_cast<int>( pIndex ) );
			return static_cast<float>( result );
		}

		template <>
		inline typename LuaDataType<double>::Type getValue<double>( lua_State * pLuaState, int pIndex )
		{
			auto result = getFloatingPoint( pLuaState, static_cast<int>( pIndex ) );
			return static_cast<double>( result );
		}

		template <class T>
		inline T * getThisPointer( lua_State * pLuaState, size_t pArgsCount )
		{
			int thisPtrIndex = -static_cast<int>( pArgsCount ) - 1;
			return getValue<T>( pLuaState, thisPtrIndex );
		}

		template <class T>
		inline auto getFunctionArgument( lua_State * pLuaState, size_t pTotalArgsCount, int pArgNum ) -> decltype( getValue<T>( pLuaState, 0 ) )
		{
			const auto argumentIndex = -static_cast<int>( pTotalArgsCount ) + pArgNum;
			return getValue<T>( pLuaState, argumentIndex );
		}

		///<summary>
		///</summary>
		template <class T>
		inline auto popValue( lua_State * pLuaState, int pIndex ) -> decltype( getValue<T>( pLuaState, pIndex ) )
		{
			auto result = getValue<T>( pLuaState, pIndex );
			popStack( pLuaState, 1 );
			return result;
		}

		inline void pushValue( lua_State * pLuaState, const std::string & pValue )
		{
			pushString( pLuaState, pValue.data(), pValue.length() );
		}

		///<summary>
		///</summary>
		template <class R>
		inline void pushValue( lua_State * pLuaState, typename LuaDataType<R>::Type pValue )
		{
			void * userDataPtr = reinterpret_cast< void* >( pValue );
			pushUserData( pLuaState, userDataPtr, getTypeMetatableName<R>() );
		}

		template <>
		inline void pushValue<std::string>( lua_State * pLuaState, LuaDataType<std::string>::Type pValue )
		{
			pushString( pLuaState, pValue.data(), pValue.length() );
		}

		template <>
		inline void pushValue<void*>( lua_State * pLuaState, LuaDataType<void*>::Type pValue )
		{
			pushPointer( pLuaState, pValue );
		}

		template <>
		inline void pushValue<const char*>( lua_State * pLuaState, LuaDataType<const char*>::Type pValue )
		{
			pushString( pLuaState, pValue );
		}

		template <>
		inline void pushValue<bool>( lua_State * pLuaState, LuaDataType<bool>::Type pValue )
		{
			pushBoolean( pLuaState, pValue );
		}

		template <>
		inline void pushValue<short>( lua_State * pLuaState, LuaDataType<short>::Type pValue )
		{
			pushInteger( pLuaState, pValue );
		}

		template <>
		inline void pushValue<unsigned short>( lua_State * pLuaState, LuaDataType<unsigned short>::Type pValue )
		{
			pushInteger( pLuaState, pValue );
		}

		template <>
		inline void pushValue<int>( lua_State * pLuaState, LuaDataType<int>::Type pValue )
		{
			pushInteger( pLuaState, pValue );
		}

		template <>
		inline void pushValue<unsigned int>( lua_State * pLuaState, LuaDataType<unsigned int>::Type pValue )
		{
			pushInteger( pLuaState, pValue );
		}

		template <>
		inline void pushValue<long>( lua_State * pLuaState, LuaDataType<long>::Type pValue )
		{
			pushInteger( pLuaState, pValue );
		}

		template <>
		inline void pushValue<unsigned long>( lua_State * pLuaState, LuaDataType<unsigned long>::Type pValue )
		{
			pushInteger( pLuaState, static_cast<lua_Integer>( pValue ) );
		}

		template <>
		inline void pushValue<long long>( lua_State * pLuaState, LuaDataType<long long>::Type pValue )
		{
			pushInteger( pLuaState, pValue );
		}

		template <>
		inline void pushValue<unsigned long long>( lua_State * pLuaState, LuaDataType<unsigned long long>::Type pValue )
		{
			pushInteger( pLuaState, static_cast<lua_Integer>( pValue ) );
		}

		template <>
		inline void pushValue<float>( lua_State * pLuaState, LuaDataType<float>::Type pValue )
		{
			pushFloatingPoint( pLuaState, pValue );
		}

		template <>
		inline void pushValue<double>( lua_State * pLuaState, LuaDataType<double>::Type pValue )
		{
			pushFloatingPoint( pLuaState, pValue );
		}

		template <>
		inline void pushValue<long double>( lua_State * pLuaState, LuaDataType<long double>::Type pValue )
		{
			pushFloatingPoint( pLuaState, static_cast<lua_Number>( pValue ) );
		}

	} // namespace LuaCore

} // namespace Ic3::Script

#endif /* __IC3_SCRIPT_LUA_CORE_API_H__ */
