
#pragma once

#ifndef __IC3_NXMAIN_DATA_TYPES_CONV_H__
#define __IC3_NXMAIN_DATA_TYPES_CONV_H__

#include "GeometryCommonDefs.h"
#include <Ic3/Math/Vector.h>

namespace Ic3
{

	using DataTypeConversionFunction = std::function<void( const void *, void *, uint32 )>;

	template <typename TPInput, typename TPOutput>
	struct DataTypeConverter
	{
		static void ConvertValue( const TPInput * pInputElements, TPOutput * pOutputElements )
		{
			*pOutputElements = cppx::numeric_cast<TPOutput>( *pInputElements );
		}

		static void ConvertVector( const TPInput * pInputElements, TPOutput * pOutputElements, uint32 pElementsNum )
		{
			for( uint32 iElement = 0; iElement < pElementsNum; ++iElement )
			{
				pOutputElements[iElement] = cppx::numeric_cast<TPOutput>( pInputElements[iElement] );
			}
		}
	};

	template <typename TPInput>
	struct GeometryDataConverter
	{
		template <typename TPOutput>
		static DataTypeConversionFunction GetConversionFunction()
		{
			return []( const void * pInputElements, void * pOutputElements, uint32 ) {
				DataTypeConverter<TPInput, TPOutput>::ConvertValue(
						reinterpret_cast<const TPInput *>( pInputElements ),
						reinterpret_cast<TPOutput *>( pOutputElements ) );
			};
		}
	};

	template <typename TPValue, size_t tVectorSize>
	struct GeometryDataConverter<Math::Vector<TPValue, tVectorSize>>
	{
		template <typename TPOutput>
		static DataTypeConversionFunction GetConversionFunction()
		{
			return []( const void * pInputElements, void * pOutputElements, uint32 pElementsNum ) {
				DataTypeConverter<TPValue, TPOutput>::ConvertVector(
						reinterpret_cast<const TPValue *>( pInputElements ),
						reinterpret_cast<TPOutput *>( pOutputElements ),
						get_min_of( tVectorSize, pElementsNum ) );
			};
		}
	};


	namespace gmutil
	{

		template <typename TPInput>
		inline DataTypeConversionFunction GetGeometryConversionFunction( GCI::EBaseDataType pOutputElementType )
		{
			switch( pOutputElementType )
			{
				case GCI::EBaseDataType::Byte:
				{
					return GeometryDataConverter<TPInput>::template GetConversionFunction<int8>();
				}
				case GCI::EBaseDataType::Ubyte:
				{
					return GeometryDataConverter<TPInput>::template GetConversionFunction<uint8>();
				}
				case GCI::EBaseDataType::Int16:
				{
					return GeometryDataConverter<TPInput>::template GetConversionFunction<int16>();
				}
				case GCI::EBaseDataType::Uint16:
				{
					return GeometryDataConverter<TPInput>::template GetConversionFunction<uint16>();
				}
				case GCI::EBaseDataType::Int32:
				{
					return GeometryDataConverter<TPInput>::template GetConversionFunction<int32>();
				}
				case GCI::EBaseDataType::Uint32:
				{
					return GeometryDataConverter<TPInput>::template GetConversionFunction<uint32>();
				}
				case GCI::EBaseDataType::Float16:
				{
					return GeometryDataConverter<TPInput>::template GetConversionFunction<uint16>();
				}
				case GCI::EBaseDataType::Float32:
				{
					return GeometryDataConverter<TPInput>::template GetConversionFunction<float>();
				}
				default:
				{
					break;
				}
			}

			return {};
		}

	}

} // namespace Ic3

#endif // __IC3_NXMAIN_DATA_TYPES_CONV_H__
