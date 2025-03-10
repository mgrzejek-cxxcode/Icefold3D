
#ifndef __CXM_MATRIX_UTILS_H__
#define __CXM_MATRIX_UTILS_H__

#include "matrixOps.h"

namespace cxm
{

	template <typename TPValue>
	inline matrix2x2<TPValue> identity2()
	{
		constexpr auto cxOne = static_cast<TPValue>( 1 );
		constexpr auto cxZero = static_cast<TPValue>( 0 );

		return matrix2x2<TPValue> {
			cxOne,  cxZero,
			cxZero, cxOne
		};
	}

	template <typename TPValue>
	inline matrix3x3<TPValue> identity3()
	{
		constexpr auto cxOne = static_cast<TPValue>( 1 );
		constexpr auto cxZero = static_cast<TPValue>( 0 );

		return matrix3x3<TPValue> {
			cxOne,  cxZero, cxZero,
			cxZero, cxOne,  cxZero,
			cxZero, cxZero, cxOne
		};
	}

	template <typename TPValue>
	inline matrix4x4<TPValue> identity4()
	{
		constexpr auto cxOne = static_cast<TPValue>( 1 );
		constexpr auto cxZero = static_cast<TPValue>( 0 );

		return matrix4x4<TPValue> {
			cxOne,  cxZero, cxZero, cxZero,
			cxZero, cxOne,  cxZero, cxZero,
			cxZero, cxZero, cxOne,  cxZero,
			cxZero, cxZero, cxZero, cxOne
		};
	}

	template <typename TPValue>
	inline void transpose_inplace(matrix2x2<TPValue> & pMatrix )
	{
		std::swap( pMatrix[1][0], pMatrix[0][1] );
	}

	template <typename TPValue>
	inline matrix2x2<TPValue> transpose( const matrix2x2<TPValue> & pMatrix )
	{
		return matrix2x2<TPValue> {
			pMatrix[0][0], pMatrix[1][0],
			pMatrix[0][1], pMatrix[1][1]
		};
	}

	template <typename TPValue>
	inline void transpose_inplace( matrix3x3<TPValue> & pMatrix )
	{
		std::swap( pMatrix[1][0], pMatrix[0][1]);
		std::swap( pMatrix[2][0], pMatrix[0][2]);
		std::swap( pMatrix[2][1], pMatrix[1][2]);
	}

	template <typename TPValue>
	inline matrix3x3<TPValue> transpose( const matrix3x3<TPValue> & pMatrix )
	{
		return matrix3x3<TPValue> {
			pMatrix[0][0], pMatrix[1][0], pMatrix[2][0],
			pMatrix[0][1], pMatrix[1][1], pMatrix[2][1],
			pMatrix[0][2], pMatrix[1][2], pMatrix[2][2]
		};
	}

	template <typename TPValue>
	inline void transpose_inplace( matrix4x4<TPValue> & pMatrix )
	{
		std::swap( pMatrix[1][0], pMatrix[0][1]);
		std::swap( pMatrix[2][0], pMatrix[0][2]);
		std::swap( pMatrix[2][1], pMatrix[1][2]);
		std::swap( pMatrix[3][0], pMatrix[0][3]);
		std::swap( pMatrix[3][1], pMatrix[1][3]);
		std::swap( pMatrix[3][2], pMatrix[2][3]);
	}

	template <typename TPValue>
	inline matrix4x4<TPValue> transpose( const matrix4x4<TPValue> & pMatrix )
	{
		return matrix4x4<TPValue> {
			pMatrix[0][0], pMatrix[1][0], pMatrix[2][0], pMatrix[3][0],
			pMatrix[0][1], pMatrix[1][1], pMatrix[2][1], pMatrix[3][1],
			pMatrix[0][2], pMatrix[1][2], pMatrix[2][2], pMatrix[3][2],
			pMatrix[0][3], pMatrix[1][3], pMatrix[2][3], pMatrix[3][3]
		};
	}

