
#include "CameraController.h"

namespace Ic3
{

	CameraController::CameraController()
	: mCameraState( _cameraState )
	{}

	void CameraController::Initialize( const cxm::vec3f & pOrigin, const cxm::vec3f & pTarget, float pFOVAngle )
	{
		auto vForward = pTarget - pOrigin;
		_cameraState.configuration.movementSensitivity = 1.0f;
		_cameraState.configuration.rollSensitivity = 0.0975f;
		_cameraState.configuration.rotationSensitivity = 0.0075f;
		_cameraState.orientation.origin = pOrigin;
		_cameraState.orientation.target = pTarget;
		_cameraState.orientation.originTargetDistance = cxm::length( vForward );
		_cameraState.orientation.vForward = cxm::normalize( vForward );
		_cameraState.orientation.vRight = cxm::normalize( cxm::cross( cxm::vec3f( 0, 1, 0 ), _cameraState.orientation.vForward ) );
		_cameraState.orientation.vUp = cxm::normalize( cxm::cross( _cameraState.orientation.vForward, _cameraState.orientation.vRight ) );
		_cameraState.rotation.roll = 0.0f;

		auto xzLength = cxm::length( cxm::vec2f( _cameraState.orientation.vForward.x, _cameraState.orientation.vForward.z ) );
		auto xzSin = _cameraState.orientation.vForward.x / xzLength;
		auto xzCos = _cameraState.orientation.vForward.z / xzLength;
		_cameraState.rotation.yaw = static_cast<float>( atan2( xzSin, xzCos ) );

		auto yzLength = cxm::length( cxm::vec2f( _cameraState.orientation.vForward.y, _cameraState.orientation.vForward.z ) );
		auto yzSin = _cameraState.orientation.vForward.y / yzLength;
		auto yzCos = _cameraState.orientation.vForward.z / yzLength;
		_cameraState.rotation.pitch = static_cast<float>( atan2( yzSin, yzCos ) );

		_cameraState.projection.fovAngleDiff = 0.0f;
		_cameraState.projection.fovAngle = pFOVAngle;
	}

	void CameraController::SetTarget( const cxm::vec2f & pTarget )
	{
		auto target = cxm::vec3f( pTarget.x, pTarget.y, _cameraState.orientation.target.z );
		auto vForward = target - _cameraState.orientation.origin;

		_cameraState.orientation.target = target;
		_cameraState.orientation.originTargetDistance = cxm::length( vForward );
		_cameraState.orientation.vForward = cxm::normalize( vForward );

		UpdateRotation();
	}

	void CameraController::Move( float pFactor )
	{
		_cameraState.orientation.origin += pFactor * _cameraState.orientation.vForward;
		_cameraState.orientation.target = _cameraState.orientation.origin + ( _cameraState.orientation.vForward * _cameraState.orientation.originTargetDistance );
	}

	void CameraController::MoveFlat( float pFactor )
	{
		_cameraState.orientation.origin += pFactor * cxm::vec3f( _cameraState.orientation.vForward.x, 0.0f, _cameraState.orientation.vForward.z );
		_cameraState.orientation.target = _cameraState.orientation.origin + ( _cameraState.orientation.vForward * _cameraState.orientation.originTargetDistance );
	}

	void CameraController::MoveSide( float pFactor )
	{
		_cameraState.orientation.origin += pFactor * _cameraState.orientation.vRight;
		_cameraState.orientation.target = _cameraState.orientation.origin + ( _cameraState.orientation.vForward * _cameraState.orientation.originTargetDistance );
	}

	void CameraController::MoveSideFlat( float pFactor )
	{
		_cameraState.orientation.origin += pFactor * cxm::vec3f( _cameraState.orientation.vRight.x, 0.0f, _cameraState.orientation.vRight.z );
		_cameraState.orientation.target = _cameraState.orientation.origin + ( _cameraState.orientation.vForward * _cameraState.orientation.originTargetDistance );
	}

	void CameraController::MoveUpDown( float pFactor )
	{
		_cameraState.orientation.origin += pFactor * cxm::vec3f( 0, 1, 0 );
		_cameraState.orientation.target = _cameraState.orientation.origin + ( _cameraState.orientation.vForward * _cameraState.orientation.originTargetDistance );
	}

	void CameraController::Roll( float pRoll )
	{
		_cameraState.rotation.roll += ( pRoll * _cameraState.configuration.rollSensitivity );

		UpdateRotation();
	}

	void CameraController::RotateAroundOrigin( float pYaw, float pPitch )
	{
		_cameraState.rotation.yaw += ( pYaw * _cameraState.configuration.rotationSensitivity );
		_cameraState.rotation.pitch -= ( pPitch * _cameraState.configuration.rotationSensitivity );

		UpdateRotation();

		_cameraState.orientation.target = _cameraState.orientation.origin + ( _cameraState.orientation.vForward * _cameraState.orientation.originTargetDistance );
	}

