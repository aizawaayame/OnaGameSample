
#include "CameraSystem/OnaPlayerCameraBehavior.h"

void UOnaPlayerCameraBehavior::SetRotationMode(EOnaRotationMode rotationMode)
{
	bVelocityDirection = rotationMode == EOnaRotationMode::VelocityDirection;
	bLookingDirection = rotationMode == EOnaRotationMode::LookingDirection;
	bAiming = rotationMode == EOnaRotationMode::Aiming;
}