	template <typename TPValue>
	inline matrix2x2<TPValue> inverse_transpose( const matrix2x2<TPValue> & pMatrix )
	{
		const TPValue det = pMatrix[0][0] * pMatrix[1][1] - pMatrix[1][0] * pMatrix[0][1];

		return matrix2x2<TPValue> {
			+ ( pMatrix[1][1] / det ),
			- ( pMatrix[0][1] / det ),
			- ( pMatrix[1][0] / det ),
			+ ( pMatrix[0][0] / det )
		};
	}

	template <typename TPValue>
	inline matrix3x3<TPValue> inverse( const matrix3x3<TPValue> & pMatrix )
	{
		const auto oneOverDet = static_cast<TPValue>( 1 ) / (
				+ pMatrix[0][0] * (pMatrix[1][1] * pMatrix[2][2] - pMatrix[2][1] * pMatrix[1][2])
				- pMatrix[1][0] * (pMatrix[0][1] * pMatrix[2][2] - pMatrix[2][1] * pMatrix[0][2])
				+ pMatrix[2][0] * (pMatrix[0][1] * pMatrix[1][2] - pMatrix[1][1] * pMatrix[0][2]));

		return {
			+ (pMatrix[1][1] * pMatrix[2][2] - pMatrix[2][1] * pMatrix[1][2]) * oneOverDet,
			- (pMatrix[1][0] * pMatrix[2][2] - pMatrix[2][0] * pMatrix[1][2]) * oneOverDet,
			+ (pMatrix[1][0] * pMatrix[2][1] - pMatrix[2][0] * pMatrix[1][1]) * oneOverDet,
			- (pMatrix[0][1] * pMatrix[2][2] - pMatrix[2][1] * pMatrix[0][2]) * oneOverDet,
			+ (pMatrix[0][0] * pMatrix[2][2] - pMatrix[2][0] * pMatrix[0][2]) * oneOverDet,
			- (pMatrix[0][0] * pMatrix[2][1] - pMatrix[2][0] * pMatrix[0][1]) * oneOverDet,
			+ (pMatrix[0][1] * pMatrix[1][2] - pMatrix[1][1] * pMatrix[0][2]) * oneOverDet,
			- (pMatrix[0][0] * pMatrix[1][2] - pMatrix[1][0] * pMatrix[0][2]) * oneOverDet,
			+ (pMatrix[0][0] * pMatrix[1][1] - pMatrix[1][0] * pMatrix[0][1]) * oneOverDet
		};
	}

	template <typename TPValue>
	inline matrix3x3<TPValue> inverse_transpose( const matrix3x3<TPValue> & pMatrix )
	{
		const TPValue det =
			+ pMatrix[0][0] * ( pMatrix[1][1] * pMatrix[2][2] - pMatrix[1][2] * pMatrix[2][1] )
			- pMatrix[0][1] * ( pMatrix[1][0] * pMatrix[2][2] - pMatrix[1][2] * pMatrix[2][0] )
			+ pMatrix[0][2] * ( pMatrix[1][0] * pMatrix[2][1] - pMatrix[1][1] * pMatrix[2][0] );

		return matrix3x3<TPValue> {
			( + ( pMatrix[1][1] * pMatrix[2][2] - pMatrix[2][1] * pMatrix[1][2] ) / det ),
			( - ( pMatrix[1][0] * pMatrix[2][2] - pMatrix[2][0] * pMatrix[1][2] ) / det ),
			( + ( pMatrix[1][0] * pMatrix[2][1] - pMatrix[2][0] * pMatrix[1][1] ) / det ),
			( - ( pMatrix[0][1] * pMatrix[2][2] - pMatrix[2][1] * pMatrix[0][2] ) / det ),
			( + ( pMatrix[0][0] * pMatrix[2][2] - pMatrix[2][0] * pMatrix[0][2] ) / det ),
			( - ( pMatrix[0][0] * pMatrix[2][1] - pMatrix[2][0] * pMatrix[0][1] ) / det ),
			( + ( pMatrix[0][1] * pMatrix[1][2] - pMatrix[1][1] * pMatrix[0][2] ) / det ),
			( - ( pMatrix[0][0] * pMatrix[1][2] - pMatrix[1][0] * pMatrix[0][2] ) / det ),
			( + ( pMatrix[0][0] * pMatrix[1][1] - pMatrix[1][0] * pMatrix[0][1] ) / det ),
		};
	}

