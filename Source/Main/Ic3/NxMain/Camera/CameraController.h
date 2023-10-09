
#pragma once

#ifndef __IC3_NXMAIN_CAMERA_CAMERA_CONTROLLER_H__
#define __IC3_NXMAIN_CAMERA_CAMERA_CONTROLLER_H__

#include "cameraCommonDefs.h"

namespace Ic3
{

	class IC3_NXMAIN_CLASS CameraController
	{
	public:
		CameraState mCameraState;

		void initialize( const Math::Vec3f & pOrigin, const Math::Vec3f & pTarget, float pFOVAngle );

		void setTarget( const Math::Vec2f & pTarget );

		void move( float pFactor = 1.0f );
		void moveFlat( float pFactor = 1.0f );
		void moveSide( float pFactor = 1.0f );
		void moveSideFlat( float pFactor = 1.0f );
		void moveUpDown( float pFactor = 1.0f );
		void roll( float pRoll );
		void rotateAroundOrigin( float pYaw, float pPitch );
		void rotateAroundTarget( float pYaw, float pPitch );
		void zoom( int32 pZoomFactor );

		Math::Mat4f computeViewMatrixLH();
		Math::Mat4f computeViewMatrixRH();

		float getPerspectiveFOVAngle() const;

	private:
		void updateRotation();
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_CAMERA_CAMERA_CONTROLLER_H__
