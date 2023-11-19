
#pragma once

#ifndef __IC3_GRAPHICS_GCI_VERTEX_FORMAT_ATTRIB_UTILS_H__
#define __IC3_GRAPHICS_GCI_VERTEX_FORMAT_ATTRIB_UTILS_H__

namespace Ic3::Graphics::GCI
{

	template <typename TScalar>
	struct BaseScalarDataTypeTraits;

	template <>
	struct BaseScalarDataTypeTraits<int8>
	{
		static constexpr auto sBaseDataType = EBaseDataType::Byte;
		static constexpr auto sBaseDataFlags = E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT;
		static constexpr auto sByteSize = 1;
	};

	template <>
	struct BaseScalarDataTypeTraits<uint8>
	{
		static constexpr auto sBaseDataType = EBaseDataType::Ubyte;
		static constexpr auto sBaseDataFlags = E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT;
		static constexpr auto sByteSize = 1;
	};

	template <>
	struct BaseScalarDataTypeTraits<int16>
	{
		static constexpr auto sBaseDataType = EBaseDataType::Int16;
		static constexpr auto sBaseDataFlags = E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT;
		static constexpr auto sByteSize = 2;
	};

	template <>
	struct BaseScalarDataTypeTraits<uint16>
	{
		static constexpr auto sBaseDataType = EBaseDataType::Uint16;
		static constexpr auto sBaseDataFlags = E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT;
		static constexpr auto sByteSize = 2;
	};

	template <>
	struct BaseScalarDataTypeTraits<int32>
	{
		static constexpr auto sBaseDataType = EBaseDataType::Int32;
		static constexpr auto sBaseDataFlags = E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT;
		static constexpr auto sByteSize = 4;
	};

	template <>
	struct BaseScalarDataTypeTraits<uint32>
	{
		static constexpr auto sBaseDataType = EBaseDataType::Uint32;
		static constexpr auto sBaseDataFlags = E_GPU_DATA_FORMAT_FLAG_TYPE_UNSIGNED_BIT;
		static constexpr auto sByteSize = 4;
	};

	template <>
	struct BaseScalarDataTypeTraits<float>
	{
		static constexpr auto sBaseDataType = EBaseDataType::Float32;
		static constexpr auto sBaseDataFlags = E_GPU_DATA_FORMAT_FLAG_TYPE_SIGNED_BIT;
		static constexpr auto sByteSize = 4;
	};

	template <typename TMathType>
	struct VertexAttribFormatDataTypeTraits;

	template <size_t tSize>
	struct VertexAttribFormatDataTypeTraits<Math::Vector<int64, tSize>>;

	template <size_t tSize>
	struct VertexAttribFormatDataTypeTraits<Math::Vector<uint64, tSize>>;

	template <size_t tSize>
	struct VertexAttribFormatDataTypeTraits<Math::Vector<double, tSize>>;

	template <size_t tRows, size_t tColumns>
	struct VertexAttribFormatDataTypeTraits<Math::Matrix<int64, tRows, tColumns>>;

	template <size_t tRows, size_t tColumns>
	struct VertexAttribFormatDataTypeTraits<Math::Matrix<uint64, tRows, tColumns>>;

	template <size_t tRows, size_t tColumns>
	struct VertexAttribFormatDataTypeTraits<Math::Matrix<double, tRows, tColumns>>;

	template <typename TScalar, size_t tSize>
	struct VertexAttribFormatDataTypeTraits<Math::Vector<TScalar, tSize>>
	{
		using BaseDataType = TScalar;

		static constexpr auto sAttribComponentsNum = 1;
		static constexpr auto sScalarValuesNum = tSize;
		static constexpr auto sSizeInBytes = sizeof( BaseDataType ) * sScalarValuesNum;

		static constexpr auto sBaseAttribFormat = ( EVertexAttribFormat )CxDef::declareVertexAttribFormat(
				BaseScalarDataTypeTraits<TScalar>::sBaseDataType,
				tSize,
				BaseScalarDataTypeTraits<TScalar>::sBaseDataFlags );
	};

	template <typename TScalar, size_t tRows, size_t tColumns>
	struct VertexAttribFormatDataTypeTraits<Math::Matrix<TScalar, tRows, tColumns>>
	{
		using BaseDataType = TScalar;

		static constexpr auto sAttribComponentsNum = tRows;
		static constexpr auto sScalarValuesNum = tRows * tColumns;
		static constexpr auto sSizeInBytes = sizeof( BaseDataType ) * sScalarValuesNum;

		static constexpr auto sBaseAttribFormat = ( EVertexAttribFormat )CxDef::declareVertexAttribFormat(
				BaseScalarDataTypeTraits<TScalar>::sBaseDataType,
				tColumns,
				BaseScalarDataTypeTraits<TScalar>::sBaseDataFlags );
	};

	template <>
	struct VertexAttribFormatDataTypeTraits<Math::RGBAColor>
	{
		using BaseDataType = uint8;

		static constexpr auto sAttribComponentsNum = 1;
		static constexpr auto sScalarValuesNum = 4;
		static constexpr auto sSizeInBytes = sizeof( BaseDataType ) * sScalarValuesNum;

		static constexpr auto sBaseAttribFormat = ( EVertexAttribFormat )CxDef::declareVertexAttribFormat(
				BaseScalarDataTypeTraits<uint8>::sBaseDataType,
				4,
				BaseScalarDataTypeTraits<uint8>::sBaseDataFlags );
	};

	template <>
	struct VertexAttribFormatDataTypeTraits<Math::RGBAColorNorm<float>>
	{
		using BaseDataType = float;

		static constexpr auto sAttribComponentsNum = 1;
		static constexpr auto sScalarValuesNum = 4;
		static constexpr auto sSizeInBytes = sizeof( BaseDataType ) * sScalarValuesNum;

		static constexpr auto sBaseAttribFormat = ( EVertexAttribFormat )CxDef::declareVertexAttribFormat(
				BaseScalarDataTypeTraits<float>::sBaseDataType,
				4,
				BaseScalarDataTypeTraits<float>::sBaseDataFlags );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_VERTEX_FORMAT_ATTRIB_UTILS_H__