	template <typename TPValue>
	inline matrix4x4<TPValue> inverse_transpose( const matrix4x4<TPValue> & pMatrix )
	{
		const TPValue tmp00 = pMatrix[2][2] * pMatrix[3][3] - pMatrix[3][2] * pMatrix[2][3];
		const TPValue tmp01 = pMatrix[2][1] * pMatrix[3][3] - pMatrix[3][1] * pMatrix[2][3];
		const TPValue tmp02 = pMatrix[2][1] * pMatrix[3][2] - pMatrix[3][1] * pMatrix[2][2];
		const TPValue tmp03 = pMatrix[2][0] * pMatrix[3][3] - pMatrix[3][0] * pMatrix[2][3];
		const TPValue tmp04 = pMatrix[2][0] * pMatrix[3][2] - pMatrix[3][0] * pMatrix[2][2];
		const TPValue tmp05 = pMatrix[2][0] * pMatrix[3][1] - pMatrix[3][0] * pMatrix[2][1];
		const TPValue tmp06 = pMatrix[1][2] * pMatrix[3][3] - pMatrix[3][2] * pMatrix[1][3];
		const TPValue tmp07 = pMatrix[1][1] * pMatrix[3][3] - pMatrix[3][1] * pMatrix[1][3];
		const TPValue tmp08 = pMatrix[1][1] * pMatrix[3][2] - pMatrix[3][1] * pMatrix[1][2];
		const TPValue tmp09 = pMatrix[1][0] * pMatrix[3][3] - pMatrix[3][0] * pMatrix[1][3];
		const TPValue tmp10 = pMatrix[1][0] * pMatrix[3][2] - pMatrix[3][0] * pMatrix[1][2];
		const TPValue tmp11 = pMatrix[1][0] * pMatrix[3][1] - pMatrix[3][0] * pMatrix[1][1];
		const TPValue tmp12 = pMatrix[1][2] * pMatrix[2][3] - pMatrix[2][2] * pMatrix[1][3];
		const TPValue tmp13 = pMatrix[1][1] * pMatrix[2][3] - pMatrix[2][1] * pMatrix[1][3];
		const TPValue tmp14 = pMatrix[1][1] * pMatrix[2][2] - pMatrix[2][1] * pMatrix[1][2];
		const TPValue tmp15 = pMatrix[1][0] * pMatrix[2][3] - pMatrix[2][0] * pMatrix[1][3];
		const TPValue tmp16 = pMatrix[1][0] * pMatrix[2][2] - pMatrix[2][0] * pMatrix[1][2];
		const TPValue tmp17 = pMatrix[1][0] * pMatrix[2][1] - pMatrix[2][0] * pMatrix[1][1];

		matrix4x4<TPValue> tmpInv;

		tmpInv[0][0] = + ( pMatrix[1][1] * tmp00 - pMatrix[1][2] * tmp01 + pMatrix[1][3] * tmp02 );
		tmpInv[0][1] = - ( pMatrix[1][0] * tmp00 - pMatrix[1][2] * tmp03 + pMatrix[1][3] * tmp04 );
		tmpInv[0][2] = + ( pMatrix[1][0] * tmp01 - pMatrix[1][1] * tmp03 + pMatrix[1][3] * tmp05 );
		tmpInv[0][3] = - ( pMatrix[1][0] * tmp02 - pMatrix[1][1] * tmp04 + pMatrix[1][2] * tmp05 );

		tmpInv[1][0] = - ( pMatrix[0][1] * tmp00 - pMatrix[0][2] * tmp01 + pMatrix[0][3] * tmp02 );
		tmpInv[1][1] = + ( pMatrix[0][0] * tmp00 - pMatrix[0][2] * tmp03 + pMatrix[0][3] * tmp04 );
		tmpInv[1][2] = - ( pMatrix[0][0] * tmp01 - pMatrix[0][1] * tmp03 + pMatrix[0][3] * tmp05 );
		tmpInv[1][3] = + ( pMatrix[0][0] * tmp02 - pMatrix[0][1] * tmp04 + pMatrix[0][2] * tmp05 );

		tmpInv[2][0] = + ( pMatrix[0][1] * tmp06 - pMatrix[0][2] * tmp07 + pMatrix[0][3] * tmp08 );
		tmpInv[2][1] = - ( pMatrix[0][0] * tmp06 - pMatrix[0][2] * tmp09 + pMatrix[0][3] * tmp10 );
		tmpInv[2][2] = + ( pMatrix[0][0] * tmp07 - pMatrix[0][1] * tmp09 + pMatrix[0][3] * tmp11 );
		tmpInv[2][3] = - ( pMatrix[0][0] * tmp08 - pMatrix[0][1] * tmp10 + pMatrix[0][2] * tmp11 );

		tmpInv[3][0] = - ( pMatrix[0][1] * tmp12 - pMatrix[0][2] * tmp13 + pMatrix[0][3] * tmp14 );
		tmpInv[3][1] = + ( pMatrix[0][0] * tmp12 - pMatrix[0][2] * tmp15 + pMatrix[0][3] * tmp16 );
		tmpInv[3][2] = - ( pMatrix[0][0] * tmp13 - pMatrix[0][1] * tmp15 + pMatrix[0][3] * tmp17 );
		tmpInv[3][3] = + ( pMatrix[0][0] * tmp14 - pMatrix[0][1] * tmp16 + pMatrix[0][2] * tmp17 );

		const TPValue det =
			+ ( pMatrix[0][0] * tmpInv[0][0] )
			+ ( pMatrix[0][1] * tmpInv[0][1] )
			+ ( pMatrix[0][2] * tmpInv[0][2] )
			+ ( pMatrix[0][3] * tmpInv[0][3] );

		return tmpInv / det;
	}

