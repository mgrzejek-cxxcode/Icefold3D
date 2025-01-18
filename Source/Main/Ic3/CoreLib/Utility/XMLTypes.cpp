
#include "XMLTypes.h"
#include "RXMLParser.h"
#include <cppx/stdHelperAlgo.h>

namespace Ic3
{

	XMLAttribute::XMLAttribute( std::nullptr_t )
	: _rxmlAttribute( nullptr )
	{}

	XMLAttribute::XMLAttribute( RxmlAttribute * pRxmlAttribute )
	: _rxmlAttribute( pRxmlAttribute )
	{}

	XMLAttribute::operator bool() const
	{
		return IsValid();
	}

	XMLNode XMLAttribute::parent() const
	{
		Ic3DebugAssert( IsValid() );
		return XMLNode{ _rxmlAttribute->parent() };
	}

	std::string XMLAttribute::name() const
	{
		Ic3DebugAssert( IsValid() );
		return RxmlParser::getAttributeName( _rxmlAttribute );
	}

	std::string XMLAttribute::value() const
	{
		Ic3DebugAssert( IsValid() );
		return RxmlParser::getAttributeValue( _rxmlAttribute );
	}

	bool XMLAttribute::IsEmpty() const
	{
		return !_rxmlAttribute || ( _rxmlAttribute->value_size() == 0 );
	}

	bool XMLAttribute::IsValid() const
	{
		return _rxmlAttribute != nullptr;
	}


	XMLNode::XMLNode( std::nullptr_t )
	: _rxmlNode( nullptr )
	{}

	XMLNode::XMLNode( RxmlNode * pRxmlNode )
	: _rxmlNode( pRxmlNode )
	{}

	XMLNode::operator bool() const
	{
		return IsValid();
	}

	XMLAttribute XMLNode::attribute( const cppx::string_view & pAttribName ) const
	{
		Ic3DebugAssert( IsValid() );
		return XMLAttribute{ _rxmlNode->first_attribute( pAttribName.str(), pAttribName.length() ) };
	}

	XMLNode XMLNode::firstSubNode() const
	{
		Ic3DebugAssert( IsValid() );
		return XMLNode{ _rxmlNode->first_node() };
	}

	XMLNode XMLNode::firstSubNode( const cppx::string_view & pNodeName ) const
	{
		Ic3DebugAssert( IsValid() );
		return XMLNode{ _rxmlNode->first_node( pNodeName.str(), pNodeName.length() ) };
	}

	size_t XMLNode::countSubNodes() const
	{
		Ic3DebugAssert( IsValid() );
		return RxmlParser::countSubNodes( _rxmlNode );
	}

	size_t XMLNode::countSubNodes( const cppx::string_view & pNodeName ) const
	{
		Ic3DebugAssert( IsValid() );
		return RxmlParser::countSubNodes( _rxmlNode, pNodeName.str(), pNodeName.length() );
	}

	XMLNode XMLNode::nextSibling() const
	{
		Ic3DebugAssert( IsValid() );
		return XMLNode{ _rxmlNode->next_sibling() };
	}

	XMLNode XMLNode::nextSibling( const cppx::string_view & pNodeName ) const
	{
		Ic3DebugAssert( IsValid() );
		return XMLNode{ _rxmlNode->next_sibling( pNodeName.str(), pNodeName.length() ) };
	}

	XMLNode XMLNode::parent() const
	{
		Ic3DebugAssert( IsValid() );
		return XMLNode{ _rxmlNode->parent() };
	}

	std::string XMLNode::name() const
	{
		Ic3DebugAssert( IsValid() );
		return RxmlParser::getNodeName( _rxmlNode );
	}

	std::string XMLNode::value() const
	{
		Ic3DebugAssert( IsValid() );
		return RxmlParser::getNodeValue( _rxmlNode );
	}

	bool XMLNode::hasAttribute( const cppx::string_view & pAttribName ) const
	{
		Ic3DebugAssert( IsValid() );
		return _rxmlNode->first_attribute( pAttribName.str(), pAttribName.length() );
	}

	bool XMLNode::hasSubNode( const cppx::string_view & pNodeName ) const
	{
		Ic3DebugAssert( IsValid() );
		return _rxmlNode->first_node( pNodeName.str(), pNodeName.length() );
	}

	bool XMLNode::hasSubNodes() const
	{
		Ic3DebugAssert( IsValid() );
		return _rxmlNode->first_attribute() != nullptr;
	}

	bool XMLNode::hasValue() const
	{
		Ic3DebugAssert( IsValid() );
		return RxmlParser::checkNodeHasValue( _rxmlNode );
	}

	bool XMLNode::IsEmpty() const
	{
		Ic3DebugAssert( IsValid() );
		return !hasValue() && !hasSubNodes();
	}

	bool XMLNode::IsValid() const
	{
		return _rxmlNode != nullptr;
	}



	XMLTree::XMLTree( std::nullptr_t )
	: _rxmlData( nullptr )
	{}

	XMLTree::XMLTree( RxmlData pRxmlData )
	: _rxmlData( std::move( pRxmlData ) )
	{}

	XMLTree::operator bool() const
	{
		return IsValid();
	}

	XMLNode XMLTree::rootNode() const
	{
		Ic3DebugAssert( IsValid() );
		return XMLNode{ _rxmlData.rootNode };
	}

	bool XMLTree::IsEmpty() const
	{
		Ic3DebugAssert( IsValid() );
		return !_rxmlData.rootNode->first_node();
	}

	bool XMLTree::IsValid() const
	{
		return _rxmlData && _rxmlData.rootNode;
	}


	XMLTree XMLReader::readXMLContent( std::string pXMLContent )
	{
		auto rxmlData = RxmlParser::parseXMLContent( std::move( pXMLContent ) );
		return XMLTree{ std::move( rxmlData ) };
	}

}
