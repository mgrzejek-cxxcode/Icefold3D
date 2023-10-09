
#pragma once

#ifndef __IC3_CORELIB_XML_COMMON_H__
#define __IC3_CORELIB_XML_COMMON_H__

#include "../Prerequisites.h"
#include <Ic3/Cppx/StringView.h>
#include <Ic3/Cppx/Utilities.h>
#include <XML/rapidxml.hpp>

namespace Ic3
{

	class XMLAttribute;
	class XMLNode;
	class XMLTree;

	using RxmlDocument = rapidxml::xml_document<char>;
	using RxmlNode = rapidxml::xml_node<char>;
	using RxmlAttribute = rapidxml::xml_attribute<char>;

	struct RxmlData
	{
		RxmlData() = default;
		RxmlData( RxmlData && ) = default;
		RxmlData & operator=( RxmlData && ) = default;

		RxmlData( std::nullptr_t )
		: rootNode( nullptr )
		{}

		explicit operator bool() const
		{
			return document && rootNode && !textContent.empty();
		}

		std::unique_ptr<RxmlDocument> document;
		RxmlNode * rootNode;
		std::string textContent;
	};

} // namespace Ic3

#endif // __IC3_CORELIB_XML_COMMON_H__