	template <typename TPValue>
	inline matrix4x4<TPValue> look_at_LH( const vec3<TPValue> & pEyePos, const vec3<TPValue> & pLookAt, const vec3<TPValue> & pUpVec )
	{
		const auto zAxis = normalize( pLookAt - pEyePos );
		const auto xAxis = normalize( cross( pUpVec, zAxis ) );
		const auto yAxis = cross( zAxis, xAxis );

		const TPValue xAxisEyeDot = dot( xAxis, pEyePos );
		const TPValue yAxisEyeDot = dot( yAxis, pEyePos );
		const TPValue zAxisEyeDot = dot( zAxis, pEyePos );

		return matrix4x4<TPValue> {
				xAxis.x, xAxis.y, xAxis.z, -xAxisEyeDot,
				yAxis.x, yAxis.y, yAxis.z, -yAxisEyeDot,
				zAxis.x, zAxis.y, zAxis.z, -zAxisEyeDot,
				0, 0, 0, 1
		};
	}

	template <typename TPValue>
	inline matrix4x4<TPValue> look_at_RH( const vec3<TPValue> & pEyePos, const vec3<TPValue> & pLookAt, const vec3<TPValue> & pUpVec )
	{
		const auto zAxis = normalize( pLookAt - pEyePos );
		const auto xAxis = normalize( cross( zAxis, pUpVec ) );
		const auto yAxis = cross( xAxis, zAxis );

		const TPValue xAxisEyeDot = dot( xAxis, pEyePos );
		const TPValue yAxisEyeDot = dot( yAxis, pEyePos );
		const TPValue zAxisEyeDot = dot( zAxis, pEyePos );

		return matrix4x4<TPValue> {
				xAxis.x, xAxis.y, xAxis.z, -xAxisEyeDot,
				yAxis.x, yAxis.y, yAxis.z, -yAxisEyeDot,
				-zAxis.x, -zAxis.y, -zAxis.z, zAxisEyeDot,
				0, 0, 0, 1
		};
	}

