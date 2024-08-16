
#pragma once

#ifndef __IC3_GRAPHICS_GCI_VERTEX_FORMAT_ATTRIB_UTILS_H__
#define __IC3_GRAPHICS_GCI_VERTEX_FORMAT_ATTRIB_UTILS_H__

namespace Ic3::Graphics::GCI
{

	template <typename TScalar>
	struct QBaseScalarDataTypeTraits;

	template <>
	struct QBaseScalarDataTypeTraits<int8>
	{
		static constexpr auto sBaseDataType = EBaseDataType::Byte;
		static constexpr auto sBaseDataFlags = E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT;
		static constexpr auto sByteSize = 1;
	};

	template <>
	struct QBaseScalarDataTypeTraits<uint8>
	{
		static constexpr auto sBaseDataType = EBaseDataType::Ubyte;
		static constexpr auto sBaseDataFlags = E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT;
		static constexpr auto sByteSize = 1;
	};

	template <>
	struct QBaseScalarDataTypeTraits<int16>
	{
		static constexpr auto sBaseDataType = EBaseDataType::Int16;
		static constexpr auto sBaseDataFlags = E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT;
		static constexpr auto sByteSize = 2;
	};

	template <>
	struct QBaseScalarDataTypeTraits<uint16>
	{
		static constexpr auto sBaseDataType = EBaseDataType::Uint16;
		static constexpr auto sBaseDataFlags = E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT;
		static constexpr auto sByteSize = 2;
	};

	template <>
	struct QBaseScalarDataTypeTraits<int32>
	{
		static constexpr auto sBaseDataType = EBaseDataType::Int32;
		static constexpr auto sBaseDataFlags = E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT;
		static constexpr auto sByteSize = 4;
	};

	template <>
	struct QBaseScalarDataTypeTraits<uint32>
	{
		static constexpr auto sBaseDataType = EBaseDataType::Uint32;
		static constexpr auto sBaseDataFlags = E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT;
		static constexpr auto sByteSize = 4;
	};

	template <>
	struct QBaseScalarDataTypeTraits<float>
	{
		static constexpr auto sBaseDataType = EBaseDataType::Float32;
		static constexpr auto sBaseDataFlags = E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT;
		static constexpr auto sByteSize = 4;
	};

	template <typename TMathType>
	struct QVertexAttribFormatDataTypeTraits;

	template <size_t tSize>
	struct QVertexAttribFormatDataTypeTraits<Math::Vector<int64, tSize>>;

	template <size_t tSize>
	struct QVertexAttribFormatDataTypeTraits<Math::Vector<uint64, tSize>>;

	template <size_t tSize>
	struct QVertexAttribFormatDataTypeTraits<Math::Vector<double, tSize>>;

	template <size_t tRows, size_t tColumns>
	struct QVertexAttribFormatDataTypeTraits<Math::Matrix<int64, tRows, tColumns>>;

	template <size_t tRows, size_t tColumns>
	struct QVertexAttribFormatDataTypeTraits<Math::Matrix<uint64, tRows, tColumns>>;

	template <size_t tRows, size_t tColumns>
	struct QVertexAttribFormatDataTypeTraits<Math::Matrix<double, tRows, tColumns>>;

	template <typename TScalar, size_t tSize>
	struct QVertexAttribFormatDataTypeTraits<Math::Vector<TScalar, tSize>>
	{
		using BaseDataType = TScalar;

		static constexpr auto sAttribComponentsNum = 1;
		static constexpr auto sScalarValuesNum = tSize;
		static constexpr auto sComponentSizeInBytes = sizeof( BaseDataType ) * tSize;
		static constexpr auto sSizeInBytes = sComponentSizeInBytes;

		static constexpr auto sBaseAttribFormat = ( EVertexAttribFormat )CxDef::declareVertexAttribFormat(
				QBaseScalarDataTypeTraits<TScalar>::sBaseDataType,
				tSize,
				QBaseScalarDataTypeTraits<TScalar>::sBaseDataFlags );
	};

	template <typename TScalar, size_t tRows, size_t tColumns>
	struct QVertexAttribFormatDataTypeTraits<Math::Matrix<TScalar, tRows, tColumns>>
	{
		using BaseDataType = TScalar;

		static constexpr auto sAttribComponentsNum = tRows;
		static constexpr auto sScalarValuesNum = tRows * tColumns;
		static constexpr auto sComponentSizeInBytes = sizeof( BaseDataType ) * tColumns;
		static constexpr auto sSizeInBytes = sComponentSizeInBytes * sAttribComponentsNum;

		static constexpr auto sBaseAttribFormat = ( EVertexAttribFormat )CxDef::declareVertexAttribFormat(
				QBaseScalarDataTypeTraits<TScalar>::sBaseDataType,
				tColumns,
				QBaseScalarDataTypeTraits<TScalar>::sBaseDataFlags );
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
