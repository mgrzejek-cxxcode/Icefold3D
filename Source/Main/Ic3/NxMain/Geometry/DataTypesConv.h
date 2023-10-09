
#pragma once

#ifndef __IC3_NXMAIN_DATA_TYPES_CONV_H__
#define __IC3_NXMAIN_DATA_TYPES_CONV_H__

#include "GeometryCommonDefs.h"
#include <Ic3/Math/Vector.h>

namespace Ic3
{

	using DataTypeConversionFunction = std::function<void( const void *, void *, uint32 )>;

	template <typename TInput, typename TOutput>
	struct DataTypeConverter
	{
		static void convertValue( const TInput * pInputElements, TOutput * pOutputElements )
		{
			*pOutputElements = numeric_cast<TOutput>( *pInputElements );
		}

		static void convertVector( const TInput * pInputElements, TOutput * pOutputElements, uint32 pElementsNum )
		{
			for( uint32 iElement = 0; iElement < pElementsNum; ++iElement )
			{
				pOutputElements[iElement] = numeric_cast<TOutput>( pInputElements[iElement] );
			}
		}
	};

	template <typename TInput>
	struct GeometryDataConverter
	{
		template <typename TOutput>
		static DataTypeConversionFunction getConversionFunction()
		{
			return []( const void * pInputElements, void * pOutputElements, uint32 ) {
				DataTypeConverter<TInput, TOutput>::convertValue(
						reinterpret_cast<const TInput *>( pInputElements ),
						reinterpret_cast<TOutput *>( pOutputElements ) );
			};
		}
	};

	template <typename TValue, size_t tVectorSize>
	struct GeometryDataConverter<Math::Vector<TValue, tVectorSize>>
	{
		template <typename TOutput>
		static DataTypeConversionFunction getConversionFunction()
		{
			return []( const void * pInputElements, void * pOutputElements, uint32 pElementsNum ) {
				DataTypeConverter<TValue, TOutput>::convertVector(
						reinterpret_cast<const TValue *>( pInputElements ),
						reinterpret_cast<TOutput *>( pOutputElements ),
						getMinOf( tVectorSize, pElementsNum ) );
			};
		}
	};


	namespace gmutil
	{

		template <typename TInput>
		inline DataTypeConversionFunction getGeometryConversionFunction( GCI::EBaseDataType pOutputElementType )
		{
			switch( pOutputElementType )
			{
				case GCI::EBaseDataType::Byte:
				{
					return GeometryDataConverter<TInput>::template getConversionFunction<int8>();
				}
				case GCI::EBaseDataType::Ubyte:
				{
					return GeometryDataConverter<TInput>::template getConversionFunction<uint8>();
				}
				case GCI::EBaseDataType::Int16:
				{
					return GeometryDataConverter<TInput>::template getConversionFunction<int16>();
				}
				case GCI::EBaseDataType::Uint16:
				{
					return GeometryDataConverter<TInput>::template getConversionFunction<uint16>();
				}
				case GCI::EBaseDataType::Int32:
				{
					return GeometryDataConverter<TInput>::template getConversionFunction<int32>();
				}
				case GCI::EBaseDataType::Uint32:
				{
					return GeometryDataConverter<TInput>::template getConversionFunction<uint32>();
				}
				case GCI::EBaseDataType::Float16:
				{
					return GeometryDataConverter<TInput>::template getConversionFunction<uint16>();
				}
				case GCI::EBaseDataType::Float32:
				{
					return GeometryDataConverter<TInput>::template getConversionFunction<float>();
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
