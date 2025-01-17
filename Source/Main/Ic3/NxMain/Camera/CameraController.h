
#pragma once

#ifndef __IC3_NXMAIN_CAMERA_CAMERA_CONTROLLER_H__
#define __IC3_NXMAIN_CAMERA_CAMERA_CONTROLLER_H__

#include "CameraCommonDefs.h"

namespace Ic3
{

	class IC3_NXMAIN_CLASS CameraController
	{
	public:
		const CameraState & mCameraState;

		CameraController();

		void Initialize( const Math::Vec3f & pOrigin, const Math::Vec3f & pTarget, float pFOVAngle );

		void SetTarget( const Math::Vec2f & pTarget );

		void Move( float pFactor = 1.0f );
		void MoveFlat( float pFactor = 1.0f );
		void MoveSide( float pFactor = 1.0f );
		void MoveSideFlat( float pFactor = 1.0f );
		void MoveUpDown( float pFactor = 1.0f );
		void Roll( float pRoll );
		void RotateAroundOrigin( float pYaw, float pPitch );
		void RotateAroundTarget( float pYaw, float pPitch );
		void Zoom( int32 pZoomFactor );

		Math::Mat4f ComputeViewMatrixLH();
		Math::Mat4f ComputeViewMatrixRH();

		float GetPerspectiveFOVAngle() const;

	private:
		void UpdateRotation();

	private:
		CameraState _cameraState;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_CAMERA_CAMERA_CONTROLLER_H__