	template <typename TPValue>
	inline matrix4x4<TPValue> ortho_center_LH( TPValue pWidth, TPValue pHeight, TPValue pZNear, TPValue pZFar )
	{
		const TPValue mElem00 = static_cast<TPValue>( 2 ) / pWidth;
		const TPValue mElem11 = static_cast<TPValue>( 2 ) / pHeight;
		const TPValue mElem22 = static_cast<TPValue>( 1 ) / ( pZFar - pZNear );
		const TPValue mElem23 = pZNear / ( pZNear - pZFar );

		return matrix4x4<TPValue> {
			mElem00, 0,       0,       0,
			0,       mElem11, 0,       0,
			0,       0,       mElem22, mElem23,
			0,       0,       0,       1
		};
	}

	template <typename TPValue>
	inline matrix4x4<TPValue> ortho_off_center_LH( TPValue pLeft, TPValue pRight, TPValue pBottom, TPValue pTop, TPValue pZNear, TPValue pZFar )
	{
		const TPValue mElem00 = static_cast<TPValue>( 2 ) / ( pRight - pLeft );
		const TPValue mElem11 = static_cast<TPValue>( 2 ) / ( pTop - pBottom );
		const TPValue mElem22 = static_cast<TPValue>( 1 ) / ( pZFar - pZNear );
		const TPValue mElem03 = ( pLeft + pRight ) / ( pLeft - pRight );
		const TPValue mElem13 = ( pBottom + pTop ) / ( pBottom - pTop );
		const TPValue mElem23 = pZNear / ( pZNear - pZFar );

		return matrix4x4<TPValue> {
				mElem00, 0,       0,       mElem03,
				0,       mElem11, 0,       mElem13,
				0,       0,       mElem22, mElem23,
				0,       0,       0,       1
		};
	}

	template <typename TPValue>
	inline matrix4x4<TPValue> ortho_center_RH( TPValue pWidth, TPValue pHeight, TPValue pZNear, TPValue pZFar )
	{
		const TPValue mElem00 = static_cast<TPValue>( 2 ) / pWidth;
		const TPValue mElem11 = static_cast<TPValue>( 2 ) / pHeight;
		const TPValue mElem22 = static_cast<TPValue>( 1 ) / ( pZNear - pZFar );
		const TPValue mElem23 = pZNear / ( pZNear - pZFar );

		return matrix4x4<TPValue> {
			mElem00, 0,       0,       0,
			0,       mElem11, 0,       0,
			0,       0,       mElem22, mElem23,
			0,       0,       0,       1
		};
	}

	template <typename TPValue>
	inline matrix4x4<TPValue> ortho_off_center_RH( TPValue pLeft, TPValue pRight, TPValue pBottom, TPValue pTop, TPValue pZNear, TPValue pZFar )
	{
		const TPValue mElem00 = static_cast<TPValue>( 2 ) / ( pRight - pLeft );
		const TPValue mElem11 = static_cast<TPValue>( 2 ) / ( pTop - pBottom );
		const TPValue mElem22 = static_cast<TPValue>( 1 ) / ( pZNear - pZFar );
		const TPValue mElem03 = ( pLeft + pRight ) / ( pLeft - pRight );
		const TPValue mElem13 = ( pBottom + pTop ) / ( pBottom - pTop );
		const TPValue mElem23 = pZNear / ( pZNear - pZFar );

		return matrix4x4<TPValue> {
				mElem00, 0,       0,       mElem03,
				0,       mElem11, 0,       mElem13,
				0,       0,       mElem22, mElem23,
				0,       0,       0,       1
		};
	}

