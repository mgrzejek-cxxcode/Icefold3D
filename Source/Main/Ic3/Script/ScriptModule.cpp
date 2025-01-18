
#include "ScriptModule.h"

namespace Ic3::Script
{

	SmNamespace & SmAPI::defineNamespace( SmNamespace * pParent, std::string pName )
	{
		auto namespacePtr = std::make_unique<SmNamespace>( pParent, std::move( pName ) );
		auto & namespaceRef = *namespacePtr;
		pParent->addChild( std::move( namespacePtr ) );
		return namespaceRef;
	}

}
