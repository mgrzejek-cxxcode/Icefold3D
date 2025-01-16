
#ifndef __CPPX_PATH_NAME_ITERATOR_H__
#define __CPPX_PATH_NAME_ITERATOR_H__

#include "Prerequisites.h"
#include <string>

namespace cppx
{

	class path_name_iterator
	{
	public:
		path_name_iterator() = default;

		path_name_iterator( path_name_iterator && ) = default;
		path_name_iterator & operator=( path_name_iterator && ) = default;

		path_name_iterator( const path_name_iterator & ) = default;
		path_name_iterator & operator=( const path_name_iterator & ) = default;

		explicit path_name_iterator( std::string pPath )
		: _currentPathComponent{}
		, _remainingPath( std::move( pPath ) )
		{
			_move_fwd();
		}

		explicit operator bool() const
		{
			return !_currentPathComponent.empty();
		}

		path_name_iterator & operator++()
		{
			_move_fwd();
			return *this;
		}

		path_name_iterator operator++( int )
		{
			path_name_iterator copy{ *this };
			_move_fwd();
			return copy;
		}

		CPPX_ATTR_NO_DISCARD const std::string & name() const
		{
			return _currentPathComponent;
		}

		CPPX_ATTR_NO_DISCARD bool empty() const
		{
			return _currentPathComponent.empty();
		}

		CPPX_ATTR_NO_DISCARD bool has_next() const
		{
			return !_remainingPath.empty();
		}

	private:
		void _move_fwd()
		{
			std::string nextPathComponent;
			
			if( !_remainingPath.empty() )
			{
				auto startPos = _remainingPath.find_first_not_of( "/\\" );
				auto endPos = _remainingPath.find_first_of( "/\\", startPos );

				if( ( startPos != std::string::npos ) && ( endPos != startPos ) )
				{
					nextPathComponent = _remainingPath.substr( startPos, endPos - startPos );
				}

				if( nextPathComponent.empty() )
				{
					_remainingPath.clear();
				}
				else
				{
					endPos = _remainingPath.find_first_not_of( "/\\", endPos );
					_remainingPath.erase( 0, endPos );
				}
			}

			_currentPathComponent = std::move( nextPathComponent );
		}

	private:
		std::string _currentPathComponent;
		std::string _remainingPath;
	};

}

#endif // __CPPX_PATH_NAME_ITERATOR_H__
