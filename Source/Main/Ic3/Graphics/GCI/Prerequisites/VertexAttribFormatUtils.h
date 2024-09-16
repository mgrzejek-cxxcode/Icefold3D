
#pragma once

#ifndef __IC3_GRAPHICS_GCI_VERTEX_FORMAT_ATTRIB_UTILS_H__
#define __IC3_GRAPHICS_GCI_VERTEX_FORMAT_ATTRIB_UTILS_H__

namespace Ic3::Graphics::GCI
{

	template <typename TPScalar>
	struct QBaseScalarDataTypeTraits;

	template <>
	struct QBaseScalarDataTypeTraits<int8>
	{
		static constexpr auto sBaseDataType = EBaseDataType::Byte;
		static constexpr auto sBaseDataFlags = eGPUDataFormatFlagTypeSignedBit;
		static constexpr auto sByteSize = 1;
	};

	template <>
	struct QBaseScalarDataTypeTraits<uint8>
	{
		static constexpr auto sBaseDataType = EBaseDataType::Ubyte;
		static constexpr auto sBaseDataFlags = eGPUDataFormatFlagTypeUnsignedBit;
		static constexpr auto sByteSize = 1;
	};

	template <>
	struct QBaseScalarDataTypeTraits<int16>
	{
		static constexpr auto sBaseDataType = EBaseDataType::Int16;
		static constexpr auto sBaseDataFlags = eGPUDataFormatFlagTypeSignedBit;
		static constexpr auto sByteSize = 2;
	};

	template <>
	struct QBaseScalarDataTypeTraits<uint16>
	{
		static constexpr auto sBaseDataType = EBaseDataType::Uint16;
		static constexpr auto sBaseDataFlags = eGPUDataFormatFlagTypeUnsignedBit;
		static constexpr auto sByteSize = 2;
	};

	template <>
	struct QBaseScalarDataTypeTraits<int32>
	{
		static constexpr auto sBaseDataType = EBaseDataType::Int32;
		static constexpr auto sBaseDataFlags = eGPUDataFormatFlagTypeSignedBit;
		static constexpr auto sByteSize = 4;
	};

	template <>
	struct QBaseScalarDataTypeTraits<uint32>
	{
		static constexpr auto sBaseDataType = EBaseDataType::Uint32;
		static constexpr auto sBaseDataFlags = eGPUDataFormatFlagTypeUnsignedBit;
		static constexpr auto sByteSize = 4;
	};

	template <>
	struct QBaseScalarDataTypeTraits<float>
	{
		static constexpr auto sBaseDataType = EBaseDataType::Float32;
		static constexpr auto sBaseDataFlags = eGPUDataFormatFlagTypeSignedBit;
		static constexpr auto sByteSize = 4;
	};

	template <typename TMathType>
	struct QVertexAttribFormatDataTypeTraits;

	template <size_t tpSize>
	struct QVertexAttribFormatDataTypeTraits<Math::Vector<int64, tpSize>>;

	template <size_t tpSize>
	struct QVertexAttribFormatDataTypeTraits<Math::Vector<uint64, tpSize>>;

	template <size_t tpSize>
	struct QVertexAttribFormatDataTypeTraits<Math::Vector<double, tpSize>>;

	template <size_t tpRows, size_t tpColumns>
	struct QVertexAttribFormatDataTypeTraits<Math::Matrix<int64, tpRows, tpColumns>>;

	template <size_t tpRows, size_t tpColumns>
	struct QVertexAttribFormatDataTypeTraits<Math::Matrix<uint64, tpRows, tpColumns>>;

	template <size_t tpRows, size_t tpColumns>
	struct QVertexAttribFormatDataTypeTraits<Math::Matrix<double, tpRows, tpColumns>>;

	template <typename TPScalar, size_t tpSize>
	struct QVertexAttribFormatDataTypeTraits<Math::Vector<TPScalar, tpSize>>
	{
		using BaseDataType = TPScalar;

		static constexpr auto sAttribComponentsNum = 1;
		static constexpr auto sScalarValuesNum = tpSize;
		static constexpr auto sComponentSizeInBytes = sizeof( BaseDataType ) * tpSize;
		static constexpr auto sSizeInBytes = sComponentSizeInBytes;

		static constexpr auto sBaseAttribFormat = ( EVertexAttribFormat )CxDef::declareVertexAttribFormat(
				QBaseScalarDataTypeTraits<TPScalar>::sBaseDataType,
				tpSize,
				QBaseScalarDataTypeTraits<TPScalar>::sBaseDataFlags );
	};

	template <typename TPScalar, size_t tpRows, size_t tpColumns>
	struct QVertexAttribFormatDataTypeTraits<Math::Matrix<TPScalar, tpRows, tpColumns>>
	{
		using BaseDataType = TPScalar;

		static constexpr auto sAttribComponentsNum = tpRows;
		static constexpr auto sScalarValuesNum = tpRows * tpColumns;
		static constexpr auto sComponentSizeInBytes = sizeof( BaseDataType ) * tpColumns;
		static constexpr auto sSizeInBytes = sComponentSizeInBytes * sAttribComponentsNum;

		static constexpr auto sBaseAttribFormat = ( EVertexAttribFormat )CxDef::declareVertexAttribFormat(
				QBaseScalarDataTypeTraits<TPScalar>::sBaseDataType,
				tpColumns,
				QBaseScalarDataTypeTraits<TPScalar>::sBaseDataFlags );
	};

	template <>
	struct QVertexAttribFormatDataTypeTraits<Math::RGBAColor>
	{
		using BaseDataType = uint8;

		static constexpr auto sAttribComponentsNum = 1;
		static constexpr auto sScalarValuesNum = 4;
		static constexpr auto sComponentSizeInBytes = sizeof( BaseDataType ) * 4;
		static constexpr auto sSizeInBytes = sComponentSizeInBytes * sAttribComponentsNum;

		static constexpr auto sBaseAttribFormat = ( EVertexAttribFormat )CxDef::declareVertexAttribFormat(
				QBaseScalarDataTypeTraits<uint8>::sBaseDataType,
				4,
				QBaseScalarDataTypeTraits<uint8>::sBaseDataFlags );
	};

	template <>
	struct QVertexAttribFormatDataTypeTraits<Math::RGBAColorNorm<float>>
	{
		using BaseDataType = float;

		static constexpr auto sAttribComponentsNum = 1;
		static constexpr auto sScalarValuesNum = 4;
		static constexpr auto sComponentSizeInBytes = sizeof( BaseDataType ) * 4;
		static constexpr auto sSizeInBytes = sComponentSizeInBytes * sAttribComponentsNum;

		static constexpr auto sBaseAttribFormat = ( EVertexAttribFormat )CxDef::declareVertexAttribFormat(
				QBaseScalarDataTypeTraits<float>::sBaseDataType,
				4,
				QBaseScalarDataTypeTraits<float>::sBaseDataFlags );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_VERTEX_FORMAT_ATTRIB_UTILS_H__
