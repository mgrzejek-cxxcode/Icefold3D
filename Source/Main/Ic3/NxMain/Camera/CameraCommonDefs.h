
#pragma once

#ifndef __IC3_NXMAIN_CAMERA_CAMERA_COMMON_DEFS_H__
#define __IC3_NXMAIN_CAMERA_CAMERA_COMMON_DEFS_H__

#include "../Prerequisites.h"

namespace Ic3
{

	class CameraProjection
	{
	};

	class CameraProjectionOrtho : public CameraProjection
	{
	};

	class CameraProjectionPerspective : public CameraProjection
	{
	};

	struct CameraState
	{
		struct Configuration
		{
			float movementSensitivity;
			float rollSensitivity;
			float rotationSensitivity;
			uint32 maxZoomLevel = 8;
			float zoomBaseFactor = 4.0f;
		};

		struct Orientation
		{
			cxm::vec3f origin;
			cxm::vec3f target;
			cxm::vec3f vForward;
			cxm::vec3f vRight;
			cxm::vec3f vUp;
			float originTargetDistance;
		};

		struct Projection
		{
			float fovAngle;
			float fovAngleDiff;
			uint32 zoomLevel = 1;
		};

		struct Rotation
		{
			float yaw;
			float pitch;
			float roll;
		};

		Configuration configuration;

		Orientation orientation;

		Projection projection;

		Rotation rotation;

	};

} // namespace Ic3

#endif // __IC3_NXMAIN_CAMERA_CAMERA_COMMON_DEFS_H__