	void CameraController::RotateAroundTarget( float pYaw, float pPitch )
	{
		_cameraState.rotation.yaw += ( pYaw * _cameraState.configuration.rotationSensitivity );
		_cameraState.rotation.pitch -= ( pPitch * _cameraState.configuration.rotationSensitivity );

		UpdateRotation();

		_cameraState.orientation.origin = _cameraState.orientation.target - ( _cameraState.orientation.vForward * _cameraState.orientation.originTargetDistance );
	}

	void CameraController::Zoom( int32 pZoomFactor )
	{
		if( ( pZoomFactor > 0 ) && ( _cameraState.projection.zoomLevel == _cameraState.configuration.maxZoomLevel ) )
		{
			return;
		}
		if( ( pZoomFactor < 0 ) && ( _cameraState.projection.zoomLevel == 1 ) )
		{
			return;
		}
		if( pZoomFactor == 0 )
		{
			return;
		}
		if( pZoomFactor > 0 )
		{
			pZoomFactor = 1;
		}
		if( pZoomFactor < 0 )
		{
			pZoomFactor = -1;
		}

		_cameraState.projection.zoomLevel += pZoomFactor;

		const auto zoomLevel = _cameraState.projection.zoomLevel - 1;
		const auto fovModifier = _cameraState.configuration.zoomBaseFactor * zoomLevel;//( 2.0f - 1.0f / static_cast<float>( ( zoomLevel == 1 ) ? 1 : ( 2 << ( zoomLevel - 1 ) ) ) );

		_cameraState.projection.fovAngleDiff = fovModifier;
	}

	cxm::mat4f CameraController::ComputeViewMatrixLH()
	{
		auto viewMatrix = cxm::look_at_LH(
				_cameraState.orientation.origin,
				_cameraState.orientation.target,
				_cameraState.orientation.vUp );
		return viewMatrix;
	}

	cxm::mat4f CameraController::ComputeViewMatrixRH()
	{
		auto viewMatrix = cxm::look_at_RH(
				_cameraState.orientation.origin,
				_cameraState.orientation.target,
				_cameraState.orientation.vUp );
		return viewMatrix;
	}

	float CameraController::GetPerspectiveFOVAngle() const
	{
		auto fovAngle = _cameraState.projection.fovAngle - _cameraState.projection.fovAngleDiff;
		return fovAngle * cxm::constants::FLT_RAD_1DEG;
	}

	void CameraController::UpdateRotation()
	{
		constexpr auto cxFullAngle = cxm::constants::FLT_RAD_360DEG;
		
		if( _cameraState.rotation.yaw > cxFullAngle )
		{
			_cameraState.rotation.yaw -= cxFullAngle;
		}
		else if( _cameraState.rotation.yaw < -cxFullAngle )
		{
			_cameraState.rotation.yaw += cxFullAngle;
		}
		
		if( _cameraState.rotation.pitch > cxFullAngle )
		{
			_cameraState.rotation.pitch -= cxFullAngle;
		}
		else if( _cameraState.rotation.pitch < -cxFullAngle )
		{
			_cameraState.rotation.pitch += cxFullAngle;
		}
		
		if( _cameraState.rotation.roll > cxFullAngle )
		{
			_cameraState.rotation.roll -= cxFullAngle;
		}
		else if( _cameraState.rotation.roll < -cxFullAngle )
		{
			_cameraState.rotation.roll += cxFullAngle;
		}

		auto sinPitch = std::sin( _cameraState.rotation.pitch );
		auto cosPitch = std::cos( _cameraState.rotation.pitch );
		auto sinYaw = std::sin( _cameraState.rotation.yaw );
		auto cosYaw = std::cos( _cameraState.rotation.yaw );
		auto sinRoll = std::sin( _cameraState.rotation.roll );
		auto cosRoll = std::cos( _cameraState.rotation.roll );

		cxm::vec3f vcForward;
		vcForward.x = cosPitch * sinYaw;
		vcForward.y = sinPitch;
		vcForward.z = cosPitch * cosYaw;

		cxm::vec3f vcUp;
		vcUp.x = -sinRoll * cosYaw - cosRoll * sinPitch * sinYaw;
		vcUp.y =  cosRoll * cosPitch;
		vcUp.z =  sinRoll * sinYaw - cosRoll * sinPitch * cosYaw;

		_cameraState.orientation.vForward = vcForward;
		_cameraState.orientation.vUp = vcUp;
		_cameraState.orientation.vRight = cxm::normalize( cxm::cross( _cameraState.orientation.vUp, _cameraState.orientation.vForward ) );
	}

} // namespace Ic3