	template <typename TPValue>
	inline matrix4x4<TPValue> perspective_aspect_LH( TPValue pFOV, TPValue pAspectRatio, TPValue pZNear, TPValue pZFar )
	{
		const TPValue halfTanFOV = std::tan( static_cast<TPValue>( 0.5 ) * pFOV );

		const TPValue mElem00 = static_cast<TPValue>( 1 ) / ( pAspectRatio * halfTanFOV );
		const TPValue mElem11 = static_cast<TPValue>( 1 ) / halfTanFOV;
		const TPValue mElem22 = ( pZFar + pZNear ) / ( pZFar - pZNear );
		const TPValue mElem23 = ( 2 * pZFar * pZNear ) / ( pZNear - pZFar );

		return matrix4x4<TPValue> {
			mElem00, 0,       0,       0,
			0,       mElem11, 0,       0,
			0,       0,       mElem22, mElem23,
			0,       0,       1,       0
		};
	}

	template <typename TPValue>
	inline matrix4x4<TPValue> perspective_aspect_RH( TPValue pFOV, TPValue pAspectRatio, TPValue pZNear, TPValue pZFar )
	{
		const TPValue halfTanFOV = std::tan( static_cast<TPValue>( 0.5 ) * pFOV );

		const TPValue mElem00 = static_cast<TPValue>( 1 ) / ( pAspectRatio * halfTanFOV );
		const TPValue mElem11 = static_cast<TPValue>( 1 ) / halfTanFOV;
		const TPValue mElem22 = ( pZFar + pZNear ) / ( pZNear - pZFar );
		const TPValue mElem23 = ( 2 * pZFar * pZNear ) / ( pZNear - pZFar );

		return matrix4x4<TPValue> {
			mElem00, 0,       0,       0,
			0,       mElem11, 0,       0,
			0,       0,       mElem22, mElem23,
			0,       0,       -1,      0
		};
	}

	template <typename TPValue>
	inline matrix4x4<TPValue> perspective_rect_LH( TPValue pFOV, TPValue pWidth, TPValue pHeight, TPValue pZNear, TPValue pZFar )
	{
		const TPValue fovFactor = std::cos( static_cast<TPValue>( 0.5 ) * pFOV ) / std::sin( static_cast<TPValue>( 0.5 ) * pFOV );

		const TPValue mElem00 = fovFactor * pHeight / pWidth;
		const TPValue mElem11 = fovFactor;
		const TPValue mElem22 = ( pZFar + pZNear ) / ( pZFar - pZNear );
		const TPValue mElem23 = ( 2 * pZFar * pZNear ) / ( pZNear - pZFar );

		return matrix4x4<TPValue> {
			mElem00, 0,       0,       0,
			0,       mElem11, 0,       0,
			0,       0,       mElem22, mElem23,
			0,       0,       1,       0
		};
	}

	template <typename TPValue>
	inline matrix4x4<TPValue> perspective_rect_RH( TPValue pFOV, TPValue pWidth, TPValue pHeight, TPValue pZNear, TPValue pZFar )
	{
		const TPValue fovFactor = std::cos( static_cast<TPValue>( 0.5 ) * pFOV ) / std::sin( static_cast<TPValue>( 0.5 ) * pFOV );

		const TPValue mElem00 = fovFactor * pHeight / pWidth;
		const TPValue mElem11 = fovFactor;
		const TPValue mElem22 = ( pZFar + pZNear ) / ( pZFar - pZNear );
		const TPValue mElem23 = ( 2 * pZFar * pZNear ) / ( pZNear - pZFar );

		return matrix4x4<TPValue> {
			mElem00, 0,        0,       0,
			0,       mElem11,  0,       0,
			0,       0,        mElem22, mElem23,
			0,       0,       -1,       0
		};
	}

