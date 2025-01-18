
#pragma once

#ifndef __IC3_NXMAIN_SCF_XML_COMMON_H__
#define __IC3_NXMAIN_SCF_XML_COMMON_H__

#include "scfCommon.h"
#include <Ic3/CoreLib/utility/xmlTypes.h>

namespace Ic3
{

	class SCFXMLNode;

	using SCFXMLNodeList = std::vector<const SCFXMLNode *>;

	enum class ESCFXMLNodeType : enum_default_value_t
	{
		Unknown,
		Resource,
		Folder
	};

	class SCFXMLNode
	{
	public:

	public:
		SCFXMLNode( std::nullptr_t = nullptr );

		ESCFXMLNodeType nodeType() const;

		const std::string & nodeName() const;

		const std::string & nodeTextValue() const;

		bool empty() const;

	protected:
		XMLNode _xmlNode;
		ESCFXMLNodeType _nodeType;
		std::string _nodeName;
		std::string _nodeTextValue;
	};

	class SCFXMLResourceNode : public SCFXMLNode
	{
	public:
		using DataNodeList = std::vector<XMLNode>;

		SCFXMLResourceNode( std::nullptr_t = nullptr );

		const std::string & resourceID() const;

		const std::string & resourceType() const;

		XMLAttribute attribute( const cppx::string_view<char> & pAttribName ) const;

		XMLNode dataNode( const cppx::string_view<char> & pNodeName ) const;

		bool hasDataNode( const cppx::string_view<char> & pNodeName ) const;

		static SCFXMLResourceNode initFromXMLNode( XMLNode pXMLNode );

		static void initFromXMLNode( XMLNode pXMLNode, SCFXMLResourceNode & pInitNode );

	private:
		std::string _id;
		std::string _type;
	};

	class SCFXMLFolderNode : public SCFXMLNode
	{
	public:
		using ResourceNodeList = std::vector<SCFXMLResourceNode>;
		using SubFolderNodeList = std::vector<SCFXMLFolderNode>;

		SCFXMLFolderNode( std::nullptr_t = nullptr );

		const std::string & folderName() const;

		const SCFXMLResourceNode * resource( const cppx::string_view<char> & pResourceID ) const;

		const SCFXMLFolderNode * subFolder( const cppx::string_view<char> & pSubFolderName ) const;

		SCFXMLNodeList getNodeList() const;

		SCFXMLNodeList & getNodeList( SCFXMLNodeList & pOutputList ) const;

		SCFXMLNodeList getNodeList( bool pRecursive = false ) const;

		SCFXMLNodeList & getNodeList( SCFXMLNodeList & pOutputList, bool pRecursive = false ) const;

		const ResourceNodeList & getResourceNodes() const;

		const SubFolderNodeList & getSubFolderNodes() const;

		bool hasResource( const cppx::string_view<char> & pResourceID ) const;

		bool hasSubFolder( const cppx::string_view<char> & pSubFolderName ) const;

		static SCFXMLFolderNode initFromXMLNode( XMLNode pXMLNode );

		static void initFromXMLNode( XMLNode pXMLNode, SCFXMLFolderNode & pInitNode );

	protected:
		using ResourceNodeMap = std::map<std::string, SCFXMLResourceNode *, Qcppx::string_viewCmpLess<char>>;
		using SubFolderNodeMap = std::map<std::string, SCFXMLFolderNode *, Qcppx::string_viewCmpLess<char>>;

		static void _initContent( const XMLNode & pXMLNode, SCFXMLFolderNode & pInitNode );

		static ResourceNodeList _readResources( const XMLNode & pXMLNode, size_t pResourceNodesNum );

		static ResourceNodeMap _buildResourceNodeMap( ResourceNodeList & pResourceNodes );

		static SubFolderNodeList _readSubFolders( const XMLNode & pXMLNode, size_t pSubFolderNodesNum );

		static SubFolderNodeMap _buildSubFolderNodeMap( SubFolderNodeList & pSubFolderNodes );

	protected:
		std::string _folderName;

	private:
		ResourceNodeList _resourceNodeList;
		ResourceNodeMap _resourceNodeMap;
		SubFolderNodeList _subFolderNodeList;
		SubFolderNodeMap _subFolderNodeMap;
	};

	class SCFXMLRootNode : public SCFXMLFolderNode
	{
	public:
		SCFXMLRootNode( std::nullptr_t = nullptr );

		static SCFXMLRootNode initFromXMLTree( XMLTree pXMLTree );

		static void initFromXMLTree( XMLTree pXMLTree, SCFXMLRootNode & pInitNode );

	private:
		XMLTree _xmlTree;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_SCF_XML_COMMON_H__
