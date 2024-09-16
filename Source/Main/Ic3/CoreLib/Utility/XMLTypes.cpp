
#include "XMLTypes.h"
#include "RXMLParser.h"
#include <Ic3/Cppx/STLHelperAlgo.h>

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
		return valid();
	}

	XMLNode XMLAttribute::parent() const
	{
		ic3DebugAssert( valid() );
		return XMLNode{ _rxmlAttribute->parent() };
	}

	std::string XMLAttribute::name() const
	{
		ic3DebugAssert( valid() );
		return RxmlParser::getAttributeName( _rxmlAttribute );
	}

	std::string XMLAttribute::value() const
	{
		ic3DebugAssert( valid() );
		return RxmlParser::getAttributeValue( _rxmlAttribute );
	}

	bool XMLAttribute::empty() const
	{
		return !_rxmlAttribute || ( _rxmlAttribute->value_size() == 0 );
	}

	bool XMLAttribute::valid() const
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
		return valid();
	}

	XMLAttribute XMLNode::attribute( const TStringView<char> & pAttribName ) const
	{
		ic3DebugAssert( valid() );
		return XMLAttribute{ _rxmlNode->first_attribute( pAttribName.str(), pAttribName.length() ) };
	}

	XMLNode XMLNode::firstSubNode() const
	{
		ic3DebugAssert( valid() );
		return XMLNode{ _rxmlNode->first_node() };
	}

	XMLNode XMLNode::firstSubNode( const TStringView<char> & pNodeName ) const
	{
		ic3DebugAssert( valid() );
		return XMLNode{ _rxmlNode->first_node( pNodeName.str(), pNodeName.length() ) };
	}

	size_t XMLNode::countSubNodes() const
	{
		ic3DebugAssert( valid() );
		return RxmlParser::countSubNodes( _rxmlNode );
	}

	size_t XMLNode::countSubNodes( const TStringView<char> & pNodeName ) const
	{
		ic3DebugAssert( valid() );
		return RxmlParser::countSubNodes( _rxmlNode, pNodeName.str(), pNodeName.length() );
	}

	XMLNode XMLNode::nextSibling() const
	{
		ic3DebugAssert( valid() );
		return XMLNode{ _rxmlNode->next_sibling() };
	}

	XMLNode XMLNode::nextSibling( const TStringView<char> & pNodeName ) const
	{
		ic3DebugAssert( valid() );
		return XMLNode{ _rxmlNode->next_sibling( pNodeName.str(), pNodeName.length() ) };
	}

	XMLNode XMLNode::parent() const
	{
		ic3DebugAssert( valid() );
		return XMLNode{ _rxmlNode->parent() };
	}

	std::string XMLNode::name() const
	{
		ic3DebugAssert( valid() );
		return RxmlParser::getNodeName( _rxmlNode );
	}

	std::string XMLNode::value() const
	{
		ic3DebugAssert( valid() );
		return RxmlParser::getNodeValue( _rxmlNode );
	}

	bool XMLNode::hasAttribute( const TStringView<char> & pAttribName ) const
	{
		ic3DebugAssert( valid() );
		return _rxmlNode->first_attribute( pAttribName.str(), pAttribName.length() );
	}

	bool XMLNode::hasSubNode( const TStringView<char> & pNodeName ) const
	{
		ic3DebugAssert( valid() );
		return _rxmlNode->first_node( pNodeName.str(), pNodeName.length() );
	}

	bool XMLNode::hasSubNodes() const
	{
		ic3DebugAssert( valid() );
		return _rxmlNode->first_attribute() != nullptr;
	}

	bool XMLNode::hasValue() const
	{
		ic3DebugAssert( valid() );
		return RxmlParser::checkNodeHasValue( _rxmlNode );
	}

	bool XMLNode::empty() const
	{
		ic3DebugAssert( valid() );
		return !hasValue() && !hasSubNodes();
	}

	bool XMLNode::valid() const
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
		return valid();
	}

	XMLNode XMLTree::rootNode() const
	{
		ic3DebugAssert( valid() );
		return XMLNode{ _rxmlData.rootNode };
	}

	bool XMLTree::empty() const
	{
		ic3DebugAssert( valid() );
		return !_rxmlData.rootNode->first_node();
	}

	bool XMLTree::valid() const
	{
		return _rxmlData && _rxmlData.rootNode;
	}


	XMLTree XMLReader::readXMLContent( std::string pXMLContent )
	{
		auto rxmlData = RxmlParser::parseXMLContent( std::move( pXMLContent ) );
		return XMLTree{ std::move( rxmlData ) };
	}

}