	template <typename TPValue>
	inline matrix4x4<TPValue> rotation_axis_X( TPValue pAngle )
	{
		const TPValue angleSin = std::sin( pAngle );
		const TPValue angleCos = std::cos( pAngle );

		return matrix4x4<TPValue> {
			1, 0,         0,        0,
			0, angleCos, -angleSin, 0,
			0, angleSin,  angleCos, 0,
			0, 0,         0,        1
		};
	}

	template <typename TPValue>
	inline matrix4x4<TPValue> rotation_axis_Y( TPValue pAngle )
	{
		const TPValue angleSin = std::sin( pAngle );
		const TPValue angleCos = std::cos( pAngle );

		return matrix4x4<TPValue> {
			 angleCos, 0, angleSin, 0,
			 0,        1, 0,        0,
			-angleSin, 0, angleCos, 0,
			 0,        0, 0,        1
		};
	}

	template <typename TPValue>
	inline matrix4x4<TPValue> rotation_axis_Z( TPValue pAngle )
	{
		const TPValue angleSin = std::sin( pAngle );
		const TPValue angleCos = std::cos( pAngle );

		return matrix4x4<TPValue> {
			angleCos, -angleSin, 0, 0,
			angleSin,  angleCos, 0, 0,
			0,         0,        1, 0,
			0,         0,        0, 1
		};
	}

	template <typename TPValue>
	inline matrix4x4<TPValue> rotation_combined( const vector3<TPValue> & pAxis, TPValue pAngle )
	{
		const TPValue angleSin = std::sin( pAngle );
		const TPValue angleCos = std::cos( pAngle );
		const auto normalizedAxis = normalize( pAxis );
		const auto icnAxis = normalizedAxis * ( static_cast<TPValue>( 1 ) - angleCos );

		matrix3x3<TPValue> rMat;
		rMat[0][0] = pAxis.x * icnAxis.x + angleCos;
		rMat[0][1] = pAxis.x * icnAxis.y - pAxis.z * angleSin;
		rMat[0][2] = pAxis.x * icnAxis.z + pAxis.y * angleSin;

		rMat[1][0] = pAxis.y * icnAxis.x + pAxis.z * angleSin;
		rMat[1][1] = pAxis.y * icnAxis.y + angleCos;
		rMat[1][2] = pAxis.y * icnAxis.z - pAxis.x * angleSin;

		rMat[2][0] = pAxis.z * icnAxis.x - pAxis.y * angleSin;
		rMat[2][1] = pAxis.z * icnAxis.y + pAxis.y * angleSin;
		rMat[2][2] = pAxis.z * icnAxis.z + angleCos;

		return matrix4x4<TPValue> {
			rMat.row0.x, rMat.row0.y, rMat.row0.z, 0,
			rMat.row1.x, rMat.row1.y, rMat.row1.z, 0,
			rMat.row2.x, rMat.row2.y, rMat.row2.z, 0,
			0,           0,           0,           1
		};
	}

	template <typename TPValue>
	inline matrix4x4<TPValue> scaling( TPValue pScaleX, TPValue pScaleY, TPValue pScaleZ )
	{
		return matrix4x4<TPValue> {
			pScaleX, 0,       0,       0,
			0,       pScaleY, 0,       0,
			0,       0,       pScaleZ, 0,
			0,       0,       0,       1
		};
	}

	template <typename TPValue>
	inline matrix4x4<TPValue> translation( TPValue pMoveX, TPValue pMoveY, TPValue pMoveZ )
	{
		return matrix4x4<TPValue> {
			1, 0, 0, pMoveX,
			0, 1, 0, pMoveY,
			0, 0, 1, pMoveZ,
			0, 0, 0, 1
		};
	}

}

#if( CXM_SIMD_ENABLE )
#  include "matrixUtilsSIMD.inl"
#endif

#endif // __CXM_MATRIX_UTILS_H__
