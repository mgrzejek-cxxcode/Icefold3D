
#pragma once

#ifndef __IC3_CORELIB_XML_TYPES_H__
#define __IC3_CORELIB_XML_TYPES_H__

#include "XMLCommon.h"

namespace Ic3
{

	/// @brief
	class XMLAttribute
	{
	public:
		XMLAttribute( const XMLAttribute & ) = default;
		XMLAttribute & operator=( const XMLAttribute & ) = default;

		XMLAttribute( std::nullptr_t = nullptr );
		explicit XMLAttribute( RxmlAttribute * pRxmlAttribute );

		explicit operator bool() const;

		XMLNode parent() const;

		std::string name() const;

		std::string value() const;

		bool IsEmpty() const;

		bool IsValid() const;

	private:
		RxmlAttribute * _rxmlAttribute;
	};

	/// @brief
	class XMLNode
	{
	public:
		XMLNode( const XMLNode & ) = default;
		XMLNode & operator=( const XMLNode & ) = default;

		XMLNode( std::nullptr_t = nullptr );
		explicit XMLNode( RxmlNode * pRxmlNode );

		explicit operator bool() const;

		XMLAttribute attribute( const cppx::string_view & pAttribName ) const;

		XMLNode firstSubNode() const;

		XMLNode firstSubNode( const cppx::string_view & pNodeName ) const;

		size_t countSubNodes() const;

		size_t countSubNodes( const cppx::string_view & pNodeName ) const;

		XMLNode nextSibling() const;

		XMLNode nextSibling( const cppx::string_view & pNodeName ) const;

		XMLNode parent() const;

		std::string name() const;

		std::string value() const;

		bool hasAttribute( const cppx::string_view & pAttribName ) const;

		bool hasSubNode( const cppx::string_view & pNodeName ) const;

		bool hasSubNodes() const;

		bool hasValue() const;

		bool IsEmpty() const;

		bool IsValid() const;

	private:
		RxmlNode * _rxmlNode;
	};

	/// @brief
	class XMLTree
	{
	public:
		XMLTree( XMLTree && ) = default;
		XMLTree & operator=( XMLTree && ) = default;

		XMLTree( std::nullptr_t = nullptr );
		explicit XMLTree( RxmlData pRxmlData );

		explicit operator bool() const;

		XMLNode rootNode() const;

		bool IsEmpty() const;

		bool IsValid() const;

	private:
		RxmlData _rxmlData;
	};

	/// @brief
	class XMLReader
	{
	public:
		static XMLTree readXMLContent( std::string pXMLContent );
	};
	
}

#endif // __IC3_CORELIB_XML_TYPES_H__
